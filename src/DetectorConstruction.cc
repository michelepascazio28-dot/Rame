#include "DetectorConstruction.hh"

#include "DetectorMessenger.hh"
#include "TrackerSD.hh"
#include "CalorimeterSD_Nucl.hh"
#include "CalorimeterSD_Cyto.hh"

#include "G4GeometryManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4UserLimits.hh"

#include "G4PhysicalConstants.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction()
{
  // defaults
  fpTrackingCut = 7.4 * CLHEP::eV;
  fpMaxStepSize = DBL_MAX;
  fRadius = 210 * CLHEP::nm;
  fWorldRadius = 1 * CLHEP::mm;
  fCytoThickness = 1 * CLHEP::um;
  fNuclRadius = 4 * CLHEP::um;
  fHalfWorldSize = 1 * CLHEP::mm;

  DefineMaterials();

  fDetectorMessenger = std::make_unique<DetectorMessenger>(this);
}

DetectorConstruction::~DetectorConstruction() = default;

// keep default destructor (unique_ptr will clean up)

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

void DetectorConstruction::DefineMaterials()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* H2O = nist->FindOrBuildMaterial("G4_WATER");
  fpWaterMaterial = H2O;
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // World
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(fHalfWorldSize);

  auto* sWorld = new G4Box("World", fHalfWorldSize, fHalfWorldSize, fHalfWorldSize);

  auto* worldLV = new G4LogicalVolume(sWorld, fpWaterMaterial, "World_LV");

  G4VPhysicalVolume* worldPV = new G4PVPlacement(nullptr, G4ThreeVector(), worldLV, "World", nullptr, false, 0);

  // Nucleus
  auto* sNucl = new G4Sphere("Nucl", 0., fNuclRadius, 0., twopi, 0., pi);
  auto* nuclLV = new G4LogicalVolume(sNucl, fpWaterMaterial, "Nucl_LV");
  new G4PVPlacement(nullptr, G4ThreeVector(), nuclLV, "Nucl", worldLV, false, 0);

  // Cytoplasm
  auto* sCyto = new G4Sphere("Cyto", fNuclRadius, fNuclRadius + fCytoThickness, 0., twopi, 0., pi);
  auto* cytoLV = new G4LogicalVolume(sCyto, fpWaterMaterial, "Cyto_LV");
  new G4PVPlacement(nullptr, G4ThreeVector(), cytoLV, "Cyto", worldLV, false, 0);

  // User limits
  worldLV->SetUserLimits(new G4UserLimits(fpMaxStepSize, DBL_MAX, DBL_MAX, fpTrackingCut));
  nuclLV->SetUserLimits(new G4UserLimits(fpMaxStepSize, DBL_MAX, DBL_MAX, fpTrackingCut));
  cytoLV->SetUserLimits(new G4UserLimits(fpMaxStepSize, DBL_MAX, DBL_MAX, fpTrackingCut));

  PrintParameters();

  return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
  // Primo SD
  G4String trackerChamberSDname = "TrackerChamberSD";

  auto* aTrackerSD = new TrackerSD(trackerChamberSDname, "TrackerHitsCollection");
  // qui stiamo settando il raggio di SD
  aTrackerSD->SetRadius(fRadius);

  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);

  SetSensitiveDetector("Nucl_LV", aTrackerSD, true);

  // Secondo SD
  G4String CalorimeterSDname = "CalorimeterSD_Nucl";

  auto* aCalorimeterSD = new CalorimeterSD_Nucl(CalorimeterSDname, 7);

  G4SDManager::GetSDMpointer()->AddNewDetector(aCalorimeterSD);

  SetSensitiveDetector("Nucl_LV", aCalorimeterSD, true);

  // Secondo SD
  G4String CalorimeterSDname_1 = "CalorimeterSD_Cyto";

  auto* aCalorimeterSD_1 = new CalorimeterSD_Cyto(CalorimeterSDname_1, 8);

  G4SDManager::GetSDMpointer()->AddNewDetector(aCalorimeterSD_1);

  SetSensitiveDetector("Cyto_LV", aCalorimeterSD_1, true);
}

void DetectorConstruction::SetTrackingCut(const G4double& value)
{
  fpTrackingCut = value;
}

void DetectorConstruction::SetMaxStepSize(const G4double& value)
{
  fpMaxStepSize = value;
}

void DetectorConstruction::SetTrackerSDRadius(const G4double& value)
{
  fRadius = value;
}

void DetectorConstruction::SetNuclRadius(const G4double& value)
{
    fNuclRadius = value;
}

void DetectorConstruction::SetCytoThickness(const G4double& value)
{
    fCytoThickness = value;
}

void DetectorConstruction::SetHalfWorldSize(const G4double& value)
{
    fHalfWorldSize = value;
}

void DetectorConstruction::SetWorldMaterial(const G4String& materialName)
{
    G4NistManager* nist = G4NistManager::Instance();
    fpWaterMaterial = nist->FindOrBuildMaterial(materialName);
}

void DetectorConstruction::PrintParameters() const
{
  G4cout << "\n---------------------------------------------------------\n";
  G4cout << "---> The tracking cut is set to " << G4BestUnit(fpTrackingCut, "Energy") << G4endl;
  G4cout << "---> The maximum step size is set to " << G4BestUnit(fpMaxStepSize, "Length") << G4endl;
  G4cout << "---> The TrackerSD radius is set to " << G4BestUnit(fRadius, "Length") << G4endl;
  G4cout << "---> The Nucleus radius is " << G4BestUnit(fNuclRadius, "Length") << G4endl;
  G4cout << "---> The Cytoplasm thickness is " << G4BestUnit(fCytoThickness, "Length") << G4endl;
  G4cout << "---> The Half Size of the World is " << G4BestUnit(fHalfWorldSize, "Length") << G4endl;
  G4cout << "\n---------------------------------------------------------\n";
}
