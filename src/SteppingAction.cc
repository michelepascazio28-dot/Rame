#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

SteppingAction::SteppingAction(EventAction* eventAction) 
 : G4UserSteppingAction(), fEventAction(eventAction) 
{
    auto runManager = G4RunManager::GetRunManager();
    auto det = static_cast<const DetectorConstruction*>(runManager->GetUserDetectorConstruction());
    
    fLogicNucleus = det->GetLogicNucleus();
    fLogicCell    = det->GetLogicCell();

    if (!fLogicNucleus) G4cout << "ATTENZIONE: fLogicNucleus non trovato!" << G4endl;
    if (!fLogicCell)    G4cout << "ATTENZIONE: fLogicCell non trovato!" << G4endl;
}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) 
{
    G4Track* track = step->GetTrack();
    
    // 1. Filtro particella: Solo elettroni (e-)
    if (track->GetDefinition()->GetParticleName() != "e-") return;

    // 2. Filtro energetico fondamentale per isolare gli Auger (< 10 keV)
    G4double eKin = track->GetKineticEnergy();
    if (eKin >= 10.0 * keV) return; 

    G4double edep = step->GetTotalEnergyDeposit();
    G4double stepLen = step->GetStepLength();
    G4LogicalVolume* vol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    // 3. Sempre contare la lunghezza totale
    fEventAction->AddAugerTotalLength(stepLen);

    // 4. Logica Additiva per Nucleo e Citoplasma basata sui volumi logici
    if (vol == fLogicNucleus) {
        // --- Energia ---
        fEventAction->AddNucleusEdep(edep);
        fEventAction->AddCytoplasmEdep(edep); // Additivo (il nucleo è dentro la cellula)
        
        // --- Lunghezza ---
        fEventAction->AddAugerTotalLength(stepLen);
        fEventAction->AddAugerCytoLength(stepLen); // Additivo
    } 
    else if (vol == fLogicCell) {
        // --- Energia ---
        fEventAction->AddCytoplasmEdep(edep);
        
        // --- Lunghezza ---
        fEventAction->AddAugerCytoLength(stepLen);
    }
}


