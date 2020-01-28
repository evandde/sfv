#ifndef LACCBUILDER_HH
#define LACCBUILDER_HH

#include "CCBuilder.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4UImanager.hh"

class G4LogicalVolume;

class LAScintDet: public Detector
{
public:
    LAScintDet(G4double crystalThickness);

    G4LogicalVolume* GetCrystalLV() const { return fCrystalLV; }
    G4LogicalVolume* GetPMTPhotoCathodeLV() const { return fPMTPhotoCathodeLV; }

private:
    G4LogicalVolume* ConstructFrontHousing();
    G4LogicalVolume* ConstructRearHousing();

    void DefineMaterials();
    static G4bool fDefineMaterialsFlag;

    G4double fCrystalThickness;

    G4LogicalVolume* fCrystalLV;
    G4LogicalVolume* fPMTPhotoCathodeLV;
};

class LACC: public SimpleScAbCC
{
public:
    LACC(G4String name,
         G4double scThk = 2.*cm,
         G4double abThk = 3.*cm,
         G4double sc2abDist = 25.*cm);
};

#endif // LACCBUILDER_HH
