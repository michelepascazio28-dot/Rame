#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4Region.hh"
#include "G4UserLimits.hh"
#include "MicrodosimetrySD.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4EmParameters.hh"
#include "G4LossTableManager.hh"

#include <cfloat>
#include "G4SolidStore.hh"
#include "G4VSolid.hh"

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fWorldSize(30.0 * um),
   fCellRadius(10.0 * um),
   fNucleusRadius(3.0 * um),
   fpTrackingCut(7.4 * eV),
   fpMaxStepSize(1.0 * nm),
   fNucleusMass(0.),
   fNucleusChordLength(0.)
{
    // Il corpo del costruttore ora è correttamente definito
}

DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    DefineMaterials();
    return DefineVolumes();
}

void DetectorConstruction::DefineMaterials()
{
    G4NistManager* man = G4NistManager::Instance();
    fpWaterMaterial = man->FindOrBuildMaterial("G4_WATER");
    
    // Creazione materiale citoplasma (semplificato)
    G4Material* cytoplasm = new G4Material("Cytoplasm", 1.05*g/cm3, 1);
    cytoplasm->AddMaterial(fpWaterMaterial, 1.0);
    fpCytoplasmMaterial = cytoplasm;
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
    // 1. Mondo (Box)
    auto* worldS = new G4Box("World_Sol", fWorldSize/2, fWorldSize/2, fWorldSize/2);
    fLogicWorld = new G4LogicalVolume(worldS, fpWaterMaterial, "World_Log");
    auto* worldPV = new G4PVPlacement(nullptr, G4ThreeVector(), fLogicWorld, "World_Phys", nullptr, false, 0);

    // 2. Cellula (Sfera)
    auto* cellS = new G4Sphere("Cell_Sol", 0., fCellRadius, 0., 360.*deg, 0., 180.*deg);
    fLogicCell = new G4LogicalVolume(cellS, fpCytoplasmMaterial, "Cell_Log");
    new G4PVPlacement(nullptr, G4ThreeVector(), fLogicCell, "Cell_Phys", fLogicWorld, false, 0);

    // 3. Nucleo (Sfera)
    auto* nucleusS = new G4Sphere("Nucleus_Sol", 0., fNucleusRadius, 0., 360.*deg, 0., 180.*deg);
    fLogicNucleus = new G4LogicalVolume(nucleusS, fpWaterMaterial, "Nucleus_Log");
    
    // Calcoli geometrici per la Microdosimetria
    G4double vol = nucleusS->GetCubicVolume();
    G4double density = fLogicNucleus->GetMaterial()->GetDensity();
    fNucleusMass = vol * density;
    
    // Mean Chord Length = 4V / S
    fNucleusChordLength = 4.0 * vol / nucleusS->GetSurfaceArea();

    new G4PVPlacement(nullptr, G4ThreeVector(), fLogicNucleus, "Nucleus_Phys", fLogicCell, false, 0);

    // 4. Configurazione DNA (Region e UserLimits)
    fDNARegion = new G4Region("TargetDNARegion");
    fLogicCell->SetRegion(fDNARegion);
    fLogicNucleus->SetRegion(fDNARegion);

    // 1. Attiva la de-eccitazione atomica globale (ma deve essere accoppiata alla regione)
    G4EmParameters* param = G4EmParameters::Instance();
    param->SetAuger(true);
    param->SetAugerCascade(true);
    param->SetFluo(true);
    param->SetDeexcitationIgnoreCut(true);

    auto* userLimits = new G4UserLimits(fpMaxStepSize, DBL_MAX, DBL_MAX, fpTrackingCut);
    fDNARegion->SetUserLimits(userLimits);

    return worldPV;
}

G4double DetectorConstruction::GetNucleusMass() const {
    auto solid = G4SolidStore::GetInstance()->GetSolid("Nucleus_Sol");
    if (!solid) return 0.;
    
    // Debug: stampa cosa succede
    G4double density = fpWaterMaterial->GetDensity();
    if (density <= 0.) {
        G4cout << "ATTENZIONE: La densità del materiale è 0!" << G4endl;
    }
    
    return solid->GetCubicVolume() * density;
}

G4double DetectorConstruction::GetNucleusChord() const {
    auto solid = G4SolidStore::GetInstance()->GetSolid("Nucleus_Sol");
    if (!solid) return 1.0; 
    return 4.0 * solid->GetCubicVolume() / solid->GetSurfaceArea();
}

void DetectorConstruction::UpdateGeometryParameters()
{
    // Recupera il puntatore al solido tramite il suo nome
    G4VSolid* solidNucleus = ::G4SolidStore::GetInstance()->GetSolid("Nucleus_Sol");
    if (solidNucleus) {
        G4double volume = solidNucleus->GetCubicVolume();
        G4double surface = solidNucleus->GetSurfaceArea();
        
        // Assicurati che il materiale sia stato definito prima di accedere alla densità
        G4double density = fLogicNucleus->GetMaterial()->GetDensity();
        
        fNucleusMass = volume * density;
        fNucleusChordLength = 4.0 * volume / surface;
        
        G4cout << "--- Parametri Nucleo Aggiornati ---" << G4endl;
        G4cout << "Massa Nucleo: " << fNucleusMass/gram << " g" << G4endl;
        G4cout << "Chord Length: " << fNucleusChordLength/um << " um" << G4endl;
    }
}

void DetectorConstruction::ConstructSDandField()
{
    // Ora passi anche fEventAction
    fMicroSD = new MicrodosimetrySD("MicrodosimetrySD", fEventAction); 
    G4SDManager::GetSDMpointer()->AddNewDetector(fMicroSD);

    fLogicNucleus->SetSensitiveDetector(fMicroSD);
    fLogicCell->SetSensitiveDetector(fMicroSD);
}

G4double DetectorConstruction::GetCytoplasmMass() const {
    auto solidCell = G4SolidStore::GetInstance()->GetSolid("Cell_Sol");
    auto solidNuc = G4SolidStore::GetInstance()->GetSolid("Nucleus_Sol");
    
    if (!solidCell || !solidNuc) return 0.;
    
    G4double volCyto = solidCell->GetCubicVolume() - solidNuc->GetCubicVolume();
    // Usa la densità definita nel materiale del citoplasma
    G4double density = fpCytoplasmMaterial->GetDensity();
    
    return volCyto * density;
}

G4double DetectorConstruction::GetCytoplasmChord() const {
    auto solidCell = G4SolidStore::GetInstance()->GetSolid("Cell_Sol");
    auto solidNuc = G4SolidStore::GetInstance()->GetSolid("Nucleus_Sol");
    
    if (!solidCell || !solidNuc) return 1.0;
    
    // Per un guscio sferico, la corda media si calcola come 4 * V / S_totale
    // S_totale è la somma della superficie esterna e interna
    G4double volCyto = solidCell->GetCubicVolume() - solidNuc->GetCubicVolume();
    G4double surfCyto = solidCell->GetSurfaceArea() + solidNuc->GetSurfaceArea();
    
    return 4.0 * volCyto / surfCyto;
}

// Metodi setter
void DetectorConstruction::SetTrackingCut(G4double val) { fpTrackingCut = val; }
void DetectorConstruction::SetMaxStepSize(G4double val) { fpMaxStepSize = val; }