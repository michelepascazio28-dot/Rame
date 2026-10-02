#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

HistoManager::HistoManager()
{
  Book();
}

void HistoManager::Book()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetFileName("Cu64_simulation");
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);

  analysisManager->CreateH1("0", "dummy", 100, 0., 1.);
  analysisManager->CreateH1("1", "energy spectrum: positron (beta+)", 200, 0.0, 1.0);
  analysisManager->CreateH1("2", "energy spectrum: nu_e anti_nu_e", 200, 0.0, 1.0);
  analysisManager->CreateH1("3", "energy spectrum: gamma", 300, 0.0, 1.5);
  analysisManager->CreateH1("4", "energy spectrum: alpha", 100, 0.0, 10.0);
  analysisManager->CreateH1("5", "energy spectrum: ions", 100, 0.0, 0.1);
  analysisManager->CreateH1("6", "total kinetic energy per single decay (Q)", 200, 0.0, 2.0);
  analysisManager->CreateH1("7", "momentum balance", 100, 0.0, 2.0);
  analysisManager->CreateH1("8", "total time of life of decay chain (s)", 200, 0.0, 360000.0);
  analysisManager->CreateH1("9", "total visible energy in decay chain", 200, 0.0, 2.0);
  analysisManager->CreateH1("10", "energy spectrum: electron (beta-)", 200, 0.0, 1.0);
  analysisManager->CreateH1("11", "energy spectrum: electron (Auger/IC <20keV)", 2000, 0.0, 20.0);
  analysisManager->CreateH1("12", "total kinetic energy per full decay chain (event)", 200, 0.0, 2.5);

  for (G4int i = 0; i < analysisManager->GetNofH1s(); ++i) {
    analysisManager->SetH1Activation(i, true);
  }
}