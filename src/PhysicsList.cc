#include "PhysicsList.hh"
#include "G4EmDNAPhysics_option4.hh" // Fisica specifica DNA
#include "G4EmStandardPhysics_option4.hh" // Per processi elettromagnetici generali (inclusi quelli di rilassamento)
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmParameters.hh"          // Necessario per configurare Auger
#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
    SetVerboseLevel(0);

    // 1. CONFIGURAZIONE PARAMETRI PRIMA DELLA REGISTRAZIONE
    // È vitale farlo prima di RegisterPhysics!
    G4EmParameters* param = G4EmParameters::Instance();
    param->SetAuger(true);                // Explicitly ON
    param->SetAugerCascade(true);         // Cascata Auger ON
    param->SetFluo(true);                 // Anche la fluorescenza aiuta il rilassamento
    param->SetDeexcitationIgnoreCut(true); // Fondamentale: ignora i cut per l'emissione atomica

    // 2. Registrazione Fisica
    // Nota: G4EmDNAPhysics_option4 è ottima, ma assicurati che la tua geometria 
    // sia definita come "G4_WATER" o materiale compatibile.
    RegisterPhysics(new G4EmDNAPhysics_option4());
    RegisterPhysics(new G4EmStandardPhysics_option4()); // Per processi elettromagnetici generali (inclusi quelli di rilassamento)

    // 3. Fisica del decadimento
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList() = default;

void PhysicsList::ConstructParticle()
{
    G4VModularPhysicsList::ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();
}

void PhysicsList::SetCuts()
{
    G4double cutValue = 10* um; 
    SetCutsWithDefault();
    SetCutValue(cutValue, "e-");
    SetCutValue(cutValue, "e+");
    SetCutValue(cutValue, "gamma");
}