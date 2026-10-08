#ifndef DomainHit_h
#define DomainHit_h 1

// Hit "fittizio" usato SOLO per visualizzare il dominio sferico di un evento:
//  - sfera verde a fili: il dominio (centro = randCenterPos, raggio = R_d)
//  - punto nero grande: la hit scelta a caso
//  - punti rossi: hit DENTRO il dominio (contano in n_scored)
//  - punti grigi: hit del nucleo FUORI dal dominio (contano solo in n_hits)
// Non contiene energia e non entra in nessun calcolo.

#include <utility>
#include <vector>

#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"

class DomainHit : public G4VHit
{
 public:
  DomainHit(const G4ThreeVector& center, G4double radius, const G4ThreeVector& hitPos,
            std::vector<G4ThreeVector> allHits)
    : fCenter(center), fRadius(radius), fHitPos(hitPos), fAllHits(std::move(allHits))
  {}
  ~DomainHit() override = default;

  void Draw() override
  {
    G4VVisManager* vis = G4VVisManager::GetConcreteInstance();
    if (!vis) return;

    // Dominio: sfera a fili verde
    G4Sphere solid("Domain", 0., fRadius, 0., twopi, 0., pi);
    G4VisAttributes att(G4Colour(0., 0.7, 0., 1.));
    att.SetForceWireframe(true);
    vis->Draw(solid, att, G4Translate3D(fCenter));

    // Tutte le hit del nucleo: rosse se dentro il dominio, grigie se fuori
    const G4double r2 = fRadius * fRadius;
    for (const auto& p : fAllHits) {
      const G4bool inside = (p - fCenter).mag2() <= r2;
      G4Circle c(p);
      c.SetFillStyle(G4Circle::filled);
      if (inside) {
        c.SetScreenSize(5.);
        c.SetVisAttributes(G4VisAttributes(G4Colour(1., 0., 0.)));
      }
      else {
        c.SetScreenSize(4.);
        c.SetVisAttributes(G4VisAttributes(G4Colour(0.6, 0.6, 0.6)));
      }
      vis->Draw(c);
    }

    // Hit scelta a caso: punto nero piu grande
    G4Circle marker(fHitPos);
    marker.SetScreenSize(11.);
    marker.SetFillStyle(G4Circle::filled);
    marker.SetVisAttributes(G4VisAttributes(G4Colour(0., 0., 0.)));
    vis->Draw(marker);
  }

 private:
  G4ThreeVector fCenter;
  G4double fRadius;
  G4ThreeVector fHitPos;
  std::vector<G4ThreeVector> fAllHits;
};

using DomainHitsCollection = G4THitsCollection<DomainHit>;

#endif
