#include "EventAction.hh"

#include "HistoManager.hh"
#include "Run.hh"

#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction() : G4UserEventAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
  // Energy deposited per event
  fCellEdep = 0.;
  fNuclEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  if (fCellEdep > 0.) {
    run->AddCellEdep(fCellEdep);
    analysisManager->FillH1(5, fCellEdep);
  }

  if (fNuclEdep > 0.) {
    run->AddNuclEdep(fNuclEdep);
    analysisManager->FillH1(6, fNuclEdep);
  }

  // Energia nel solo citoplasma (fCellEdep = cellula intera, fNuclEdep = nucleo)
  G4double cytoEdep = std::max(0., fCellEdep - fNuclEdep);
  if (cytoEdep > 0.) {
    run->AddTotEdep(cytoEdep);   // fTotEdeposit ora = citoplasma
  }
}
