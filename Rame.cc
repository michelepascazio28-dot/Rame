#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh" // Assicurati che il nome sia corretto

int main(int argc, char** argv)
{
    // 1. Rilevamento modalità interattiva
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // 2. Creazione del Run Manager tramite Factory
    // Usa 'Default' per creare il manager corretto automaticamente
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

    // 3. Inizializzazione Detector, Physics e Actions
    DetectorConstruction* detector = new DetectorConstruction();
    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization()); // Assicurati che ActionInitialization accetti il puntatore!

    // 5. Inizializzazione visualizzazione
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // 6. Gestione comandi UI
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        // Modalità grafica
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    } else {
        // Modalità batch
        G4String fileName = argv[1];
        G4String command = "/control/execute ";
        
        // DEBUG: Stampa cosa sta cercando di eseguire
        G4cout << ">>> Esecuzione batch: " << command + fileName << G4endl;
        
        G4int status = UImanager->ApplyCommand(command + fileName);
        
        // DEBUG: Stampa se il comando ha avuto successo
        if (status != 0) {
            G4cerr << ">>> ERRORE: Il comando ha restituito status " << status << G4endl;
        }
    }

    // 7. Pulizia finale
    delete visManager;
    delete runManager;

    return 0;
}