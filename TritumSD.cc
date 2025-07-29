#include "TritumSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4AnalysisManager.hh"

TritiumSD::TritiumSD(const G4String& name)
 : G4VSensitiveDetector(name),
   hitCollection(nullptr),
   collID(-1)
{
  collectionName.insert("TritiumColl");  // nome interno della collezione :contentReference[oaicite:2]{index=2}
}

void TritiumSD::Initialize(G4HCofThisEvent* hce) {
  hitCollection = new TritiumHitsCollection(SensitiveDetectorName, collectionName[0]);
  if (collID < 0) {
    collID = G4SDManager::GetSDMpointer()->GetCollectionID(hitCollection->GetName());
  }
  hce->AddHitsCollection(collID, hitCollection);
  tritIDs.clear();
}

G4bool TritiumSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
  G4Track* trk = step->GetTrack();
  G4int pdgID =
  step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  //auto analysisManager = G4AnalysisManager::Instance();
  // analysisManager->FillNtupleDColumn(0, edep);   // column 0: Edep
  //analysisManager->FillNtupleIColumn(0, pdgID);  // column 1: PDG ID
  //analysisManager->AddNtupleRow(0);
  // Se il particleName è "triton", memorizza il trackID :contentReference[oaicite:3]{index=3}
  if (trk->GetDefinition()->GetParticleName() == "triton") {
    tritIDs.insert(trk->GetTrackID());
  }
  return true;  // raccomandato per Sensitive Detectors :contentReference[oaicite:4]{index=4}
}

void TritiumSD::EndOfEvent(G4HCofThisEvent*) {
  auto* hit = new TritiumHit();
  hit->SetCount(static_cast<G4int>(tritIDs.size()));
  hitCollection->insert(hit);
  auto analysisManager = G4AnalysisManager::Instance();
  //for (auto tid : ) {
  analysisManager->FillNtupleIColumn(0, tritIDs.size() );
  analysisManager->AddNtupleRow(0);
  //}
}
