#include "TritumSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

TritiumSD::TritiumSD(const G4String& name)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr),
   collID(-1)
{
  collectionName.insert("TritiumColl");  // nome interno della collezione :contentReference[oaicite:2]{index=2}
}

void TritiumSD::Initialize(G4HCofThisEvent* hce) {
  fHitsCollection = new TritiumHitsCollection(SensitiveDetectorName, collectionName[0]);
  if (collID < 0) {
    collID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection->GetName());
  }
  hce->AddHitsCollection(collID, fHitsCollection);
  tritIDs.clear();
}

G4bool TritiumSD::ProcessHits(G4Step* step, G4TouchableHistory*)

{
    const auto* secondaries = step->GetSecondaryInCurrentStep();
    if (!secondaries || secondaries->empty()) return false;

    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    for (const auto* sec : *secondaries) {
        if (!sec || !sec->GetCreatorProcess()) continue;

        auto* hit = new TritiumHit();
        hit->SetTrackID(sec->GetTrackID());
        hit->SetPDG(sec->GetParticleDefinition()->GetPDGEncoding());
        hit->SetKinEnergy(sec->GetKineticEnergy());
        hit->SetEventID(eventID);

        fHitsCollection->insert(hit);
    }

    return true;

}

void TritiumSD::EndOfEvent(G4HCofThisEvent*) {
  //}
}
