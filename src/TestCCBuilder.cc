#include "TestCCBuilder.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"

GAGGDetector::GAGGDetector()
: Detector()
{
    G4double crystalWidth = 5.*cm;
    G4double crystalLength = crystalWidth;
    G4double crystalThickness = 10.*mm;
    G4NistManager* nist = G4NistManager::Instance();
    auto nistElGd = nist->FindOrBuildElement("Gd");
    auto nistElAl = nist->FindOrBuildElement("Al");
    auto nistElGa = nist->FindOrBuildElement("Ga");
    auto nistElO = nist->FindOrBuildElement("O");
    auto nistElCe = nist->FindOrBuildElement("Ce");
    auto GAGGCe = G4Material::GetMaterial("GAGGCe", false);
    if(!GAGGCe)
    {
        auto GAGG = new G4Material("GAGG", 6.63*g/cm3, 4, kStateSolid);
        GAGG->AddElement(nistElGd, 3);
        GAGG->AddElement(nistElAl, 2);
        GAGG->AddElement(nistElGa, 3);
        GAGG->AddElement(nistElO, 12);
        GAGGCe = new G4Material("GAGGCe", 6.63*g/cm3, 2, kStateSolid);
        GAGGCe->AddMaterial(GAGG, 99.*perCent);
        GAGGCe->AddElement(nistElCe, 1.*perCent);
    }
    auto crystalSol = new G4Box("GAGGDetector", crystalWidth/2., crystalLength/2., crystalThickness/2.);
    fDetectorLV = new G4LogicalVolume(crystalSol, GAGGCe, "GAGGDetector");
    auto crystalVA = new G4VisAttributes(G4Colour::Yellow());
    crystalVA->SetForceSolid();
    fDetectorLV->SetVisAttributes(crystalVA);
}

TestCC1::TestCC1(G4String name, G4double sc2abDistance)
: SimpleScAbCC(name, sc2abDistance)
{
    fScatter = std::make_shared<GAGGDetector>();
    fAbsorber = std::make_shared<GAGGDetector>();

    G4ThreeVector pMin, pMax;
    fScatter->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
    G4ThreeVector scatterDimension = pMax - pMin;
    fAbsorber->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
    G4ThreeVector absorberDimension = pMax - pMin;
    G4double CCWidth = std::max(scatterDimension.x(), absorberDimension.x());
    G4double CCLength = std::max(scatterDimension.y(), absorberDimension.y());
    fSc2AbDistance = std::max(scatterDimension.z(), fSc2AbDistance);
    G4double CCHeight = fSc2AbDistance + absorberDimension.z();

    auto CCSol = new G4Box(GetName(), CCWidth/2., CCLength/2., CCHeight/2.);
    auto nistAir = G4Material::GetMaterial("G4_AIR");
    fCCLV = new G4LogicalVolume(CCSol, nistAir, GetName());

    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., CCHeight/2. - scatterDimension.z()/2.),
                      fScatter->GetLogicalVolume(), "Scatter", fCCLV, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., - CCHeight/2. + absorberDimension.z()/2.),
                      fAbsorber->GetLogicalVolume(), "Absorber", fCCLV, false, 1);
}
