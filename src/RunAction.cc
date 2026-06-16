#include "RunAction.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction() : G4UserRunAction()
{
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetDefaultFileType("root");
    
    // CORREZIONE CRITICA: Disabilitiamo il merging per evitare l'errore
    // "Merging ntuples is not applicable in sequential application"
    analysisManager->SetNtupleMerging(false);

    G4cout << "Using " << analysisManager->GetType() << " analysis manager" << G4endl;
    analysisManager->SetVerboseLevel(1);

    // Creazione Ntuple
    // Assicurati che l'ordine sia identico a quello in MicrodosimetrySD.cc
    analysisManager->CreateNtuple("yz", "yz-distributions");
    
    analysisManager->CreateNtupleDColumn("radius");       // ID 0
    analysisManager->CreateNtupleDColumn("dummy");        // ID 1 (spazio vuoto, per allineamento)
    analysisManager->CreateNtupleDColumn("nbHits");       // ID 2
    analysisManager->CreateNtupleDColumn("nbScoredHits"); // ID 3
    analysisManager->CreateNtupleDColumn("y");            // ID 4
    analysisManager->CreateNtupleDColumn("z");            // ID 5
    analysisManager->CreateNtupleDColumn("Einc");         // ID 6
    
    analysisManager->FinishNtuple();
}

RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("yz"); // Crea yz.root
}

void RunAction::EndOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}