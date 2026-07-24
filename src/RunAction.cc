#include "RunAction.hh"

#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction() : G4UserRunAction()
{
  // Create analysis manager
  G4cout << "##### Create analysis manager "
         << "  " << this << G4endl;
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->SetDefaultFileType("root");
  analysisManager->SetNtupleMerging(true);

  G4cout << "Using " << analysisManager->GetType() << " analysis manager" << G4endl;

  analysisManager->SetVerboseLevel(1);

  // Create ntuple

  analysisManager->CreateNtuple("yz", "yz-distributions");
  analysisManager->CreateNtupleDColumn("radius");
  analysisManager->CreateNtupleDColumn("eventID");
  analysisManager->CreateNtupleDColumn("nbHits");
  analysisManager->CreateNtupleDColumn("nbScoredHits");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("Einc");
  analysisManager->CreateNtupleDColumn("Nuclear_dose");
  analysisManager->CreateNtupleDColumn("Cytoplasmic_dose");

  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  // Open an output file

  auto analysisManager = G4AnalysisManager::Instance();
  G4String fileName = "yz";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
  // Print histogram statistics

  auto analysisManager = G4AnalysisManager::Instance();

  // Save histograms

  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
