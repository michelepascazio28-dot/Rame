#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include <memory>

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4Region;
class MicrodosimetrySD;
class EventAction;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;


    void SetTrackingCut(G4double val);
    void SetMaxStepSize(G4double val);

    G4LogicalVolume* GetLogicNucleus() const { return fLogicNucleus; }
    G4LogicalVolume* GetLogicCell() const { return fLogicCell; }

    void UpdateGeometryParameters(); // Metodo per calcolare massa/corda
    G4double GetNucleusMass() const;
    G4double GetNucleusChord() const;

    MicrodosimetrySD* GetMicroSD() const { return fMicroSD; }

    G4double GetCytoplasmChord() const;
    G4double GetCytoplasmMass() const;

    void SetEventAction(EventAction* ea) { fEventAction = ea; }
    
  private:
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    // Materiali
    G4Material* fpCytoplasmMaterial{nullptr};
    G4Material* fpWaterMaterial{nullptr};

    // Volumi Logici
    G4LogicalVolume* fLogicWorld{nullptr};
    G4LogicalVolume* fLogicCell{nullptr};
    G4LogicalVolume* fLogicNucleus{nullptr};
    G4Region* fDNARegion{nullptr};

    // Parametri e Variabili
    G4double fWorldSize;
    G4double fCellRadius;
    G4double fNucleusRadius;
    G4double fNucleusMass;
    G4double fNucleusChordLength;
    
    G4double fpTrackingCut;
    G4double fpMaxStepSize;

    // Sensore
    MicrodosimetrySD* fMicroSD{nullptr};

    EventAction* fEventAction = nullptr;
};

#endif