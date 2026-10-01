
#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
  : G4UImessenger(),
    fDetector(Det),
    fTestemDir(0),
    fDetDir(0),
    fMaterWorldCmd(0),
    fMaterCytoCmd(0),
    fMaterNuclCmd(0),
    fNuclRadiusCmd(0),
    fCytoThicknessCmd(0),
    fTrackingCutCmd(0)
{
  fTestemDir = new G4UIdirectory("/svalue/");
  fTestemDir->SetGuidance("Detector control");

  fDetDir = new G4UIdirectory("/svalue/det/");
  fDetDir->SetGuidance("Detector construction commands");

  fMaterWorldCmd = new G4UIcmdWithAString("/svalue/det/setWorldMat", this);
  fMaterWorldCmd->SetGuidance("Select material of the World");
  fMaterWorldCmd->SetParameterName("choice", false);
  fMaterWorldCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fMaterWorldCmd->SetToBeBroadcasted(false);

  fMaterNuclCmd = new G4UIcmdWithAString("/svalue/det/setNuclMat", this);
  fMaterNuclCmd->SetGuidance("Select material of the nucleus");
  fMaterNuclCmd->SetParameterName("choice", false);
  fMaterNuclCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fMaterNuclCmd->SetToBeBroadcasted(false);

  fMaterCytoCmd = new G4UIcmdWithAString("/svalue/det/setCytoMat", this);
  fMaterCytoCmd->SetGuidance("Select material of the cytoplasm");
  fMaterCytoCmd->SetParameterName("choice", false);
  fMaterCytoCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fMaterCytoCmd->SetToBeBroadcasted(false);

  fNuclRadiusCmd = new G4UIcmdWithADoubleAndUnit("/svalue/det/setNuclRadius", this);
  fNuclRadiusCmd->SetGuidance("Set radius of the nucleus");
  fNuclRadiusCmd->SetParameterName("Radius", false);
  fNuclRadiusCmd->SetRange("Radius>0.");
  fNuclRadiusCmd->SetUnitCategory("Length");
  fNuclRadiusCmd->AvailableForStates(G4State_PreInit);
  fNuclRadiusCmd->SetToBeBroadcasted(false);

  fCytoThicknessCmd = new G4UIcmdWithADoubleAndUnit("/svalue/det/setCytoThickness", this);
  fCytoThicknessCmd->SetGuidance("Set thickness of the cytoplasm");
  fCytoThicknessCmd->SetParameterName("Thickness", false);
  fCytoThicknessCmd->SetRange("Thickness>0.");
  fCytoThicknessCmd->SetUnitCategory("Length");
  fCytoThicknessCmd->AvailableForStates(G4State_PreInit);
  fCytoThicknessCmd->SetToBeBroadcasted(false);

  fTrackingCutCmd = new G4UIcmdWithADoubleAndUnit("/svalue/det/setTrackingCut", this);
  fTrackingCutCmd->SetGuidance("Set tracking cut in the absorber");
  fTrackingCutCmd->SetParameterName("Cut", false);
  fTrackingCutCmd->SetRange("Cut>0.");
  fTrackingCutCmd->SetUnitCategory("Energy");
  fTrackingCutCmd->AvailableForStates(G4State_PreInit);
  fTrackingCutCmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fMaterWorldCmd;
  delete fMaterCytoCmd;
  delete fMaterNuclCmd;
  delete fNuclRadiusCmd;
  delete fCytoThicknessCmd;
  delete fTrackingCutCmd;
  delete fDetDir;
  delete fTestemDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fMaterWorldCmd) {
    fDetector->SetWorldMaterial(newValue);
  }

  if (command == fMaterCytoCmd) {
    fDetector->SetCytoMaterial(newValue);
  }

  if (command == fMaterNuclCmd) {
    fDetector->SetNuclMaterial(newValue);
  }

  if (command == fNuclRadiusCmd) {
    fDetector->SetNuclRadius(fNuclRadiusCmd->GetNewDoubleValue(newValue));
  }

  if (command == fCytoThicknessCmd) {
    fDetector->SetCytoThickness(fCytoThicknessCmd->GetNewDoubleValue(newValue));
  }

  if (command == fTrackingCutCmd) {
    fDetector->SetTrackingCut(fTrackingCutCmd->GetNewDoubleValue(newValue));
  }
}
