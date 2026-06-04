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
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

SteppingAction::SteppingAction(EventAction* eventAction) 
 : G4UserSteppingAction(), fEventAction(eventAction) 
{
    // Recupera i puntatori UNA SOLA VOLTA alla costruzione
    auto runManager = G4RunManager::GetRunManager();
    auto det = static_cast<const DetectorConstruction*>(runManager->GetUserDetectorConstruction());
    fLogicNucleus = det->GetLogicNucleus();
    fLogicCell    = det->GetLogicCell();
}

SteppingAction::~SteppingAction() 
{}

void SteppingAction::UserSteppingAction(const G4Step* step) 
{
    G4Track* track = step->GetTrack();
    if (track->GetDefinition()->GetParticleName() != "e-") return;
    
    G4double eKin = track->GetKineticEnergy();
    if (eKin >= 10.0 * keV) return; // Filtro energetico (solo Auger)

    G4double stepLen = step->GetStepLength();
    G4String volName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();

    // 1. Aggiorna sempre il totale
    fEventAction->AddAugerTotalLength(stepLen);

    // 2. Aggiorna il citoplasma solo se siamo lì dentro
    if (volName == "Cell_Log") {
        fEventAction->AddAugerCytoLength(stepLen);
    }
}

