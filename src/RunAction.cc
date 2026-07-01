#include "RunAction.hh"

#include "HistoManager.hh"
#include "MyFile.hh"
#include "Run.hh"

#ifdef MYFILE
#  include "MyPrimaryGeneratorActionFromFile.hh"
#else
#  include "PrimaryGeneratorAction.hh"
#endif

#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction() : G4UserRunAction(), fpDetector(0), fpRun(0), fpHistoManager(0)
{
  fpDetector = dynamic_cast<const DetectorConstruction*>(
    G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  // Book predefined histograms
  fpHistoManager = new HistoManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fpHistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{
  fpRun = new Run(fpDetector);
  return fpRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  // If needed, save Rndm status
  // G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  // if (isMaster) G4Random::showEngineStatus();

  // Histograms

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (analysisManager->IsActive()) {
    analysisManager->OpenFile();
  }

  // Selection of primary generator

#ifdef MYFILE

  const MyPrimaryGeneratorActionFromFile* primary =
    dynamic_cast<const MyPrimaryGeneratorActionFromFile*>(
      G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

#else
  const PrimaryGeneratorAction* primary = dynamic_cast<const PrimaryGeneratorAction*>(
    G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

#endif

  if (!primary) return;  //

  // Keep run condition

  G4ParticleDefinition* particle = primary->GetParticleGun()->GetParticleDefinition();

  G4double energy = primary->GetParticleGun()->GetParticleEnergy();

  fpRun->SetPrimary(particle, energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
  if (isMaster) fpRun->EndOfRun();

  // Save histograms

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (analysisManager->IsActive()) {
    analysisManager->Write();
    analysisManager->CloseFile();
  }

  // If needed, show Rndm status
  // if (isMaster) G4Random::showEngineStatus();
}
