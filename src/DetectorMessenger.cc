#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
:G4UImessenger(), fpDetector(Det)
{
  fpTestDir = std::make_unique<G4UIdirectory>("/microyz/");
  fpTestDir->SetGuidance(" detector control.");

  fpDetDir = std::make_unique<G4UIdirectory>("/microyz/det/");
  fpDetDir->SetGuidance("detector construction commands");

  fpTrackingCutCmd =
    std::make_unique<G4UIcmdWithADoubleAndUnit>("/microyz/det/setTrackingCut", this);
  fpTrackingCutCmd->SetGuidance("Set tracking cut");
  fpTrackingCutCmd->SetParameterName("Cut", false);
  fpTrackingCutCmd->SetRange("Cut>0.");
  fpTrackingCutCmd->SetUnitCategory("Energy");
  fpTrackingCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fpTrackingCutCmd->SetToBeBroadcasted(false);

  fpMaxStepSizeCmd =
    std::make_unique<G4UIcmdWithADoubleAndUnit>("/microyz/det/setMaxStepSize", this);
  fpMaxStepSizeCmd->SetGuidance("Set maximum step size");
  fpMaxStepSizeCmd->SetParameterName("Size", false);
  fpMaxStepSizeCmd->SetRange("Size>0.");
  fpMaxStepSizeCmd->SetUnitCategory("Length");
  fpMaxStepSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fpMaxStepSizeCmd->SetToBeBroadcasted(false);

  fAddRadius = std::make_unique<G4UIcmdWithADoubleAndUnit>("/microyz/det/Radius", this);
  fpMaxStepSizeCmd->SetGuidance("Set TrackSD radius");
  fAddRadius->SetToBeBroadcasted(false);
  fAddRadius->SetParameterName("Radius", false);
  fAddRadius->SetRange("Radius>0.");
  fAddRadius->SetUnitCategory("Length");
  fAddRadius->AvailableForStates(G4State_PreInit, G4State_Idle);

  fSetNuclRadiusCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>("/microyz/det/setNuclRadius", this);
  fSetNuclRadiusCmd->SetGuidance("Set nucleus radius");
  fSetNuclRadiusCmd->SetParameterName("Radius", false);
  fSetNuclRadiusCmd->SetRange("Radius>0.");
  fSetNuclRadiusCmd->SetUnitCategory("Length");
  fSetNuclRadiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetNuclRadiusCmd->SetToBeBroadcasted(false);

  fSetCytoThicknessCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>("/microyz/det/setCytoThickness", this);
  fSetCytoThicknessCmd->SetGuidance("Set cytoplasm thickness");
  fSetCytoThicknessCmd->SetParameterName("Thickness", false);
  fSetCytoThicknessCmd->SetRange("Thickness>0.");
  fSetCytoThicknessCmd->SetUnitCategory("Length");
  fSetCytoThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetCytoThicknessCmd->SetToBeBroadcasted(false);

  fHalfWorldSizeCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>("/microyz/det/setHalfWorldSize", this);
  fHalfWorldSizeCmd->SetGuidance("Set Half World Size");
  fHalfWorldSizeCmd->SetParameterName("Size", false);
  fHalfWorldSizeCmd->SetRange("Size>0.");
  fHalfWorldSizeCmd->SetUnitCategory("Length");
  fHalfWorldSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fHalfWorldSizeCmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fpTrackingCutCmd.get()) {
    fpDetector->SetTrackingCut(fpTrackingCutCmd->GetNewDoubleValue(newValue));
  }

  if (command == fpMaxStepSizeCmd.get()) {
    fpDetector->SetMaxStepSize(fpMaxStepSizeCmd->GetNewDoubleValue(newValue));
  }

  if (command == fAddRadius.get()) {
    fpDetector->SetTrackerSDRadius(fAddRadius->GetNewDoubleValue(newValue));
  }

  if (command == fSetNuclRadiusCmd.get()) {
    fpDetector->SetNuclRadius(fSetNuclRadiusCmd->GetNewDoubleValue(newValue));
  }

  if (command == fSetCytoThicknessCmd.get()) {
    fpDetector->SetCytoThickness(fSetCytoThicknessCmd->GetNewDoubleValue(newValue));
  }

  if (command == fHalfWorldSizeCmd.get()) {
    fpDetector->SetHalfWorldSize(fHalfWorldSizeCmd->GetNewDoubleValue(newValue));
  }
}