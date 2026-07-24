#ifndef CalorimeterSD_Nucl_h
#define CalorimeterSD_Nucl_h

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4TouchableHistory;

class CalorimeterSD_Nucl : public G4VSensitiveDetector
{
public:
    CalorimeterSD_Nucl(const G4String& name, G4int colID);
    virtual ~CalorimeterSD_Nucl() override = default;

    virtual void Initialize(G4HCofThisEvent* hce) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    virtual void EndOfEvent(G4HCofThisEvent* hce) override;

private:
    G4double fEdepSum;
    G4double fMass;
    G4int fcolID;
};

#endif