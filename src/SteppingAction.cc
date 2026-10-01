#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "G4Track.hh"

#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* event, DetectorConstruction* detector)
  : G4UserSteppingAction(), fEventAction(event), fDetectorConstruction(detector)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep <= 0.) return;

  const G4Track* track = aStep->GetTrack();
  const G4int trackID = track->GetTrackID();

  // Ignore energy deposited by the primary ion itself and count only secondary tracks.
  if (trackID == 1) return;

  // Total energy deposit in the whole cell (cytoplasm + nucleus) from secondaries only

  const G4LogicalVolume* volume = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();
  
  // qui vado ad aggiungere l'energia depositata nell'intera cellula (citoplasma + nucleo)
  if (volume == fDetectorConstruction->GetCytoLogicalVolume() ||
      volume == fDetectorConstruction->GetNuclLogicalVolume()) {
    fEventAction->AddCellEdep(edep);
  }
  // qui rimango come prima a sommare l'energia depositata solo nel nucleo 
  if (volume == fDetectorConstruction->GetNuclLogicalVolume()) {
    fEventAction->AddNuclEdep(edep);
  }

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Step size of primary particle or charged secondaries

  G4double steplen = aStep->GetStepLength();
  if (track->GetTrackID() == 1)
    analysisManager->FillH1(8, steplen);
  else if (track->GetDefinition()->GetPDGCharge() != 0.)
    analysisManager->FillH1(9, steplen);
}
