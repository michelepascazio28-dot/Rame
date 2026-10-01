#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  // Inizializzazione di base del cannone
  fParticleGun->SetParticleEnergy(0. * eV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.)); // Origine fissa
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Verifica ed eventuale assegnazione dello ione Cu-64
  G4ParticleDefinition* particle = fParticleGun->GetParticleDefinition();
  
  if (!particle || particle->GetParticleName() != "Cu64") {
    G4int Z = 29, A = 64;
    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z, A, 0. * keV);
    if (ion) {
      fParticleGun->SetParticleDefinition(ion);
      fParticleGun->SetParticleCharge(0. * eplus);
    }
  }

  // Sorgente puntiforme esatta al centro del World (0, 0, 0)
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));

  // Genera il vertice primario (il decadimento del Cu-64 a riposo)
  fParticleGun->GeneratePrimaryVertex(anEvent);
}