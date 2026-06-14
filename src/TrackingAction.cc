#include "TrackingAction.hh"
#include "EventAction.hh" // Assicurati che il nome del file corrisponda alla tua EventAction

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4SystemOfUnits.hh"

TrackingAction::TrackingAction(EventAction* eventAction)
 : G4UserTrackingAction(),
   fEventAction(eventAction)
{}

TrackingAction::~TrackingAction()
{}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    // Il nucleo di Rame-64 primario ha ParentID = 0 e TrackID = 1.
    // I prodotti diretti del suo decadimento (elettroni beta, positroni, Auger, X) hanno ParentID = 1.
    if (track->GetParentID() == 1) { 
        
        // Recuperiamo il processo che ha generato questa particella secondaria
        const G4VProcess* creatorProcess = track->GetCreatorProcess();
        
        if (creatorProcess) {
            G4int subType = creatorProcess->GetProcessSubType();
            
            // Verifichiamo se l'EventAction non ha ancora registrato il processo iniziale per questo evento
            if (fEventAction && fEventAction->GetProcessID() == 0) {
                
                // Salviamo l'ID del processo (sarà 51 per il RadioactiveDecay)
                fEventAction->SetProcessID(subType);

                G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
                
                
                // Stampa di debug opzionale per verificare cosa stiamo catturando
                if (eventID < 10) {
                    G4cout << "DEBUG: Evento inizializzato da processo SubType: " << subType 
                        << " (" << creatorProcess->GetProcessName() << ")" << G4endl;
                    }
            }
        }
    }
}

void TrackingAction::PostUserTrackingAction(const G4Track* /*track*/)
{
    // Lasciato vuoto: non serve logica alla fine del tracciamento per questo scopo
}