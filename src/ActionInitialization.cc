#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "MicrodosimetrySD.hh"
#include "TrackingAction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* det)
 : G4VUserActionInitialization(), fDet(det)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());
}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction());

    RunAction* runAction = new RunAction();
    SetUserAction(runAction);

    EventAction* eventAction = new EventAction(fDet); 
    SetUserAction(eventAction);

    MicrodosimetrySD* sd = fDet->GetMicroSD(); // GetMicroSD() è un getter in DetectorConstruction.hh per ottenere il puntatore alla nostra MicrodosimetrySD
    if (sd) {
        sd->SetEventAction(eventAction);
    }

    SetUserAction(new SteppingAction(eventAction));

    SetUserAction(new TrackingAction(eventAction));
}