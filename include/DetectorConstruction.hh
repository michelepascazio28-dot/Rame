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
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

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

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4LogicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"

class DetectorMessenger;
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;
    // I seguenti metodi sono stati aggiunti per permettere di modificare
    // i parametri del rivelatore da macro, e quindi a runtime, senza dover ricompilare il codice.
    // Ancora non sono stati implementati, ma l'idea è che ognuno di loro sia un setter per uno dei parametri del rivelatore,
    // e permetta di modificare quel parametro da macro. Per esempio SetCytoThickness() permetterà di modificare
    // lo spessore del citoplasma da macro definito come fCytoThickness, e così via per gli altri setter.
    void SetTrackingCut(G4double);

    void SetCytoThickness(G4double);
    void SetCytoMaterial(const G4String&);

    void SetNuclRadius(G4double);
    void SetNuclMaterial(const G4String&);

    void SetWorldMaterial(const G4String&);

    G4VPhysicalVolume* Construct() override;
    // I seguenti metodi sono stati aggiunti per permettere di accedere ai parametri del rivelatore da macro,
    // e quindi a runtime, senza dover ricompilare il codice. Cioè ognuno di loro è un getter per uno dei parametri del rivelatore,
    // e permette di accedere a quel parametro da macro. Per esempio GetCytoThickness() permette di accedere allo
    // spessore del citoplasma da macro definito come fCytoThickness, e così via per gli altri getter.
    inline G4double GetCytoThickness() const { return fCytoThickness; }

    inline G4Material* GetCytoMaterial() const { return fCytoMaterial; }
    // Addirittura è possibile accedere alla massa del citoplasma da macro, usando GetCytoMass(),
    // che restituisce la massa del citoplasma calcolata usando il metodo GetMass() della classe G4LogicalVolume,
    // che a sua volta restituisce la massa del volume logico del citoplasma, che è stato definito come fLogicalCyto. 
    inline G4double GetCytoMass() const { return fLogicalCyto->GetMass(); }

    inline G4double GetNuclRadius() const { return fNuclRadius; }

    inline G4Material* GetNuclMaterial() const { return fNuclMaterial; }

    inline G4double GetNuclMass() const { return fLogicalNucl->GetMass(); }

    const G4LogicalVolume* GetNuclLogicalVolume() const { return fLogicalNucl; }

    const G4LogicalVolume* GetCytoLogicalVolume() const { return fLogicalCyto; }

    void PrintParameters() const;

  private:
    void DefineMaterials();
    // Qui ritroviamo i parametri del rivelatore, che sono stati definiti come variabili membro della classe,
    // e che sono stati inizializzati nel costruttore della classe.
    G4Material* fNuclMaterial = nullptr;
    G4Material* fCytoMaterial = nullptr;
    G4Material* fWorldMaterial = nullptr;
    G4Material* fWaterMaterial = nullptr;
    
    G4VPhysicalVolume* fNucl = nullptr;
    G4VPhysicalVolume* fCyto = nullptr;
    G4VPhysicalVolume* fWorld = nullptr;

    G4LogicalVolume* fLogicalNucl = nullptr;
    G4LogicalVolume* fLogicalCyto = nullptr;
    G4LogicalVolume* fLogicalWorld = nullptr;

    DetectorMessenger* fDetectorMessenger = nullptr;

    G4double fTrackingCut;
    G4double fNuclRadius;
    G4double fCytoThickness;
    G4double fWorldRadius;
};

#endif
