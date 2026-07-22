#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

  private:
    G4double fHalfWorldSize;
    G4double fNuclRadius;
    G4double fCytoThickness;
    G4double fpMaxStepSize;
    G4double fpTrackingCut;
};

#endif