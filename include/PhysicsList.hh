#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"

class PhysicsListMessenger;

class PhysicsList : public G4VModularPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList() override;

    void ConstructParticle() override;
    void ConstructProcess() override;

    void AddPhysicsList(const G4String& name);
    void AddTrackingCut();

  private:
    G4String fEmName;
    G4VPhysicsConstructor* fEmPhysicsList;
    G4VPhysicsConstructor* fRadDecay;
    PhysicsListMessenger* fMessenger;
};

#endif
