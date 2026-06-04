#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4Threading.hh"

RunAction::RunAction() : G4UserRunAction() {
    auto man = G4AnalysisManager::Instance();
    man->SetDefaultFileType("csv");

    // Elimina ogni riferimento a CreateH1 qui sopra se vuoi smettere di generare file inutili!

    man->CreateNtuple("Micro", "Dati_Microdosimetria");
    man->CreateNtupleDColumn("EdepNuc_keV");
    man->CreateNtupleDColumn("yNuc_keV_um");
    man->CreateNtupleDColumn("zNuc_mGy");
    man->CreateNtupleDColumn("EdepCyto_keV");
    man->CreateNtupleDColumn("yCyto_keV_um");
    man->CreateNtupleDColumn("zCyto_mGy");
    man->CreateNtupleDColumn("TotalTrackLength_um");
    man->CreateNtupleDColumn("CytoTrackLength_um");
    man->FinishNtuple();
}

RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run*) {
    if (G4Threading::IsMasterThread()) { 
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->OpenFile("../output/microdosimetria.csv"); // Per metterlo fuori dalla cartella di build
    }
}

void RunAction::EndOfRunAction(const G4Run*) {
    if (G4Threading::IsMasterThread()) {
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->Write();
        analysisManager->CloseFile();
    }
}