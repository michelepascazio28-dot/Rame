#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"

#include "G4RandomDirection.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), G4VStateDependent(), fParticleGun(0), fDetector(0)
{
  fDetector = dynamic_cast<const DetectorConstruction*>(
    G4RunManager::GetRunManager()->GetUserDetectorConstruction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  G4StateManager::GetStateManager()->DeregisterDependent(this);
  if (fParticleGun) delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double thickness = fDetector->GetCytoThickness();
  G4double radius = fDetector->GetNuclRadius();

  // Distribuzione uniforme solo nel citoplasma escluso il nucleo
  G4double R_min = radius;
  G4double R_max = radius + thickness;

  // Distribuzione uniforme solo nel nucleo
  // G4double R_min = 0;
  // G4double R_max = radius;

  // Distribuzione uniforme in tutto il citoplasma e nucleo
  // G4double R_min = 0;
  // G4double R_max = radius + thickness;

  // Distribuzione uniforme solo sulla superficie del citoplasma
  // G4double R_min = radius + thickness;
  // G4double R_max = radius + thickness;


  G4double randVal = G4UniformRand();
  G4double myRadius = std::cbrt(std::pow(R_min, 3) + randVal * (std::pow(R_max, 3) - std::pow(R_min, 3)));

  G4double phi = CLHEP::twopi * G4UniformRand();            
  G4double cosTheta = 2.0 * G4UniformRand() - 1.0;          
  G4double theta = std::acos(cosTheta);

  G4double rx = myRadius* std::sin(theta) * std::cos(phi);
  G4double ry = myRadius * std::sin(theta) * std::sin(phi);
  G4double rz = myRadius * std::cos(theta);

  fParticleGun->SetParticlePosition(G4ThreeVector(rx, ry, rz));
  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  fParticleGun->GeneratePrimaryVertex(anEvent);

  // Provo con il mio metodo

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool PrimaryGeneratorAction::Notify(G4ApplicationState requestedState)
{
  if (requestedState == G4State_Idle) {
    if (fParticleGun != 0) return true;

    fParticleGun = new G4ParticleGun(1);

    // Define default primary

    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(0 * eV);
    fParticleGun->SetParticlePosition(G4ThreeVector());
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1, 0, 0));
  }

  return true;
}
