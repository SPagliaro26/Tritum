#include "TritiumSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4Track.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

TritiumSD::TritiumSD(const G4String& name)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr),
   collID(-1)
{
 // fSourceEnergy = 0.0;  // in constructor
  collectionName.insert("TritiumColl");  // nome interno della collezione :contentReference[oaicite:2]{index=2}
}

void TritiumSD::Initialize(G4HCofThisEvent* hce) {
  fHitsCollection = new TritiumHitsCollection(SensitiveDetectorName, collectionName[0]);
  if (collID < 0) {
   // collID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection->GetName());
    collID = G4SDManager::GetSDMpointer()->GetCollectionID(SensitiveDetectorName + "/" + collectionName[0]);
  }
  hce->AddHitsCollection(collID, fHitsCollection);
  tritIDs.clear();
}


G4bool TritiumSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4Track* track = step->GetTrack();
    G4int trackID = track->GetTrackID();
    G4double edep = step->GetTotalEnergyDeposit();
    G4int PID = track->GetDefinition()->GetPDGEncoding();

    if (PID != 1000010030) {  return false;} // Process only tritium tracks, for debugging purposes
      

    if (edep > 0.) {
        fEnergyDepositPerTrack[trackID] += edep;
    }
    G4cout << "Track " << trackID << " deposited " << edep << " MeV in this step." << G4endl;
    // Create hit only when track is killed
    if (track->GetTrackStatus() == G4TrackStatus::fStopAndKill) {

        // Only create a hit if the track deposited energy
        auto depositIt = fEnergyDepositPerTrack.find(trackID);
        if (depositIt == fEnergyDepositPerTrack.end()) {
            // No energy deposited - skip
            return true;
        }

        const auto* PGA = static_cast<const B1::PrimaryGeneratorAction*>(
            G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
        const auto* SmokingGun = PGA->GetGeneralParticleSource();
        G4double SourceEnergy = SmokingGun ? SmokingGun->GetParticleEnergy() : 0.0;


        auto* hit = new TritiumHit();
        hit->SetTrackID(trackID);
        hit->SetPDG(track->GetDefinition()->GetPDGEncoding());
        hit->SetKinEnergy(track->GetVertexKineticEnergy());
        hit->SetEventID(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        hit->SetSourceEnergy(SourceEnergy);
        hit->SetTotalDeposit(depositIt->second);

        fHitsCollection->insert(hit);

        // Remove the track's accumulated energy to save memory
        fEnergyDepositPerTrack.erase(depositIt);
    }

    return true;
}
void TritiumSD::EndOfEvent(G4HCofThisEvent*) {
  //}
}
