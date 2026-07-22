#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeometryManager.hh"
#include "G4UserLimits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
  fHalfWorldSize = 20 * micrometer; // Portato a micrometri per una scala cellulare sensata!
  fNuclRadius    = 5 * micrometer;  // Esempio di raggio nucleare tipico
  fCytoThickness = 10 * micrometer; // Spessore citoplasma
  fpMaxStepSize  = 0.1 * micrometer;
  fpTrackingCut  = 0.0 * mm;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::~DetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Definizione del materiale acqua tramite il NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* waterMaterial = nistManager->FindOrBuildMaterial("G4_WATER");

  // World
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(fHalfWorldSize);

  auto* sWorld = new G4Box("World", fHalfWorldSize, fHalfWorldSize, fHalfWorldSize);
  auto* worldLV = new G4LogicalVolume(sWorld, waterMaterial, "World_LV");
  G4VPhysicalVolume* worldPV = new G4PVPlacement(nullptr, G4ThreeVector(), worldLV, "World", nullptr, false, 0);

  // Nucleus (dove idealmente potresti concentrare la sorgente di Cu-64)
  auto* sNucl = new G4Sphere("Nucl", 0., fNuclRadius, 0., twopi, 0., pi);
  auto* nuclLV = new G4LogicalVolume(sNucl, waterMaterial, "Nucl_LV");
  new G4PVPlacement(nullptr, G4ThreeVector(), nuclLV, "Nucl", worldLV, false, 0);

  // Cytoplasm
  auto* sCyto = new G4Sphere("Cyto", fNuclRadius, fNuclRadius + fCytoThickness, 0., twopi, 0., pi);
  auto* cytoLV = new G4LogicalVolume(sCyto, waterMaterial, "Crucial_Cyto_LV", 0, 0, 0); // O semplicemente cytoLV
  // Ripristiniamo la creazione corretta del volume fisico del citoplasma:
  new G4PVPlacement(nullptr, G4ThreeVector(), new G4LogicalVolume(sCyto, waterMaterial, "Cyto_LV"), "Cyto", worldLV, false, 0);

  // User limits (opzionali, utili se vuoi limitare lo step size per precisione estrema)
  G4UserLimits* limits = new G4UserLimits(fpMaxStepSize);
  worldLV->SetUserLimits(limits);
  nuclLV->SetUserLimits(limits);
  // cytoLV->SetUserLimits(limits); // se vuoi applicarlo anche qui

  // PrintParameters(); // Scommentalo se hai definito questo metodo nel tuo header

  return worldPV;
}