#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

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

  // --- CREAZIONE ISTOGRAMMI CON RANGE E BINNING DEDICATI ---
  
  // 0: Dummy
  analysisManager->CreateH1("0", "dummy", 100, 0., 1.);

  // 1: Positroni Beta+ (0 -> 1.0 MeV)
  analysisManager->CreateH1("1", "energy spectrum (%): positron (beta+)", 200, 0.0, 1.0);

  // 2: Neutrini (0 -> 1.0 MeV)
  analysisManager->CreateH1("2", "energy spectrum (%): nu_e anti_nu_e", 200, 0.0, 1.0);

  // 3: Gamma e X (0 -> 1.5 MeV)
  analysisManager->CreateH1("3", "energy spectrum (%): gamma", 300, 0.0, 1.5);

  // 4: Alpha (0 -> 10 MeV)
  analysisManager->CreateH1("4", "energy spectrum (%): alpha", 100, 0.0, 10.0);

  // 5: Ioni di rinculo (0 -> 0.1 MeV)
  analysisManager->CreateH1("5", "energy spectrum (%): ions", 100, 0.0, 0.1);

  // 6: Q-value totale (0 -> 2.0 MeV)
  analysisManager->CreateH1("6", "total kinetic energy per single decay (Q)", 200, 0.0, 2.0);

  // 7: Bilancio di impulso
  analysisManager->CreateH1("7", "momentum balance", 100, 0.0, 2.0);

  // 8: Lifetime catena (in secondi, ad es. fino a 100 ore)
  analysisManager->CreateH1("8", "total time of life of decay chain (s)", 200, 0.0, 360000.0);

  // 9: Energia visibile totale
  analysisManager->CreateH1("9", "total visible energy in decay chain", 200, 0.0, 2.0);

  // 10: Elettroni Beta- e Conversione Interna (0 -> 1.0 MeV)
  analysisManager->CreateH1("10", "energy spectrum: electron (beta-)", 200, 0.0, 1.0);

  // 11: Elettroni Auger / Bassa energia (< 20 keV) -> Binning fine da 10 eV/bin!
  analysisManager->CreateH1("11", "energy spectrum: electron (Auger/IC <20keV)", 2000, 0.0, 20.0);

  // ATTIVAZIONE AUTOMATICA: attiva tutti gli istogrammi creati
  for (G4int i = 0; i < analysisManager->GetNofH1s(); ++i) {
    analysisManager->SetH1Activation(i, true);
  }
}