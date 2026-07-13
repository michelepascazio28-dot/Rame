#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4RandomDirection.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr)
{
  G4int nofParticles = 1;
  fParticleGun = std::make_unique<G4ParticleGun>(nofParticles);
  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  fParticleGun->SetParticleEnergy(0.0 * eV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Prendiamo la geom del detector e facciamo la stessa cosa che facevamo in svalue
  auto* det = dynamic_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double cytoThickness = 0.0;
  G4double nuclRadius = 0.0;
  if (det) {
    cytoThickness = det->GetCytoThickness();
    nuclRadius = det->GetNuclRadius();
  }


  if (fParticleGun->GetParticleDefinition() == nullptr) {
    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    if (particleDefinition) {
      fParticleGun->SetParticleDefinition(particleDefinition);
    }
  }
  // Per ora si impostano solo 2 distribuzioni
  // Distribuzione uniforme in tutto e solo il nucleo
  G4double R_min = 0.0;
  G4double R_max = nuclRadius;

  // Distribuzione uniforme in tutto il nucleo e il citoplasma
  // G4 R_min = 0.0;
  // G4 R_max = nuclRadius + cytoThickness;

  G4double randVal = G4UniformRand();
  G4double myRadius = std::cbrt(randVal * std::pow(R_max, 3));

  G4double phi = CLHEP::twopi * G4UniformRand();
  G4double cosTheta = 2.0 * G4UniformRand() - 1.0;
  G4double theta = std::acos(cosTheta);

  G4double rx = myRadius * std::sin(theta) * std::cos(phi);
  G4double ry = myRadius * std::sin(theta) * std::sin(phi);
  G4double rz = myRadius * std::cos(theta);

  fParticleGun->SetParticlePosition(G4ThreeVector(rx, ry, rz));
  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
