#include "SteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Positron.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction() {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) 
{
    G4Track* track = step->GetTrack();
    
    // Controlliamo se è un positrone e se la traccia sta per essere killata
    if (track->GetDefinition() == G4Positron::Positron()) {
        
        // qquando il positrone viene annichilito/spento, la traccia riceve lo stato fStopAndKill
        if (track->GetTrackStatus() == fStopAndKill) {

            G4ThreeVector creationVertex = track->GetVertexPosition();
            G4double r_creation = creationVertex.mag(); 

            G4ThreeVector annihilationVertex = track->GetPosition();
            G4double r_annihilation = annihilationVertex.mag();

            G4double r_diff = r_annihilation - r_creation;

            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            analysisManager->FillNtupleDColumn(0, 0, r_creation);
            analysisManager->FillNtupleDColumn(0, 1, r_annihilation);
            analysisManager->FillNtupleDColumn(0, 2, r_diff);
            analysisManager->AddNtupleRow(0);

            analysisManager->FillH1(10, r_creation);
            analysisManager->FillH1(11, r_annihilation);
            analysisManager->FillH1(12, r_diff);
        }
    }
}