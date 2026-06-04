#ifndef ActionInitialization_hh
#define ActionInitialization_hh 1

#include "G4VUserActionInitialization.hh"

class DetectorConstruction; // Forward declaration

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(DetectorConstruction* det);
    virtual ~ActionInitialization();

    virtual void Build() const override;
    virtual void BuildForMaster() const override;

  private:
    DetectorConstruction* fDet;
};

#endif