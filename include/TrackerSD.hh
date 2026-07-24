#ifndef TrackerSD_h
#define TrackerSD_h 1

#include "TrackerHit.hh"

#include "G4VSensitiveDetector.hh"

class TrackerSD : public G4VSensitiveDetector
{
  public:
    TrackerSD(const G4String& name, const G4String& hitsCollectionName);
    ~TrackerSD() override;

    // Methods from base class
    void Initialize(G4HCofThisEvent* hitCollection) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;

    // Local method
    void SetRadius(const G4double& value);

  private:
    TrackerHitsCollection* fHitsCollection;

    // Sphere radius set to 5 nm by default
    G4double fRadius = 5 * CLHEP::nm;
};

#endif
