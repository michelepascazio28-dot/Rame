#include "TrackingAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "Run.hh"
#include "TrackingMessenger.hh"

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4AnalysisManager.hh"
#include "G4VProcess.hh"
#include "G4Positron.hh"
#include "G4Electron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4Gamma.hh"
#include "G4Alpha.hh"

TrackingAction::TrackingAction(EventAction* event) : fEvent(event)
{
  fTrackMessenger = new TrackingMessenger(this);
}

TrackingAction::~TrackingAction()
{
  delete fTrackMessenger;
}

void TrackingAction::SetTimeWindow(G4double t1, G4double dt)
{
  fTimeWindow1 = t1;
  fTimeWindow2 = fTimeWindow1 + dt;
}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
  Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  G4ParticleDefinition* particle = track->GetDefinition();
  G4String name = particle->GetParticleName();
  G4double Ekin = track->GetKineticEnergy(); // in MeV
  G4int ID = track->GetTrackID();

  G4double meanLife = particle->GetPDGLifeTime();
  run->ParticleCount(name, Ekin, meanLife);

  // ---- PRODOTTI DI DECADIMENTO ED EFFETTI ATOMICI SECONDARI (ID > 1) ----
  if (ID > 1) {
    auto analysisManager = G4AnalysisManager::Instance();

    const G4VProcess* creator = track->GetCreatorProcess();
    G4String procName = creator ? creator->GetProcessName() : "";

    if (G4StrUtil::contains(procName, "RadioactiveDecay") || G4StrUtil::contains(procName, "Decay")) {

      G4double globalTime = fParentDecayTime;

      if (particle == G4Positron::Positron()) {
        analysisManager->FillH1(1, Ekin / CLHEP::MeV);
        analysisManager->FillH1(8, globalTime);
      }
      else if (particle == G4Electron::Electron()) {
        analysisManager->FillH1(10, Ekin / CLHEP::MeV);
        analysisManager->FillH1(8, globalTime);

        if (Ekin <= 20.0 * CLHEP::keV) {
          analysisManager->FillH1(11, Ekin / CLHEP::keV);
        }
      }
      else if (particle == G4NeutrinoE::NeutrinoE() || particle == G4AntiNeutrinoE::AntiNeutrinoE()) {
        analysisManager->FillH1(2, Ekin / CLHEP::MeV);
      }
      else if (particle == G4Gamma::Gamma()) {
        analysisManager->FillH1(3, Ekin / CLHEP::MeV);
      }
      else if (particle->GetPDGCharge() > 2. || particle->GetParticleType() == "nucleus") {
        analysisManager->FillH1(5, Ekin / CLHEP::MeV);
      }
    }
  }

  // ---- IONE PRIMARIO ----
  if (particle->GetPDGCharge() > 2. || particle->GetParticleType() == "nucleus") {
    if (ID == 1) {
      fEvent->AddDecayChain(name);
    } else {
      fEvent->AddDecayChain(" ---> " + name);
    }
    fTimeBirth = track->GetGlobalTime();
  }
}

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
  if (track->GetDefinition()->GetPDGCharge() < 3.) return;

  Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();

  G4double time = track->GetGlobalTime();
  G4int ID = track->GetTrackID();
  if (ID == 1) {
    run->PrimaryTiming(time);
    fParentDecayTime = time;   // tempo vero del decadimento, in ns (unità native Geant4)
  }

  const std::vector<const G4Track*>* secondaries = track->GetStep()->GetSecondaryInCurrentStep();
  size_t nbtrk = secondaries ? secondaries->size() : 0;

  if (nbtrk) {
    G4double EkinTot = 0., EkinVis = 0.;
    G4ThreeVector Pbalance = -track->GetMomentum();
    for (size_t itr = 0; itr < nbtrk; itr++) {
      const G4Track* trk = (*secondaries)[itr];
      G4ParticleDefinition* particle = trk->GetDefinition();
      G4double Ekin = trk->GetKineticEnergy();
      EkinTot += Ekin;
      G4bool visible = !((particle == G4NeutrinoE::NeutrinoE()) || (particle == G4AntiNeutrinoE::AntiNeutrinoE()));
      if (visible) EkinVis += Ekin;
      if (particle != G4Gamma::Gamma()) Pbalance += trk->GetMomentum();
    }
    G4double Pbal = Pbalance.mag();
    run->Balance(EkinTot, Pbal);
    analysis->FillH1(6, EkinTot / CLHEP::MeV);
    fEvent->AddQvalue(EkinTot / CLHEP::MeV);
    analysis->FillH1(7, Pbal / CLHEP::MeV);
    fEvent->AddEvisible(EkinVis);
  }

  if (!nbtrk) {
    run->EventTiming(time);
    fTimeEnd = DBL_MAX;
  }

  run->SetTimeWindow(fTimeWindow1, fTimeWindow2);
  G4String name = track->GetDefinition()->GetParticleName();
  G4bool life1(false), life2(false), decay(false);
  if ((fTimeBirth <= fTimeWindow1) && (fTimeEnd > fTimeWindow1)) life1 = true;
  if ((fTimeBirth <= fTimeWindow2) && (fTimeEnd > fTimeWindow2)) life2 = true;
  if ((fTimeEnd > fTimeWindow1) && (fTimeEnd < fTimeWindow2)) decay = true;
  if (life1 || life2 || decay) run->CountInTimeWindow(name, life1, life2, decay);
}