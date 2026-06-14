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
    // Identifichiamo i prodotti diretti del decadimento del Rame-64
    if (track->GetParentID() == 1) { 
        
        const G4VProcess* creatorProcess = track->GetCreatorProcess();
        
        if (creatorProcess) {
            G4int subType = creatorProcess->GetProcessSubType();
            
            // Salviamo direttamente l'ID reale (210) fornito da Geant4
            if (fEventAction && fEventAction->GetProcessID() == 0) {
                fEventAction->SetProcessID(subType);

                G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
                
                // Manteniamo la stampa solo per i primi 10 eventi per controllo
                if (eventID < 10) {
                    G4cout << "DEBUG: Evento " << eventID << " inizializzato da " 
                           << creatorProcess->GetProcessName() << " (ID reale: " << subType << ")" << G4endl;
                }
            }
        }
    }
}


void TrackingAction::PostUserTrackingAction(const G4Track* /*track*/)
{
    // Lasciato vuoto: non serve logica alla fine del tracciamento per questo scopo
}