#include "DetectorConstruction.hh"
#include "LACCBuilder.hh"
#include "TestCCBuilder.hh"
#include "SpentFuelAssemblyBuilder.hh"
#include "CCSensitiveDetector.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"

#include "G4PVPlacement.hh"

#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // World
    G4double worldSize = 10.*m;
    auto worldSol = new G4Box("World", 0.5*worldSize, 0.5*worldSize, 0.5*worldSize);
    auto nistAir = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    auto worldLV = new G4LogicalVolume(worldSol, nistAir, "World");
    auto worldVA = new G4VisAttributes(G4Colour::White());
    worldVA->SetForceWireframe(true);
    worldLV->SetVisAttributes(worldVA);
    auto worldPV = new G4PVPlacement(nullptr, G4ThreeVector(), worldLV, "World", nullptr, false, 0);

    // Spent Fuel Assembly
    auto spentFuelAssembly = new SpentFuelAssembly("SpentFuelAssembly", nistAir);
    spentFuelAssembly->SetFuelRodStatus(1.);
    spentFuelAssembly->PrintFuelRodStatus(G4cout);
    G4double spentFuelAssemblySurfaceDistance = 10.*cm;
//    G4double spentFuelAssemblyLength =
//            2*static_cast<G4Box*>(spentFuelAssembly->GetLogicalVolume()->GetSolid())->GetYHalfLength();
//    new G4PVPlacement(G4Transform3D(G4RotationMatrix().rotateX(90.*deg),
//                                    G4ThreeVector(0., 0., spentFuelAssemblySurfaceDistance + spentFuelAssemblyLength/2.)),
//                      spentFuelAssembly->GetLogicalVolume(), "SpentFuelAssembly", worldLV, false, 0);
    G4double spentFuelAssemblyHeight =
            2*static_cast<G4Box*>(spentFuelAssembly->GetLogicalVolume()->GetSolid())->GetZHalfLength();
    new G4PVPlacement(G4Transform3D(G4RotationMatrix(),
                                    G4ThreeVector(0., 0., spentFuelAssemblySurfaceDistance + spentFuelAssemblyHeight/2.)),
                      spentFuelAssembly->GetLogicalVolume(), "SpentFuelAssembly", worldLV, false, 0);

    // TestCC1
    fCC = std::make_shared<TestCC1>("TestCC1", 5.*cm);
    G4double CCHeight = 2*static_cast<G4Box*>(fCC->GetLogicalVolume()->GetSolid())->GetZHalfLength();
    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -CCHeight/2),
                      fCC->GetLogicalVolume(), "ComptonCamera", worldLV, false, 0);

//    // LACC
//    fCC = std::make_shared<LACC>("LACC");
//    G4double laccHeight = 2*static_cast<G4Box*>(fCC->GetLogicalVolume()->GetSolid())->GetZHalfLength();
//    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -laccHeight/2),
//                      fCC->GetLogicalVolume(), "ComptonCamera", worldLV, false, 0);

//    // Tungsten shield
//    G4double shieldWidth = 30.*cm, shieldLength = 30.*cm, shieldHeight = 5.*cm;
//    auto shieldSol = new G4Box("Shield", .5*shieldWidth, .5*shieldLength, .5*shieldHeight);
//    auto nistW = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
//    auto shieldLV = new G4LogicalVolume(shieldSol, nistW, "Shield");
//    auto shieldVA = new G4VisAttributes(G4Colour::Grey());
//    shieldVA->SetForceSolid();
//    shieldLV->SetVisAttributes(shieldVA);
//    new G4PVPlacement(nullptr, G4ThreeVector(0., 0., .5*shieldHeight), shieldLV, "Shield", worldLV, false, 0);

    return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
    auto sd_Det = new CCSensitiveDetector("LACC");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd_Det);
//    SetSensitiveDetector(std::static_pointer_cast<LAScintDet>(std::static_pointer_cast<SimpleScAbCC>(fCC)->GetScatter())->GetCrystalLV(), sd_Det);
//    SetSensitiveDetector(std::static_pointer_cast<LAScintDet>(std::static_pointer_cast<SimpleScAbCC>(fCC)->GetAbsorber())->GetCrystalLV(), sd_Det);
    SetSensitiveDetector(std::static_pointer_cast<SimpleScAbCC>(fCC)->GetScatter()->GetLogicalVolume(), sd_Det);
    SetSensitiveDetector(std::static_pointer_cast<SimpleScAbCC>(fCC)->GetAbsorber()->GetLogicalVolume(), sd_Det);
}
