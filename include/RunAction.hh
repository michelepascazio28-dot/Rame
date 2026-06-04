#ifndef RunAction_hh
#define RunAction_hh 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    G4int GetYId() const { return fH1_yNuc; }
    G4int GetZId() const { return fH1_zNuc; }

    G4int GetYCytoId() const { return fH1_yCyto; }
    G4int GetZCytoId() const { return fH1_zCyto; }

    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run*) override;
    
private:
    G4int fH1_yNuc; // ID dell'istogramma per y (Nucleo)
    G4int fH1_zNuc; // ID dell'istogramma per z (Nucleo)
    G4int fH1_yCyto; // ID dell'istogramma per y (Citoplasma)
    G4int fH1_zCyto; // ID dell'istogramma per z (Citoplasma)
};

#endif