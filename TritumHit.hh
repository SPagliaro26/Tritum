#ifndef TRITIUMHIT_H
#define TRITIUMHIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class TritiumHit : public G4VHit {
public:
  TritiumHit() : G4VHit(), nTrit(0) {}
  virtual ~TritiumHit() {}

  // Operatori di allocazione custom per performance
  inline void* operator new(size_t);
  inline void  operator delete(void*);

  // Copia e assegnazione
  TritiumHit(const TritiumHit& right) : G4VHit(), nTrit(right.nTrit) {}
  const TritiumHit& operator=(const TritiumHit& right) {
    nTrit = right.nTrit; return *this;
  }

  // Imposta e legge il conteggio di trizi
  void    SetCount(G4int n) { nTrit = n; }
  G4int   GetCount() const   { return nTrit; }

  // Stampa su console (opzionale)
  virtual void Print() override {
    G4cout << "  [TritiumHit] nTrit = " << nTrit << G4endl;
  }

private:
  G4int nTrit;
};

// Definizione di collettore di hit
using TritiumHitsCollection = G4THitsCollection<TritiumHit>;

// Allocator: aiuta Geant4 a gestire efficientemente la memoria delle hit :contentReference[oaicite:0]{index=0}
extern G4Allocator<TritiumHit> TritiumHitAllocator;

inline void* TritiumHit::operator new(size_t) {
  return (void*) TritiumHitAllocator.MallocSingle();
}
inline void  TritiumHit::operator delete(void* hit) {
  TritiumHitAllocator.FreeSingle((TritiumHit*) hit);
}

#endif // TRITIUMHIT_H
