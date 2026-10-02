#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    void AddDecayChain(G4String segment) { fDecayChain += segment; }
    void AddEvisible(G4double e) { fEvisTot += e; }
    void AddQvalue(G4double q) { fQvalueEvent += q; }

  private:
    G4String fDecayChain;
    G4double fEvisTot = 0.;
    G4double fQvalueEvent = 0.;
};

#endif