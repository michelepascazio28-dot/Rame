#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include <cfloat>
#include <memory>

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    // Getter necessari per il Sensitive Detector
    G4LogicalVolume* GetLogicNucleus() const { return fLogicNucleus; }
    G4LogicalVolume* GetLogicCell() const    { return fLogicCell; }

    void SetTrackingCut(G4double val)    { fpTrackingCut = val; }
    void SetMaxStepSize(G4double val)    { fpMaxStepSize = val; }
    void SetNucleusRadius(G4double val)  { fNucleusRadius = val; }
    void SetCellRadius(G4double val)     { fCellRadius = val; }

  private:
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    // std::unique_ptr<DetectorMessenger> fDetectorMessenger;

    // Volumi Logici (fondamentali per assegnare il SD)
    G4LogicalVolume* fLogicCell = nullptr;
    G4LogicalVolume* fLogicNucleus = nullptr;

    // Variabili
    G4double fpTrackingCut;
    G4double fpMaxStepSize = DBL_MAX;
    G4double fNucleusRadius;
    G4double fCellRadius;
    
    // Materiali
    G4Material* fpWaterMaterial = nullptr;
    G4Material* fpCytoplasmMaterial = nullptr;
    G4Material* fpVacuumMaterial = nullptr;
};

#endif