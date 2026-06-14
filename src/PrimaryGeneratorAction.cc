#include "PrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction()
{
    fParticleSource = new G4GeneralParticleSource(); // Qui uso (GeneralParticleSource) GPS che è una classe più complessa che gestisce più sorgenti. Il GPS mi permette di gestire Posizione, Distribuzione angolare, Distribuzione energetica a differenza del ParticleGun (PG) che è più semplice e gestisce solo una sorgente con distribuzione uniforme.
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    fParticleSource->GeneratePrimaryVertex(anEvent);
}