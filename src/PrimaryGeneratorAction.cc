#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
{
  fParticleSource = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Il GPS gestisce tutto internamente basandosi sui comandi macro
  fParticleSource->GeneratePrimaryVertex(anEvent);
}