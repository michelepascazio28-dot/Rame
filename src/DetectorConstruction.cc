#include "DetectorConstruction.hh"
#include "MicrodosimetrySD.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

// 1. Costruttore: Inizializza i valori
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(),
    // fDetectorMessanger = std::make_unique<DetectorMessenger>(this);
    fNucleusRadius(3.0 * um),
    fCellRadius(10.0 * um),
    fpTrackingCut(7.4 * eV)
{

}

DetectorConstruction::~DetectorConstruction() = default;

// 2. Metodo principale chiamato dal RunManager
G4VPhysicalVolume* DetectorConstruction::Construct() {
    DefineMaterials();
    return DefineVolumes();
}

void DetectorConstruction::DefineMaterials() {
    G4NistManager* nist = G4NistManager::Instance();

    // 1. Usa direttamente la variabile membro fpWaterMaterial (senza G4Material* davanti!)
    fpWaterMaterial = nist->FindOrBuildMaterial("G4_WATER");
    
    // 2. Definisci il Citoplasma (usa fpWaterMaterial che ora è valorizzato)
    G4double CytoDensity = 1.05 * g/cm3;
    fpCytoplasmMaterial = new G4Material("Cytoplasm", CytoDensity, fpWaterMaterial);

    // 3. Usa direttamente la variabile membro fpVacuumMaterial (senza G4Material* davanti!)
    fpVacuumMaterial = nist->FindOrBuildMaterial("G4_Galactic");
}

// 4. Definizione Geometria
G4VPhysicalVolume* DetectorConstruction::DefineVolumes() {
    // World
    G4Box* SWorld = new G4Box("Solid_World", 50*um, 50*um, 50*um);
    G4LogicalVolume* LWorld = new G4LogicalVolume(SWorld, fpVacuumMaterial, "Logical_World");
    G4PVPlacement* PWorld = new G4PVPlacement(nullptr, G4ThreeVector(), LWorld, "Physical_World", nullptr, false, 0);

    // Cell
    G4Sphere* SCell = new G4Sphere("Solid_Cell", 0., fCellRadius, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    // RIMOSSO "G4LogicalVolume*" per usare il membro della classe
    fLogicCell = new G4LogicalVolume(SCell, fpWaterMaterial, "Logical_Cell"); 
    G4PVPlacement* PCell = new G4PVPlacement(nullptr, G4ThreeVector(), fLogicCell, "Physical_Cell", LWorld, false, 0);

    // Nucleus
    G4Sphere* SNucleus = new G4Sphere("Solid_Nucleus", 0., fNucleusRadius, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    // RIMOSSO "G4LogicalVolume*" per usare il membro della classe
    fLogicNucleus = new G4LogicalVolume(SNucleus, fpWaterMaterial, "Logical_Nucleus");
    G4PVPlacement* PNucleus = new G4PVPlacement(nullptr, G4ThreeVector(), fLogicNucleus, "Physical_Nucleus", fLogicCell, false, 0);
    
    return PWorld;
}

// 5. Collegamento al Sensitive Detector
void DetectorConstruction::ConstructSDandField() {
    MicrodosimetrySD* sd = new MicrodosimetrySD("MicrodosimetrySD", "NucleusHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);

    if (fLogicNucleus){
        fLogicNucleus->SetSensitiveDetector(sd);
        G4cout << ">>> DEBUG: Sensitive Detector AGGANCIATO al Nucleus!" << G4endl;
    } else {
        G4cout << ">>> ERRORE: fLogicNucleus è NULLO!" << G4endl;
    }
}