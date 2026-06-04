#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "MicrodosimetrySD.hh"

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

    // Rimuovi 'runAction' dal costruttore di EventAction
    EventAction* eventAction = new EventAction(fDet); 
    SetUserAction(eventAction);

    // --- IL PONTE ---
    auto sd = fDet->GetMicroSD();
    if (sd) {
        sd->SetEventAction(eventAction);
    }
    // ----------------

    SetUserAction(new SteppingAction(eventAction));
}