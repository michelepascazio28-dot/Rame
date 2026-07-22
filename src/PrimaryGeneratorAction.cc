#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  // Nel costruttore impostiamo solo i valori base o lasciamo che sia la macro / gun a farlo.
  // Evitiamo di chiamare G4IonTable qui dentro perché la tabella non è ancora pronta!
  fParticleGun->SetParticleEnergy(0. * eV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // A questo punto (durante l'evento) la tabella delle particelle è pronta al 100%
  // Controlliamo se il cannone ha già lo ione Cu-64, altrimenti glielo assegniamo al volo:
  G4ParticleDefinition* particle = fParticleGun->GetParticleDefinition();
  
  if (!particle || particle->GetParticleName() != "Cu64") {
    G4int Z = 29, A = 64;
    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z, A, 0. * keV);
    if (ion) {
      fParticleGun->SetParticleDefinition(ion);
      fParticleGun->SetParticleCharge(0. * eplus);
    }
  }

  // Raggio del nucleo cellulare (es. 5 micrometri)
  G4double nuclRadius = 5. * micrometer; 

  // Campionamento uniforme in volume all'interno di una sfera
  G4double u = G4UniformRand();
  G4double r = nuclRadius * std::cbrt(u);
  
  G4double cosTheta = 2. * G4UniformRand() - 1.;
  G4double sinTheta = std::sqrt(1. - cosTheta * cosTheta);
  G4double phi = twopi * G4UniformRand();

  G4double x = r * sinTheta * std::cos(phi);
  G4double y = r * sinTheta * std::sin(phi);
  G4double z = r * cosTheta;

  // Assegniamo la posizione casuale generata
  fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));

  // Crea il vertice primario
  fParticleGun->GeneratePrimaryVertex(anEvent);
}