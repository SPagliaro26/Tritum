#include "TritiumHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

// Allocatore globale (obbligatorio per G4)
G4ThreadLocal G4Allocator<TritiumHit>* TritiumHitAllocator = nullptr;

TritiumHit::TritiumHit()
  : fTrackID(-1), fEventID(-1), fPDG(0), fKinEnergy(0.), fEdep(0.) {}

TritiumHit::~TritiumHit() {}

void* TritiumHit::operator new(size_t)
{
  if (!TritiumHitAllocator)
    TritiumHitAllocator = new G4Allocator<TritiumHit>;
  return TritiumHitAllocator->MallocSingle();
}

void TritiumHit::operator delete(void* hit)
{
  if (TritiumHitAllocator)
    TritiumHitAllocator->FreeSingle((TritiumHit*)hit);
}

