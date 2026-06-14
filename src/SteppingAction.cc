#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"

SteppingAction::SteppingAction(EventAction* eventAction) 
 : G4UserSteppingAction(), fEventAction(eventAction) 
{
    auto runManager = G4RunManager::GetRunManager();
    auto det = static_cast<const DetectorConstruction*>(runManager->GetUserDetectorConstruction());
    
    fLogicNucleus = det->GetLogicNucleus(); // In DetectorConstruction.hh abbiamo un getter per il volume del nucleo
    fLogicCell    = det->GetLogicCell(); // In DetectorConstruction.hh abbiamo un getter per il volume della cellula
}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) // Questo è il cuore dello SteppingAction
{
    G4Track* track = step->GetTrack(); // GetTrack() è un metodo di G4Step, obbligatorio!!!
    // Se vogliamo un oggetto G4Track contiene tutte le informazioni sulla particella che sta facendo lo step, come la posizione, l'energia, il processo che ha causato lo step, ecc.
    
    // 1. Filtro particella: Solo Auger (e-)
    if (track->GetDefinition()->GetParticleName() != "e-") return; // Se non è un elettrone, esci subito

    // 2. Calcolo Lunghezze
    G4double stepLen = step->GetStepLength(); // Altro metodo di G4Step
    G4LogicalVolume* vol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume(); // Qui è più complesso: prendo il punto prima dello step, poi prendo il volume toccato, e da lì prendo il volume logico (per confrontarlo con i nostri volumi logici del nucleo e della cellula)

    // 3. Aggiungi sempre al totale
    if (vol == fLogicNucleus || vol == fLogicCell) { // logica più sicura: aggiungi al totale solo se sei nel nucleo o nella cellula
        fEventAction->AddAugerTotalLength(stepLen);
    }

    // 4. Aggiungi al citoplasma SOLO se il volume è quello del citoplasma
    if (vol == fLogicCell) {
        fEventAction->AddAugerCytoLength(stepLen);
    }
}

