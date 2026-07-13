#include "PhysicsListMessenger.hh"

#include "PhysicsList.hh"

#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
  : G4UImessenger(), fPhysicsList(pPhys), fPhysDir(0), fListCmd(0)
{
  fPhysDir = new G4UIdirectory("/microyz/phys/");
  fPhysDir->SetGuidance("Physics list commands");

  fListCmd = new G4UIcmdWithAString("/microyz/phys/addPhysics", this);
  fListCmd->SetGuidance("Add modular physics list");
  fListCmd->SetParameterName("PList", false);
  fListCmd->AvailableForStates(G4State_PreInit);
  fListCmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::~PhysicsListMessenger()
{
  delete fListCmd;
  delete fPhysDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fListCmd) {
    fPhysicsList->AddPhysicsList(newValue);
  }
}
