#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4LogicalVolume.hh"

class EventAction; // Forward declaration

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();
    virtual void UserSteppingAction(const G4Step*);

private:
    EventAction* fEventAction; // Il puntatore salvato
    G4LogicalVolume* fLogicNucleus = nullptr;
    G4LogicalVolume* fLogicCell = nullptr;
};

#endif