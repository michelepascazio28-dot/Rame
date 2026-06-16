#include "PhysicsList.hh"

#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4DNAGenericIonsManager.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmDNAPhysics_option1.hh"
#include "G4EmDNAPhysics_option2.hh"
#include "G4EmDNAPhysics_option3.hh"
#include "G4EmDNAPhysics_option4.hh"
#include "G4EmDNAPhysics_option5.hh"
#include "G4EmDNAPhysics_option6.hh"
#include "G4EmDNAPhysics_option7.hh"
#include "G4EmDNAPhysics_option8.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4IonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

#include "G4RadioactiveDecayPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4LossTableManager.hh"
#include "G4VAtomDeexcitation.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4EmParameters.hh"            // Fondamentale per i flag Auger
#include "G4DeexPrecoParameters.hh"      // Per la de-eccitazione nucleare
#include "G4NuclearLevelData.hh"
#include "G4Radioactivation.hh"

#include <memory>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
    // fMessenger = std::make_unique<PhysicsListMessenger>(this);
    // SetVerboseLevel(1);

    // 1. CONFIGURAZIONE "PERFETTA" (Presa dal tuo secondo esempio)
    // Queste impostazioni influenzano TUTTE le liste fisiche che userai
    G4EmParameters* emParams = G4EmParameters::Instance();
    emParams->SetAugerCascade(true);
    emParams->SetDeexcitationIgnoreCut(true);
    emParams->SetFluo(true);

    // Flag per la de-eccitazione nucleare
    G4DeexPrecoParameters* deex = G4NuclearLevelData::GetInstance()->GetParameters();
    deex->SetCorrelatedGamma(false);
    deex->SetStoreAllLevels(true);
    deex->SetInternalConversionFlag(true);
    deex->SetIsomerProduction(true);
    deex->SetMaxLifeTime(1 * nanosecond);

    // 2. REGISTRAZIONE FISICA
    RegisterPhysics(new G4RadioactiveDecayPhysics()); // Registra il decadimento
    fEmPhysicsList = std::make_unique<G4EmDNAPhysics_option4>(); // La tua scelta DNA
    RegisterPhysics(fEmPhysicsList.get());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList() = default;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  G4BosonConstructor pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();

  G4DNAGenericIonsManager* genericIonsManager;
  genericIonsManager = G4DNAGenericIonsManager::Instance();
  genericIonsManager->GetIon("alpha++");
  genericIonsManager->GetIon("alpha+");
  genericIonsManager->GetIon("helium");
  genericIonsManager->GetIon("hydrogen");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
    // Esegui i processi standard modulari
    G4VModularPhysicsList::ConstructProcess();

    // 3. SE VUOI FORZARE LA DE-ECCITAZIONE (opzionale se hai già impostato G4EmParameters sopra)
    G4LossTableManager* man = G4LossTableManager::Instance();
    G4VAtomDeexcitation* deex = man->AtomDeexcitation();
    if (nullptr == deex) {
        deex = new G4UAtomicDeexcitation();
        man->SetAtomDeexcitation(deex);
    }
    deex->InitialiseAtomicDeexcitation();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::AddPhysicsList(const G4String& name)
{
  if (verboseLevel > -1) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }

  if (name == fEmName) return;

  if (name == "dna") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmDNAPhysics>();
  }
  else if (name == "dna_opt1") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmDNAPhysics_option1>();
  }
  else if (name == "dna_opt2") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmDNAPhysics_option2>();
  }
  else if (name == "dna_opt3") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmDNAPhysics_option3>();
  }
  else if (name == "dna_opt4") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmDNAPhysics_option4>();
  }
  else if (name == "dna_opt5") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmDNAPhysics_option5>();
  }
  else if (name == "dna_opt6") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmDNAPhysics_option6>();
  }
  else if (name == "dna_opt7") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmDNAPhysics_option7>();
  }
  else if (name == "dna_opt8") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmDNAPhysics_option8>();
  }
  else if (name == "liv") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmLivermorePhysics>();
  }
  else if (name == "pene") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmPenelopePhysics>();
  }
  else if (name == "emstandard_opt4") {
    fEmName = name;
    fEmPhysicsList = std::make_unique<G4EmStandardPhysics_option4>();
  }
  else {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
           << " is not defined" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::AddTrackingCut()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  auto particleIterator = GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)()) {
    G4ParticleDefinition* particle = particleIterator->value();
    G4String particleName = particle->GetParticleName();

    if (particleName == "e-") {
      ph->RegisterProcess(new G4UserSpecialCuts(), particle);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::AddMaxStepSize()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  auto particleIterator = GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)()) {
    G4ParticleDefinition* particle = particleIterator->value();
    G4String particleName = particle->GetParticleName();

    if (particleName == "e-") {
      ph->RegisterProcess(new G4StepLimiter(), particle);
    }
  }
}

void PhysicsList::SetCuts()
{
    // Questo forza Geant4 a simulare interazioni fino a 1 nm
    G4double cutValue = 1.0 * nanometer;
    
    // Applica a tutti i tagli standard
    SetCutValue(cutValue, "gamma");
    SetCutValue(cutValue, "e-");
    SetCutValue(cutValue, "e+");
    SetCutValue(cutValue, "proton");
    
    // Importante: stampa a video che i tagli sono stati impostati
    G4cout << ">>> PHYSICS LIST: Tagli impostati a " << cutValue/nanometer << " nm" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
