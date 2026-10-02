#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{
  fWorldSize = 20 * mm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //
  // define a material
  //
  G4Material* VacuumMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  //
  // World
  //
  G4Box* solidWorld = new G4Box("World",  // its name
                                fWorldSize / 2, fWorldSize / 2, fWorldSize / 2);  // its size

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                                    VacuumMaterial,  // its material
                                                    "World");  // its name
  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,  // no rotation
                                                    G4ThreeVector(),  // at (0,0,0)
                                                    logicWorld,  // its logical volume
                                                    "World",  // its name
                                                    0,  // its mother  volume
                                                    false,  // no boolean operation
                                                    0);  // copy number

  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
