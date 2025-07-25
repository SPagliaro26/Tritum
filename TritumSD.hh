#ifndef TRITIUMSD_H
#define TRITIUMSD_H

#include "G4VSensitiveDetector.hh"
#include "TritumHit.hh"
#include <set>

class TritiumSD : public G4VSensitiveDetector {
public:
  explicit TritiumSD(const G4String& name);
  virtual ~TritiumSD() {}

  // Inizializza la hit collection per ogni evento
  virtual void Initialize(G4HCofThisEvent* hce) override;

  // Chiamato per ogni passo nel volume sensibile
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* hist) override;

  // Alla fine di ogni evento salva il conteggio dei trizi
  virtual void EndOfEvent(G4HCofThisEvent* hce) override;

private:
  TritiumHitsCollection* hitCollection;
  G4int                  collID;
  std::set<G4int>        tritIDs;  // tiene univoci i trackID dei tritoni :contentReference[oaicite:1]{index=1}
};

#endif // TRITIUMSD_H