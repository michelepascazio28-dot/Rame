#include "Run.hh"

#include "HistoManager.hh"
#include "MyFile.hh"

#ifdef MYFILE
#  include "MyPrimaryGeneratorActionFromFile.hh"
#else
#  include "PrimaryGeneratorAction.hh"
#endif

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::Run(const DetectorConstruction* detector)
  : G4Run(),
    fDetector(detector),
    fParticle(0),
    fEkin(0.),
    fCellEdeposit(0.),
    fCellEdeposit2(0.),
    fNuclEdeposit(0.),
    fNuclEdeposit2(0.),
    fTrackLen(0.),
    fTrackLen2(0.),
    fProjRange(0.),
    fProjRange2(0.),
    fNbOfSteps(0),
    fNbOfSteps2(0),
    fStepSize(0.),
    fStepSize2(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::~Run() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::SetPrimary(G4ParticleDefinition* particle, G4double energy)
{
  fParticle = particle;
  fEkin = energy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::AddCellEdep(G4double e)
{
  fCellEdeposit += e;
  fCellEdeposit2 += e * e;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::AddNuclEdep(G4double e)
{
  fNuclEdeposit += e;
  fNuclEdeposit2 += e * e;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::AddTrackLength(G4double t)
{
  fTrackLen += t;
  fTrackLen2 += t * t;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::AddProjRange(G4double x)
{
  fProjRange += x;
  fProjRange2 += x * x;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::AddStepSize(G4int nb, G4double st)
{
  fNbOfSteps += nb;
  fNbOfSteps2 += nb * nb;
  fStepSize += st;
  fStepSize2 += st * st;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::Merge(const G4Run* run)
{
  const Run* localRun = static_cast<const Run*>(run);

  // Pass information about primary particle

  fParticle = localRun->fParticle;
  fEkin = localRun->fEkin;

  // Accumulate sums

  fCellEdeposit += localRun->fCellEdeposit;
  fCellEdeposit2 += localRun->fCellEdeposit2;
  fNuclEdeposit += localRun->fNuclEdeposit;
  fNuclEdeposit2 += localRun->fNuclEdeposit2;

  fTrackLen += localRun->fTrackLen;
  fTrackLen2 += localRun->fTrackLen2;
  fProjRange += localRun->fProjRange;
  fProjRange2 += localRun->fProjRange2;
  fNbOfSteps += localRun->fNbOfSteps;
  fNbOfSteps2 += localRun->fNbOfSteps2;
  fStepSize += localRun->fStepSize;
  fStepSize2 += localRun->fStepSize2;

  G4Run::Merge(run);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::EndOfRun()
{
  std::ios::fmtflags mode = G4cout.flags();
  G4cout.setf(std::ios::fixed, std::ios::floatfield);
  G4int prec = G4cout.precision(2);

  // Run conditions

  G4String partName = fParticle->GetParticleName();

  G4cout << "\n ======================== run summary =====================\n";
  G4cout << "\n The run is " << numberOfEvent << " " << partName << " of "
         << G4BestUnit(fEkin, "Energy") << G4endl;

  if (numberOfEvent == 0) {
    G4cout.setf(mode, std::ios::floatfield);
    G4cout.precision(prec);
    return;
  }

  // Compute S-value for cytoplasm (C<-C)

  fCellEdeposit /= numberOfEvent;
  fCellEdeposit2 /= numberOfEvent;
  G4double rmsCyto = fCellEdeposit2 - fCellEdeposit * fCellEdeposit;
  if (rmsCyto > 0.)
    rmsCyto = std::sqrt(rmsCyto);
  else
    rmsCyto = 0.;

  G4cout.precision(6);
  G4cout << "\n Total Energy deposited in cytoplasm = " << G4BestUnit(fCellEdeposit, "Energy")
         << " +- " << G4BestUnit(rmsCyto, "Energy") << G4endl;

  G4double sValueCyto = fCellEdeposit / fDetector->GetCytoMass();
  G4double rmsSValueCyto = rmsCyto / fDetector->GetCytoMass();
  sValueCyto *= 1000.; // Convert from Gy/Bq.s to mGy/Bq.s
  rmsSValueCyto *= 1000.; // Convert from Gy/Bq.s to mGy/Bq.s

  G4cout.precision(6);
  G4cout << "\n S value for cytoplasm (C<-C) = " << sValueCyto / gray << " mGy/Bq.s "
         << " +- " << rmsSValueCyto / gray << " mGy/Bq.s " << G4endl;

  // Compute S-value for nucleus (N<-C)

  fNuclEdeposit /= numberOfEvent;
  fNuclEdeposit2 /= numberOfEvent;
  G4double rmsNucl = fNuclEdeposit2 - fNuclEdeposit * fNuclEdeposit;
  if (rmsNucl > 0.)
    rmsNucl = std::sqrt(rmsNucl);
  else
    rmsNucl = 0.;

  G4cout.precision(6);
  G4cout << "\n Total Energy deposited in nucleus = " << G4BestUnit(fNuclEdeposit, "Energy")
         << " +- " << G4BestUnit(rmsNucl, "Energy") << G4endl;

  G4double sValueNucl = fNuclEdeposit / fDetector->GetNuclMass();
  G4double rmsSValueNucl = rmsNucl / fDetector->GetNuclMass();
  sValueNucl *= 1000.; // Convert from Gy/Bq.s to mGy/Bq.s
  rmsSValueNucl *= 1000.; // Convert from Gy/Bq.s to mGy/Bq

  G4cout.precision(6);
  G4cout << "\n S value for nucleus (N<-C) = " << sValueNucl / gray << " mGy/Bq.s "
         << " +- " << rmsSValueNucl / gray << " mGy/Bq.s " << G4endl;

  // Compute track length of primary track

  fTrackLen /= numberOfEvent;
  fTrackLen2 /= numberOfEvent;
  G4double rms = fTrackLen2 - fTrackLen * fTrackLen;
  if (rms > 0.)
    rms = std::sqrt(rms);
  else
    rms = 0.;

  G4cout.precision(3);
  G4cout << "\n Track length of primary track = " << G4BestUnit(fTrackLen, "Length") << " +- "
         << G4BestUnit(rms, "Length");

  // Compute projected range of primary track

  fProjRange /= numberOfEvent;
  fProjRange2 /= numberOfEvent;
  rms = fProjRange2 - fProjRange * fProjRange;
  if (rms > 0.)
    rms = std::sqrt(rms);
  else
    rms = 0.;

  G4cout << "\n Projected range               = " << G4BestUnit(fProjRange, "Length") << " +- "
         << G4BestUnit(rms, "Length") << G4endl;

  // Nb of steps and step size of primary track

  G4double dNofEvents = double(numberOfEvent);
  G4double fNbSteps = fNbOfSteps / dNofEvents, fNbSteps2 = fNbOfSteps2 / dNofEvents;
  rms = fNbSteps2 - fNbSteps * fNbSteps;
  if (rms > 0.)
    rms = std::sqrt(rms);
  else
    rms = 0.;

  G4cout.precision(2);
  G4cout << "\n Nb of steps of primary track  = " << fNbSteps << " +- " << rms << G4endl;

  fStepSize /= numberOfEvent;
  fStepSize2 /= numberOfEvent;
  rms = fStepSize2 - fStepSize * fStepSize;
  if (rms > 0.)
    rms = std::sqrt(rms);
  else
    rms = 0.;

  G4cout.precision(6);
  G4cout << "\n Step size                     = " << G4BestUnit(fStepSize, "Length") << " +- "
         << G4BestUnit(rms, "Length") << G4endl;

  // Normalize histograms of longitudinal energy profile

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4int ih = 1;
  G4double binWidth = analysisManager->GetH1Width(ih);
  G4double fac = (1. / (numberOfEvent * binWidth)) * (mm / MeV);
  analysisManager->ScaleH1(ih, fac);

  // Reset default formats

  G4cout.setf(mode, std::ios::floatfield);
  G4cout.precision(prec);

  // Output file

  FILE* myFile;
  myFile = fopen("s.txt", "a");
  fprintf(myFile, "%e %e %e %e %e %e %e \n", fDetector->GetNuclRadius() / nm,
          fDetector->GetCytoThickness() / nm, fEkin / eV, sValueCyto / gray, rmsSValueCyto / gray,
          sValueNucl / gray, rmsSValueNucl / gray);
  fclose(myFile);
}
