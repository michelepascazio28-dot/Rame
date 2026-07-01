//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software
// shall cite the following Geant4-DNA collaboration publications:
// Med. Phys. 45 (2018) e722-e739
// Phys. Med. 31 (2015) 861-874
// Med. Phys. 37 (2010) 4692-4708
// Int. J. Model. Simul. Sci. Comput. 1 (2010) 157–178
//
// The Geant4-DNA web site is available at http://geant4-dna.org
//

#include "DetectorConstruction.hh"

#include "DetectorMessenger.hh"

#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "G4Sphere.hh"
#include "G4UnitsTable.hh"
#include "G4UserLimits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction()
{
  // Default tracking cut

  fTrackingCut = 7.4 * CLHEP::eV; //CLHEP sta per "Class Library for High Energy Physics", ed è una libreria di unità di misura e costanti fisiche usata in Geant4. 
  // Parametri standard della cellula

  fWorldRadius = 10 * CLHEP::m;
  fCytoThickness = 10 * CLHEP::nm;
  fNuclRadius = 3 * CLHEP::nm;
  
  // Qui scrivo DefineMaterials() perché è necessario definire i materiali prima di costruire il rivelatore, 
  // altrimenti non sarebbe possibile costruire il rivelatore usando quei materiali.
  DefineMaterials();

  // Creo il messagger del rivelatore, che è un oggetto che permette di modificare i parametri del rivelatore da macro,
  // e quindi a runtime, senza dover ricompilare il codice.

  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Qui specifico i materiali che voglio usare per costruire il rivelatore, usando il G4NistManager,
// che è un manager dei materiali predefiniti in Geant4.
void DetectorConstruction::DefineMaterials()
{

  G4NistManager* nist = G4NistManager::Instance();

  fWaterMaterial = nist->FindOrBuildMaterial("G4_WATER");
  
  G4double CytoDensity = 1.05 * CLHEP::g/CLHEP::cm3;
  fCytoMaterial = fNuclMaterial = new G4Material("Cytoplasm", CytoDensity, fWaterMaterial);

  fWorldMaterial = nist->FindOrBuildMaterial("G4_Galactic");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  if (fWorld) return fWorld;

  // Spherical world
  // G4Sphere necessita dei seguenti parametri: nome, raggio interno, raggio esterno, angolo phi iniziale, angolo phi finale, angolo theta iniziale, angolo theta finale.
  // Il theta deve andare dal polo nord al polo sud (da 0 a pi), dopodiché il phi deve andare tutto intorno alla sfera (da 0 a 2 pi).
  G4Sphere* sWorld = new G4Sphere("World", 0., 1000 * fNuclRadius, 0., twopi, 0., pi);

  fLogicalWorld = new G4LogicalVolume(sWorld, fWorldMaterial, "World");

  fWorld = new G4PVPlacement(0, G4ThreeVector(), fLogicalWorld, "World", 0, false, 0);

  // Nucleo

  G4Sphere* sNucl = new G4Sphere("Nucl", 0., fNuclRadius, 0., twopi, 0., pi);

  fLogicalNucl = new G4LogicalVolume(sNucl, fNuclMaterial, "Nucl");

  fNucl = new G4PVPlacement(0, G4ThreeVector(), "Nucl", fLogicalNucl, fWorld, false, 0);

  // Citoplasma sferico

  G4Sphere* sCyto =
    new G4Sphere("Cyto", fNuclRadius, fNuclRadius + fCytoThickness, 0., twopi, 0., pi);

  fLogicalCyto = new G4LogicalVolume(sCyto, fCytoMaterial, "Cyto");

  fCyto = new G4PVPlacement(0, G4ThreeVector(), "Cyto", fLogicalCyto, fWorld, false, 0);
  //
  // Con questa istruzione, stampo i parametri del rivelatore, come il tracking cut,
  // le dimensioni e i materiali dei volumi del rivelatore, usando il metodo PrintParameters(),
  PrintParameters();

  // Tracking cut
  // Qui imposto il tracking cut per tutti i volumi del rivelatore, usando il metodo SetUserLimits() della classe G4LogicalVolume,
  // che permette di impostare dei limiti per il tracking dei particelle in quel volume, cioè di impostare un tracking cut, 
  // che è una soglia di energia al di sotto della quale le particelle non vengono più tracciate, 
  // e quindi non vengono più considerate per il calcolo delle interazioni con il materiale del rivelatore.
  // In questo caso, imposto un tracking cut di fTrackingCut per tutti i volumi del rivelatore, 
  // che è stato definito come variabile membro della classe, e che è stato inizializzato nel costruttore della classe.
  // DBL_MAX è una costante che rappresenta il massimo valore di un double,
  // e viene usata qui per indicare che non ci sono limiti per il passo massimo, la lunghezza massima del track,
  // e il tempo massimo, mentre fTrackingCut è pari a 7.4 eV, ed è il limite per l'energia cinetica minima delle particelle che vengono tracciate.

  fLogicalNucl->SetUserLimits(new G4UserLimits(DBL_MAX, DBL_MAX, DBL_MAX, fTrackingCut));
  fLogicalCyto->SetUserLimits(new G4UserLimits(DBL_MAX, DBL_MAX, DBL_MAX, fTrackingCut));
  fLogicalWorld->SetUserLimits(new G4UserLimits(DBL_MAX, DBL_MAX, DBL_MAX, fTrackingCut));

  //
  return fWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Qui definisco PrintParameters(). Servirà per stampare i parametri del rivelatore direttamente da macro. 
void DetectorConstruction::PrintParameters() const
{
  G4cout << "\n---------------------------------------------------------\n";
  G4cout << "---> The tracking cut is set to " << G4BestUnit(fTrackingCut, "Energy") << G4endl;
  G4cout << "---> The World is a sphere of " << G4BestUnit(1000 * fNuclRadius, "Length")
         << "radius of " << fWorldMaterial->GetName() << G4endl;
  G4cout << "---> The Nucleus is a sphere of " << G4BestUnit(fNuclRadius, "Length") << "radius of "
         << fWorldMaterial->GetName() << " of mass " << G4BestUnit(GetNuclMass(), "Mass") << G4endl;
  G4cout << "---> The Cytoplasm is a spherical shell of thickness "
         << G4BestUnit(fCytoThickness, "Length") << "of " << fWorldMaterial->GetName()
         << " of mass " << G4BestUnit(GetCytoMass(), "Mass") << G4endl;
  G4cout << "\n---------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Qui c'è una carellata di setter per i parametri del rivelatore.
void DetectorConstruction::SetTrackingCut(G4double value)
{
  fTrackingCut = value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetNuclRadius(G4double value)
{
  fNuclRadius = value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCytoThickness(G4double value)
{
  fCytoThickness = value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldMaterial(const G4String& materialChoice)
{
  // Search the material by its name
  G4Material* pttoMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial && pttoMaterial != fWorldMaterial) {
    fWorldMaterial = pttoMaterial;
    if (fLogicalWorld) fLogicalWorld->SetMaterial(pttoMaterial);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();  // Questa istruzione è necessaria per informare il run manager che la fisica del rivelatore è stata modificata,
                                                             // e quindi che è necessario aggiornare le tabelle di cross section e di energia,
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCytoMaterial(const G4String& materialChoice)
{
  // Se noti pttoMaterial sta per "pointer to material", ed è una variabile che punta al materiale 
  // trovato dal G4NistManager usando il nome del materiale passato come argomento alla funzione SetCytoMaterial().
  G4Material* pttoMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial && pttoMaterial != fCytoMaterial) {
    fCytoMaterial = pttoMaterial;
    if (fLogicalCyto) fLogicalCyto->SetMaterial(pttoMaterial);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetNuclMaterial(const G4String& materialChoice)
{
  // Search the material by its name
  G4Material* pttoMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial && pttoMaterial != fNuclMaterial) {
    fNuclMaterial = pttoMaterial;
    if (fLogicalNucl) fLogicalNucl->SetMaterial(pttoMaterial);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  }
}
