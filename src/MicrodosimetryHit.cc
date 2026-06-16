#include "MicrodosimetryHit.hh"

#include "G4Circle.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"

G4ThreadLocal G4Allocator<MicrodosimetryHit>* MicrodosimetryHitAllocator = nullptr;

MicrodosimetryHit::MicrodosimetryHit() : G4VHit() {}

MicrodosimetryHit::~MicrodosimetryHit() = default;

MicrodosimetryHit::MicrodosimetryHit(const MicrodosimetryHit& right) : G4VHit()
{
    fTrackID = right.fTrackID;
    fEdep = right.fEdep;
    fPos = right.fPos;
    fIncidentEnergy = right.fIncidentEnergy;
}

const MicrodosimetryHit& MicrodosimetryHit::operator=(const MicrodosimetryHit& right)
{
  fTrackID = right.fTrackID;
  fEdep = right.fEdep;
  fPos = right.fPos;
  fIncidentEnergy = right.fIncidentEnergy;

  return *this;
}

G4bool MicrodosimetryHit::operator==(const MicrodosimetryHit& right) const
{
  return this == &right;
}

void MicrodosimetryHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (pVVisManager) {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1., 0., 0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void MicrodosimetryHit::Print()
{
  G4cout << "  trackID: " << fTrackID << "Edep: " << std::setw(7) << G4BestUnit(fEdep, "Energy")
         << " Position: " << std::setw(7) << G4BestUnit(fPos, "Length")
         << "IncidentEnergy: " << std::setw(7) << G4BestUnit(fIncidentEnergy, "Energy") << G4endl;
}