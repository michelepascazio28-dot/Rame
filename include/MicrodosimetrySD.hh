#ifndef MicrodosimetrySD_h
#define MicrodosimetrySD_h 1

#include "G4VSensitiveDetector.hh"
#include "MicrodosimetryHit.hh"
#include "G4SystemOfUnits.hh" // Assicurati di includerlo

class G4Step;
class G4HCofThisEvent;

class MicrodosimetrySD : public G4VSensitiveDetector
{
  public:
    MicrodosimetrySD(const G4String& name, const G4String& hitsCollectionName);
    ~MicrodosimetrySD() override = default;

    void Initialize(G4HCofThisEvent* hce) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent* hce) override;

    void SetMicroRadius(G4double val) { fMicroRadius = val; }

  private:
    MicrodosimetryHitsCollection* fHitsCollection = nullptr;
    G4double fMicroRadius = 20.0 * nm; 
    G4int fHCID = -1;
};

#endif