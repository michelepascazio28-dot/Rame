#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include <memory>

// class PhysicsListMessenger; // Dichiarazione anticipata (Forward declaration)

class PhysicsList : public G4VModularPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList() override;
    
    
    void ConstructParticle() override;
    void AddPhysicsList(const G4String& name);
    void ConstructProcess() override;
    void AddTrackingCut();
    void AddMaxStepSize();

    virtual void SetCuts();

  private:
    G4String fEmName = "";
    std::unique_ptr<G4VPhysicsConstructor> fEmPhysicsList;
    // std::unique_ptr<PhysicsListMessenger> fMessenger;
};

#endif