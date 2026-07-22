#include "ActionInitialization.hh"

#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
  RunAction* runAction = new RunAction(nullptr);
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  PrimaryGeneratorAction* primary = new PrimaryGeneratorAction();
  SetUserAction(primary);

  RunAction* runAction = new RunAction(primary);
  SetUserAction(runAction);

  EventAction* eventAction = new EventAction();
  SetUserAction(eventAction);

  TrackingAction* trackingAction = new TrackingAction(eventAction);
  SetUserAction(trackingAction);

  SteppingAction* steppingAction = new SteppingAction();
  SetUserAction(steppingAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
