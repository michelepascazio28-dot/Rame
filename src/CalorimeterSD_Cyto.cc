#include "CalorimeterSD_Cyto.hh"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

CalorimeterSD_Cyto::CalorimeterSD_Cyto(const G4String& name, G4int colID)
: G4VSensitiveDetector(name), fEdepSum(0.), fMass(0.), fcolID(colID) {}

// CalorimeterSD_Cyto::~CalorimeterSD_Cyto() {}

void CalorimeterSD_Cyto::Initialize(G4HCofThisEvent*)
{
    fEdepSum = 0.;
    fMass = 0.;
}

G4bool CalorimeterSD_Cyto::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;

    fEdepSum += edep;

    if (fMass == 0.) {
        G4LogicalVolume* vol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        if (vol) {
            fMass = vol->GetMass();
        }
    }

    return true;
}

void CalorimeterSD_Cyto::EndOfEvent(G4HCofThisEvent*)
{
    if (fMass > 0.) {
        G4double dose = fEdepSum / fMass;
        

        G4cout << "--> SD [" << GetName() << "] Dose: " << dose / gray 
               << " Gy (Edep: " << fEdepSum << ", Mass: " << fMass << ")" << G4endl;
               
        // Salvo la dose

        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

        analysisManager->FillNtupleDColumn(fcolID, dose / gray); // Colonna 0 per la dose
        // analysisManager->AddNtupleRow();
    }
}