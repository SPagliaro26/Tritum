//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "TritiumSD.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4VisAttributes.hh"

namespace B1
{
// 1) Costruttore
DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fScoringVolume(nullptr)
{}

// 2) Distruttore
DetectorConstruction::~DetectorConstruction()
{}

// 3) Getter per fScoringVolume
G4LogicalVolume* DetectorConstruction::GetScoringVolume() const
{
  return fScoringVolume;
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  G4double env_sizeXY = 20 * cm, env_sizeZ = 30 * cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_Galactic");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 1.2 * env_sizeXY;
  G4double world_sizeZ = 1.2 * env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  auto solidWorld =
    new G4Box("World",  // its name
              0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                        world_mat,  // its material
                                        "World");  // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
                                     G4ThreeVector(),  // at (0,0,0)
                                     logicWorld,  // its logical volume
                                     "World",  // its name
                                     nullptr,  // its mother  volume
                                     false,  // no boolean operation
                                     0,  // copy number
                                     checkOverlaps);  // overlaps checking

  //
  // Envelope
  //
  auto solidEnv = new G4Box("Envelope",  // its name
                            0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size

  auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
                                      env_mat,  // its material
                                      "Envelope");  // its name

  /*new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicEnv,  // its logical volume
                    "Envelope",  // its name
                    logicWorld,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking*/

  //


  //
  // Shape 2
  //
G4double z, a, fractionmass, density;
G4String name, symbol;
G4int ncomponents;

G4Isotope* Li6 = new G4Isotope(
    "Li6",          // nome dell’isotopo
    3,              // numero atomico Z = 3 (Litio)
    6,              // numero di massa A = 6
    6.01512*g/mole  // massa molare (in g/mol)
);
G4Isotope* Li7 = new G4Isotope(
    "Li7",          // nome dell’isotopo
    3,              // numero atomico Z = 3
    7,              // numero di massa A = 7
    7.01600*g/mole  // massa molare (in g/mol)
);

G4Element* Li = new G4Element (name="Lithium" , symbol = "Li",ncomponents = 2);
Li->AddIsotope (Li6 , fractionmass=90*perCent);
Li->AddIsotope (Li7 , fractionmass=10*perCent);

a = 9.01*g/mole;
G4Element* elBe  = new G4Element(name="Berillium"  ,symbol="Be" , z= 4., a);

a = 19.00*g/mole; 
G4Element* elF = new G4Element (name= "Fluorum" , symbol="F" , z= 9., a);

density= 1944*mg/cm3;
G4Material* FLiBe = new G4Material(name="FLiBe  ",density,ncomponents=3);
FLiBe->AddElement(Li, fractionmass=18*perCent);
FLiBe->AddElement(elBe, fractionmass=10*perCent);
FLiBe->AddElement(elF, fractionmass=72*perCent);
G4ThreeVector pos2 = G4ThreeVector(0, -1 * cm, 7 * cm);

  // Trapezoid shape
  G4double shape1_dxa = 1*m ;//12 * cm, shape1_dxb = 12 * cm;
  G4double shape1_dya = 1*m ;//10 * cm, shape1_dyb = 10 * cm;
  G4double shape1_dz = 1*m ;//5 * cm;
  auto solidShape1 = new G4Box(  // its name
            "Shape1",  // its name
            0.5 * shape1_dxa, 0.5 * shape1_dya, 0.5 * shape1_dz);  // its size
            
  // Note: G4Box is a special case of G4Trd, so we
  // could also use G4Trd here, but G4Box is more intuitive for
  // rectangular shapes. If you want to use G4Trd, you can do so
  // like this:
   // new G4Trd("Shape1",  // its name
   //           0.2 * shape1_dxa, 0.2 * shape1_dxb, 0.9 * shape1_dya, 0.9 * shape1_dyb,
   //           0.5 * shape1_dz);  // its size

  auto logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
                                          FLiBe,  // its material
                                         "Shape1");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos2,  // at position
                    logicShape1,  // its logical volume
                    "Shape1",  // its name
                    logicWorld,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

                   
   fScoringVolume = logicShape1;
  
   return physWorld;

  //
  // always return the physical World
  //
  
}
void DetectorConstruction::ConstructSDandField()
{
  G4SDManager* sdMan = G4SDManager::GetSDMpointer();
  auto tritSD = new TritiumSD("TritiumSD");
  sdMan->AddNewDetector(tritSD);
  if (fScoringVolume) {
      fScoringVolume->SetSensitiveDetector(tritSD);
  } else {
      G4Exception("DetectorConstruction::ConstructSDandField()",
                  "NoVolume", JustWarning,
                  "fScoringVolume is null, cannot assign sensitive detector.");
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
