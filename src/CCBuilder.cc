#include "CCBuilder.hh"
#include "G4LogicalVolume.hh"

Detector::Detector()
{}

Detector::~Detector()
{}

ComptonCamera::ComptonCamera(G4String name): fName(name)
{
    auto wptr = std::shared_ptr<ComptonCamera>(this, [](ComptonCamera*){});
    ComptonCameraStore::GetInstance()->Register(shared_from_this());
}

ComptonCamera::~ComptonCamera()
{}

std::shared_ptr<ComptonCamera> ComptonCameraStore::GetComptonCamera(const G4String& name) const
{
    auto pStore = GetInstance();
    for(const auto& pData: *pStore)
        if(pData->GetName()==name) return pData;

    G4Exception("ComptonCameraStore::GetComptonCamera()", "", JustWarning,
                G4String("    No ComptonCamera exists named '" + name + "'.").c_str());
    return std::shared_ptr<ComptonCamera>(nullptr);
}

SimpleScAbCC::SimpleScAbCC(G4String name, G4double sc2abDistance)
: ComptonCamera(name), fSc2AbDistance(sc2abDistance)
{}
