#include "MicrodosimetrySD.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

MicrodosimetrySD::MicrodosimetrySD(const G4String& name, EventAction* eventAction)
 : G4VSensitiveDetector(name), fEventAction(eventAction)
{}

G4bool MicrodosimetrySD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;

    // 1. Recupero dinamico e super-sicuro dell'EventAction
    auto runManager = G4RunManager::GetRunManager();
    auto eventAction = static_cast<EventAction*>(
        const_cast<G4UserEventAction*>(runManager->GetUserEventAction()));

    // 2. Se per qualche motivo strano non lo trova, esci senza crashare
    if (!eventAction) {
        return false;
    }

    // 3. Ora usa "eventAction" locale, non "fEventAction" (che era NULL)
    G4String volName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();

    if (volName == "Nucleus_Log") {
        eventAction->AddNucleusEdep(edep);
        eventAction->AddCytoplasmEdep(edep);
    } else if (volName == "Cell_Log") {
        eventAction->AddCytoplasmEdep(edep);
    }
    
    return true;
}