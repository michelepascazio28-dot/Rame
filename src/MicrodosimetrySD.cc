#include "MicrodosimetrySD.hh"
#include "G4Step.hh"
#include "G4EventManager.hh"

MicrodosimetrySD::MicrodosimetrySD(const G4String& name, EventAction* eventAction)
 : G4VSensitiveDetector(name), fEventAction(eventAction)
{}

G4bool MicrodosimetrySD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;

    // RECUPERO DINAMICO (Sicuro)
    auto eventAction = static_cast<EventAction*>(
        const_cast<G4UserEventAction*>(G4EventManager::GetEventManager()->GetUserEventAction()));

    if (!eventAction) return false;

    G4String volName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();

    if (volName == "Nucleus_Log") {
        eventAction->AddNucleusEdep(edep);
    } else if (volName == "Cell_Log") {
        eventAction->AddCytoplasmEdep(edep);
    }
    return true;
    
}