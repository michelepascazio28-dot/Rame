#include "MicrodosimetrySD.hh"

#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

MicrodosimetrySD::MicrodosimetrySD(const G4String& name, const G4String& hitsCollectionName) : G4VSensitiveDetector(name), fHitsCollection(nullptr)
{
    collectionName.insert(hitsCollectionName);
}

void MicrodosimetrySD::Initialize(G4HCofThisEvent* hce)
{
  // G4cout << ">>> RILEVATO STEP NEL NUCLEO!" << G4endl;
  // Creiamo una hits collection
  fHitsCollection = new MicrodosimetryHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Aggiungiamo questa collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

  hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool MicrodosimetrySD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    G4double edep = aStep->GetTotalEnergyDeposit();
    G4String particleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();

    // STAMPA TUTTO, anche se edep è 0
    /*
    G4cout << ">>> STEP: " << particleName 
           << " | Edep: " << edep/eV << " eV" 
           << " | Volume: " << aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() 
           << G4endl;

    if (edep <= 0.) return false;
    */

    auto newHit = new MicrodosimetryHit();
    newHit->SetEdep(edep);
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
    
    fHitsCollection->insert(newHit);
    return true;
}

void MicrodosimetrySD::EndOfEvent(G4HCofThisEvent*)
{
  if (!fHitsCollection) return;
    
  G4int nofHits = fHitsCollection->entries();
  if (nofHits == 0) return;

  G4double Einc = 0;

  /*
  G4cout << G4endl
  << "-------->Hits Collection: in this event they are "
  << nofHits
  << " hits in the target volume " << G4endl;
  */

  // PROCESSING OF MICRODOSIMETRY Y & Z SPECTRA

  // *************************************
  // Please select herebelow :
  // the radius of the target sphere:
  // variable name = radius
  // it is set to 5 nm by default)

  G4double radius = fMicroRadius;

  //

  //***************
  // y and z
  //***************

  // Select random hit
  G4int randHit = 0;  // Runs from 0 to number of hits - 1
  randHit = static_cast<G4int>(G4UniformRand() * nofHits);

  /*
  G4cout
  << "======> random selection of hit number randHit ="
  << randHit << G4endl;
  */

  // Get selected random hit position
  G4ThreeVector hitPos = (*fHitsCollection)[randHit]->GetPos();
  // G4cout << "======> random hit position x/nm =" << hitPos.x()/nm << G4endl;
  // G4cout << "======> random hit position y/nm =" << hitPos.y()/nm << G4endl;
  // G4cout << "======> random hit position z/nm =" << hitPos.z()/nm << G4endl;

  // Set random position of center of sphere within radius
  G4double chord = 4. * radius / 3;
  G4double density = 1 * g / cm3;
  G4double mass = (4. / 3) * CLHEP::pi * radius * radius * radius * density;

  // Random placement of sphere: method 1
  /*
  G4ThreeVector randDir = G4RandomDirection();
  G4double randRadius = G4UniformRand()*radius;
  G4ThreeVector randCenterPos = randRadius*randDir + hitPos;
  */

  // Random placement of sphere: method 2

  G4double xRand = 1.01 * radius;
  G4double yRand = 1.01 * radius;
  G4double zRand = 1.01 * radius;
  G4double randRad = 1.01 * radius;
  do {
    xRand = (2 * G4UniformRand() - 1) * radius;
    yRand = (2 * G4UniformRand() - 1) * radius;
    zRand = (2 * G4UniformRand() - 1) * radius;
    randRad = std::sqrt(xRand * xRand + yRand * yRand + zRand * zRand);
  } while (randRad > radius);

  G4ThreeVector randCenterPos(xRand + hitPos.x(), yRand + hitPos.y(), zRand + hitPos.z());

  // Search for neighbouring hits in the sphere and cumulate deposited energy
  //  in epsilon
  G4double epsilon = 0;
  G4int nbEdep = 0;

  for (G4int i = 0; i < nofHits; i++) {
    if ((*fHitsCollection)[i]->GetIncidentEnergy() > 0)
      Einc = (*fHitsCollection)[i]->GetIncidentEnergy();

    G4ThreeVector localPos = (*fHitsCollection)[i]->GetPos();

    // G4cout << i << " " << (*fHitsCollection)[i] << G4endl;
    // G4cout << i << " " << (*fHitsCollection)[i]->GetEdep()/eV << G4endl;

    if ((localPos.x() - randCenterPos.x()) * (localPos.x() - randCenterPos.x())
          + (localPos.y() - randCenterPos.y()) * (localPos.y() - randCenterPos.y())
          + (localPos.z() - randCenterPos.z()) * (localPos.z() - randCenterPos.z())
        <= radius * radius)

    {
      epsilon = epsilon + (*fHitsCollection)[i]->GetEdep();
      nbEdep = nbEdep + 1;
    }
  }

  // For testing only
  /*
  G4cout << "======> for hit number #" << randHit <<
  ", we collect "
  << nbEdep << " energy depositions in a sphere of radius "
  << radius/nm << " nm and mass "
  << mass/kg << " kg for a total of "
  << epsilon/eV << " eV or "
  << (epsilon/joule)/(mass/kg) << " Gy" << G4endl;
  G4cout << "-" << G4endl;
  */

  /*
  FILE* myFile;
  myFile=fopen("yz.txt","a");
  fprintf(myFile,"%e %e %e\n",radius/nm,(epsilon/eV)/(chord/nm),
   (epsilon/joule)/(mass/kg));
  fclose(myFile);
  */

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Fill ntuple including weighting
  analysisManager->FillNtupleDColumn(0, radius / nm);
  analysisManager->FillNtupleDColumn(2, nofHits);
  analysisManager->FillNtupleDColumn(3, nbEdep);
  analysisManager->FillNtupleDColumn(4, (epsilon / eV) / (chord / nm));
  analysisManager->FillNtupleDColumn(5, (epsilon / mass) / gray);
  analysisManager->FillNtupleDColumn(6, Einc / eV);
  analysisManager->AddNtupleRow();
}