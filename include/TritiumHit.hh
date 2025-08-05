#ifndef TRITIUMHIT_H
#define TRITIUMHIT_H

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

// TritiumHit.hh
class TritiumHit : public G4VHit {
public:
    TritiumHit();
    virtual ~TritiumHit();

    void* operator new(size_t);
    void  operator delete(void*);

    void SetTrackID(G4int id)           { fTrackID = id; }
    void SetEventID(G4int id)           { fEventID = id; }
    void SetPDG(G4int pdg)              { fPDG = pdg; }
    void SetKinEnergy(G4double energy)  { fKinEnergy = energy; }
    void AddEdep(G4double edep)         { fEdep += edep; }

    G4int GetTrackID()      const { return fTrackID; }
    G4int GetEventID()      const { return fEventID; }
    G4int GetPDG()          const { return fPDG; }
    G4double GetKinEnergy() const { return fKinEnergy; }
    G4double GetEdep()      const { return fEdep; }
    static G4ThreadLocal G4Allocator<TritiumHit>* TritiumHitAllocator;

private:
    G4int fTrackID = -1;
    G4int fEventID = -1;
    G4int fPDG = 0;
    G4double fKinEnergy = 0.;
    G4double fEdep = 0.;
};

#endif // TRITIUMHIT_H
