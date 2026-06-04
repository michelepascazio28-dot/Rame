#ifndef MicrodosimetrySD_h
#define MicrodosimetrySD_h 1

#include "G4VSensitiveDetector.hh"
#include "EventAction.hh"

class G4Step;

class MicrodosimetrySD : public G4VSensitiveDetector
{
  public:
    MicrodosimetrySD(const G4String& name, EventAction* eventAction = nullptr);
    ~MicrodosimetrySD() override = default;

    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void SetEventAction(EventAction* ea) { fEventAction = ea; }
  private:
    EventAction* fEventAction;
};
#endif