#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh" // Include necessario!
#include "globals.hh"

class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event*) override;
    G4GeneralParticleSource* GetParticleSource() { return fParticleSource; }

  private:
    G4GeneralParticleSource* fParticleSource = nullptr;
};

#endif