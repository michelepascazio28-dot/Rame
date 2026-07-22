#include "HistoManager.hh"

#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetFileName("Cu64_simulation");
  analysisManager->SetNtupleMerging(true); 
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);

  // Define histograms start values
  const G4int kMaxHisto = 13;
  const G4String id[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", 
    "10", "11", "12" // Nuovi ID per R_Creation, R_Annihilation, R_Diff
  };
  const G4String title[] = {
    "dummy",  // 0
    "energy spectrum (%): e+ e-",  // 1
    "energy spectrum (%): nu_e anti_nu_e",  // 2
    "energy spectrum (%): gamma",  // 3
    "energy spectrum (%): alpha",  // 4
    "energy spectrum (%): ions",  // 5
    "total kinetic energy per single decay (Q)",  // 6
    "momentum balance",  // 7
    "total time of life of decay chain",  // 8
    "total visible energy in decay chain",  // 9
    "R creation distribution",  // 10
    "R annihilation distribution",  // 11
    "R difference distribution"   // 12
  };

  // Default values (to be reset via /analysis/h1/set command)
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated
  // as we have not yet set nbins, vmin, vmax
  for (G4int k = 0; k < kMaxHisto; k++) {
    G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, false);
  }

  // Tupla aggiuntiva
  analysisManager->CreateNtuple("PositronAnnihilation", "Creation and Annihilation radii");
  analysisManager->CreateNtupleDColumn("R_Creation");      // Colonna 0
  analysisManager->CreateNtupleDColumn("R_Annihilation");  // Colonna 1
  analysisManager->CreateNtupleDColumn("R_Diff");          // Colonna 2
  analysisManager->FinishNtuple();
}