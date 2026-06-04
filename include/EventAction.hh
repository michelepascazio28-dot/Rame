#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class DetectorConstruction;
class G4Event;

class EventAction : public G4UserEventAction {
public:
    EventAction(const DetectorConstruction* det);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    // Metodi per accumulare dati
    void AddNucleusEdep(G4double edep) { fNucleusEdep += edep; }
    void AddCytoplasmEdep(G4double edep) { fCytoplasmEdep += edep; }
    void AddAugerTotalLength(G4double len) { fAugerLengthTotal += len; }
    void AddAugerCytoLength(G4double len) { fAugerLengthCyto += len; }

private:
    const DetectorConstruction* fDet;
    G4double fNucleusEdep;
    G4double fCytoplasmEdep;
    G4double fAugerLengthCyto = 0;  // Percorso solo nel citoplasma
    G4double fAugerLengthTotal = 0; // Percorso totale ovunque
};

#endif