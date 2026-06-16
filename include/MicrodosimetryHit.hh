#ifndef MicrodosimetryHit_h
#define MicrodosimetryHit_h 1

#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"


class MicrodosimetryHit : public G4VHit
{
    public:
        MicrodosimetryHit();
        MicrodosimetryHit(const MicrodosimetryHit&);
        ~MicrodosimetryHit() override;

        // Operators
        const MicrodosimetryHit& operator=(const MicrodosimetryHit&);
        G4bool operator==(const MicrodosimetryHit&) const;

        // inline void* operator new(size_t);
        // inline void operator delete(void*);

        // Metodi dalla classe base

        void Draw() override;
        void Print() override;

        // Metodi locali tipo Set
        inline void SetTrackID(const G4int& track) { fTrackID = track; };
        inline void SetEdep(const G4double& de) { fEdep = de; };
        inline void SetPos(const G4ThreeVector& xyz) { fPos = xyz; };
        inline void SetIncidentEnergy(const G4double& ie) { fIncidentEnergy = ie; };

        // Metodi locali tipo Get
        inline G4int GetTrackID() const { return fTrackID; };
        inline G4double GetEdep() const { return fEdep; };
        inline G4ThreeVector GetPos() const { return fPos; };
        inline G4double GetIncidentEnergy() const { return fIncidentEnergy; };

    private:
        G4int fTrackID = -1;
        G4double fEdep = 0;
        G4ThreeVector fPos = G4ThreeVector();
        G4double fIncidentEnergy = 0.; 
};

typedef G4THitsCollection<MicrodosimetryHit> MicrodosimetryHitsCollection;

#endif