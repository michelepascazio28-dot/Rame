#include "EventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh"

EventAction::EventAction(const DetectorConstruction* det)
 : G4UserEventAction(), 
   fNucleusEdep(0.),
   fCytoplasmEdep(0.),
   fAugerLengthTotal(0.),
   fAugerLengthCyto(0.),
   fDet(det)
{}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fNucleusEdep = 0.;
    fCytoplasmEdep = 0.;
    fAugerLengthTotal = 0.;
    fAugerLengthCyto = 0.;
}

void EventAction::EndOfEventAction(const G4Event*)
{
    // Calcoli
    G4double nucChord = fDet->GetNucleusChord();
    G4double nucMass  = fDet->GetNucleusMass();
    G4double cytoMass = fDet->GetCytoplasmMass();
    
    // Se non c'è deposito, Y e Z sono 0
    G4double yNuc = (fNucleusEdep > 0.) ? (fNucleusEdep / keV) / (nucChord / um) : 0.;
    G4double zNuc = (fNucleusEdep > 0.) ? (fNucleusEdep / joule) / (nucMass / kg) * 1000.0 : 0.;
    
    // G4double effectiveChord = (fAugerLengthCyto > 0.) ? fAugerLengthCyto : fDet->GetCytoplasmChord();
    // G4double yCyto = (fCytoplasmEdep > 0.) ? (fCytoplasmEdep / keV) / (effectiveChord / um) : 0.;
    // G4double zCyto = (fCytoplasmEdep > 0.) ? (fCytoplasmEdep / joule) / (cytoMass / kg) * 1000.0 : 0.;

    G4double meanChordCyto = fDet->GetCytoplasmChord();

    G4double yCyto = (fCytoplasmEdep > 0.) ? (fCytoplasmEdep / keV) / (meanChordCyto / um) : 0.;
    G4double zCyto = (fCytoplasmEdep > 0.) ? (fCytoplasmEdep / joule) / (cytoMass / kg) * 1000.0 : 0.;


    G4double totalEdep = fNucleusEdep + fCytoplasmEdep;
    // Supponendo che il Rame-64 rilasci circa 8.3 keV (energia Auger tipica)
    // if (totalEdep > 10.0 * keV) { 
    //    G4cout << "ATTENZIONE: Evento anomalo! Edep Totale: " << totalEdep/keV << " keV" << G4endl;
    // }

    // G4cout << "Debug EndOfEvent: EdepCyto=" << fCytoplasmEdep/keV << " keV, " << "TrackLength=" << fTrackLengthCyto/um << " um" << G4endl;

    // Riempimento Ntuple (Assicurati che l'ordine sia lo stesso creato in RunAction!)
    auto man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, fNucleusEdep/keV);
    man->FillNtupleDColumn(1, yNuc);
    man->FillNtupleDColumn(2, zNuc);
    man->FillNtupleDColumn(3, fCytoplasmEdep/keV);
    man->FillNtupleDColumn(4, yCyto);
    man->FillNtupleDColumn(5, zCyto);
    man->FillNtupleDColumn(6, fAugerLengthTotal/um);
    man->FillNtupleDColumn(7, fAugerLengthCyto/um);
    man->AddNtupleRow();
}