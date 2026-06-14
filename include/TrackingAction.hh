#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

// Pre-dichiarazione della classe EventAction per evitare inclusioni cicliche
class EventAction;

class TrackingAction : public G4UserTrackingAction
{
  public:
    TrackingAction(EventAction* eventAction);
    virtual ~TrackingAction();

    // Questo metodo viene eseguito ogni volta che Geant4 inizia a tracciare una nuova particella
    virtual void PreUserTrackingAction(const G4Track* track) override;
    
    // Questo metodo viene eseguito quando la particella finisce il suo percorso (lasciato vuoto se non serve)
    virtual void PostUserTrackingAction(const G4Track* track) override;

  private:
    EventAction* fEventAction; // Puntatore all'EventAction per salvare l'ID
};

#endif