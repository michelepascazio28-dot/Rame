/// \file PhysicsList.cc
/// \brief Implementation of the PhysicsList class

#include "PhysicsList.hh"

#include "G4DeexPrecoParameters.hh"
#include "G4EmBuilder.hh"
#include "G4IonConstructor.hh"
#include "G4LossTableManager.hh"
#include "G4NuclearLevelData.hh"
#include "G4NuclideTable.hh"
#include "G4ParticleTypes.hh"
#include "G4PhysListUtil.hh"
#include "G4PhysicsListHelper.hh"
#include "G4Radioactivation.hh"
#include "G4SystemOfUnits.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
{
  // instantiate Physics List infrastructure
  //
  G4PhysListUtil::InitialiseParameters();

  // update G4NuclideTable time limit
  //
  const G4double meanLife = 1 * nanosecond;
  G4NuclideTable::GetInstance()->SetMeanLifeThreshold(meanLife);
  G4NuclideTable::GetInstance()->SetLevelTolerance(1.0 * eV);

  // define flags for the atomic de-excitation module
  //
  G4EmParameters::Instance()->SetDefaults();
  G4EmParameters::Instance()->SetAugerCascade(true);
  G4EmParameters::Instance()->SetDeexcitationIgnoreCut(true);

  // define flags for nuclear gamma de-excitation model
  //
  G4DeexPrecoParameters* deex = G4NuclearLevelData::GetInstance()->GetParameters();
  deex->SetCorrelatedGamma(false);
  deex->SetStoreAllLevels(true);
  deex->SetInternalConversionFlag(true);
  deex->SetIsomerProduction(true);
  deex->SetMaxLifeTime(meanLife);

  // set default cut in range value
  //
  SetDefaultCutValue(1 * mm);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  // minimal set of particles for EM physics and radioactive decay
  //
  G4EmBuilder::ConstructMinimalEmSet();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  AddTransportation();

  G4Radioactivation* radioactiveDecay = new G4Radioactivation();

  G4bool ARMflag = true;
  radioactiveDecay->SetARM(ARMflag);  // Atomic Rearangement

  // EM physics constructor is not used in this example, so
  // it is needed to instantiate and to initialize atomic deexcitation
  //
  G4LossTableManager* man = G4LossTableManager::Instance();
  G4VAtomDeexcitation* deex = man->AtomDeexcitation();
  if (nullptr == deex) {
    deex = new G4UAtomicDeexcitation();
    man->SetAtomDeexcitation(deex);
  }
  deex->InitialiseAtomicDeexcitation();

  // register radioactiveDecay
  //
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
