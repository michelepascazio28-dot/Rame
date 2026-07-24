#include "CalorimeterSD_Nucl.hh"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

CalorimeterSD_Nucl::CalorimeterSD_Nucl(const G4String& name, G4int colID)
: G4VSensitiveDetector(name), fEdepSum(0.), fMass(0.), fcolID(colID) {}

// CalorimeterSD_Nucl::~CalorimeterSD_Nucl() {}

void CalorimeterSD_Nucl::Initialize(G4HCofThisEvent*)
{
    fEdepSum = 0.;
    fMass = 0.;
}

G4bool CalorimeterSD_Nucl::ProcessHits(G4Step* step, G4TouchableHistory*)
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

void CalorimeterSD_Nucl::EndOfEvent(G4HCofThisEvent*)
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