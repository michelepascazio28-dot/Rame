#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

#include <CLHEP/Units/SystemOfUnits.h>
#include <memory>

class G4Material;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    void SetTrackingCut(const G4double&);
    void SetMaxStepSize(const G4double&);
    void PrintParameters() const;
    void SetTrackerSDRadius(const G4double&);

    void SetNuclRadius(const G4double&);
    void SetCytoThickness(const G4double&);
    void SetHalfWorldSize(const G4double&);
    void SetWorldMaterial(const G4String&);
    void SetCytoMaterial(const G4String&);
    // Getters
    G4double GetNuclRadius() const { return fNuclRadius; }
    G4double GetCytoThickness() const { return fCytoThickness; }
    G4double GetHalfWorldSize() const { return fHalfWorldSize; }

  private:
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
    std::unique_ptr<DetectorMessenger> fDetectorMessenger;

    G4Material* fpWaterMaterial = nullptr;
    G4double fpTrackingCut = 11. * CLHEP::eV;
    G4double fpMaxStepSize = DBL_MAX;
    G4double fRadius = 0.;
    G4double fNuclRadius = 0.;
    G4double fCytoThickness = 0.;
    G4double fWorldRadius = 0.;
    G4double fHalfWorldSize = 0.;
};

#endif
