#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4Positron.hh"
#include "G4VProcess.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction() : G4UserSteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  const G4Track* track = step->GetTrack();

  // Interessano solo i positroni
  if (track->GetDefinition() != G4Positron::Positron()) return;

  const G4StepPoint* postStep = step->GetPostStepPoint();
  const G4VProcess* proc = postStep->GetProcessDefinedStep();

  // Selezione rigorosa: solo vera annichilazione (processo "annihil",
  // G4eplusAnnihilation), non un generico fStopAndKill, che scatta
  // anche quando il positrone esce dal volume World.
  if (!proc || proc->GetProcessName() != "annihil") return;

  const G4ThreeVector& creationVertex = track->GetVertexPosition();
  const G4ThreeVector& annihilationPoint = postStep->GetPosition();

  G4double r_creation = creationVertex.mag();
  G4double r_annihilation = annihilationPoint.mag();
  G4double displacement = (annihilationPoint - creationVertex).mag();

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(10, r_creation);
  analysisManager->FillH1(11, r_annihilation);
  analysisManager->FillH1(12, displacement);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......