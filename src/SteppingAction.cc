#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4LogicalVolume.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh" // Per stampare meglio i log

SteppingAction::SteppingAction(EventAction* eventAction) 
 : G4UserSteppingAction(), fEventAction(eventAction) 
{
    auto runManager = G4RunManager::GetRunManager();
    auto det = static_cast<const DetectorConstruction*>(runManager->GetUserDetectorConstruction());
    
    fLogicNucleus = det->GetLogicNucleus();
    fLogicCell    = det->GetLogicCell();

    // Debug: Verifica se i volumi sono stati trovati correttamente
    if (!fLogicNucleus) G4cout << "ATTENZIONE: fLogicNucleus non trovato!" << G4endl;
    if (!fLogicCell)    G4cout << "ATTENZIONE: fLogicCell non trovato!" << G4endl;
}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) 
{
    G4Track* track = step->GetTrack();
    if (track->GetDefinition()->GetParticleName() != "e-") return;
    
    G4double edep = step->GetTotalEnergyDeposit();
    G4double stepLen = step->GetStepLength();
    G4LogicalVolume* vol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    // 1. Sempre contare la lunghezza totale (indipendentemente dal volume)
    fEventAction->AddAugerTotalLength(stepLen);

    // 2. Logica Additiva per Nucleo e Citoplasma
    if (vol == fLogicNucleus) {
        // --- Energia ---
        fEventAction->AddNucleusEdep(edep);
        fEventAction->AddCytoplasmEdep(edep); // Additivo
        
        // --- Lunghezza (Fondamentale per Lineal Energy!) ---
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