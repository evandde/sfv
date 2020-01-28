#ifndef CCBUILDER_HH
#define CCBUILDER_HH

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4UImanager.hh"

class G4LogicalVolume;

class Detector
{
public:
    Detector();
    virtual ~Detector();

    G4LogicalVolume* GetLogicalVolume() const { return fDetectorLV; }

protected:
    G4LogicalVolume* fDetectorLV;
};

class ComptonCamera: public std::enable_shared_from_this<ComptonCamera>
{
public:
    ComptonCamera(G4String name);
    virtual ~ComptonCamera();

    void SetName(G4String name) { fName = name; }
    G4String GetName() const { return fName; }
    void Print() { G4UImanager::GetUIpointer()->ApplyCommand("/vis/drawTree " + fName); }
    G4LogicalVolume* GetLogicalVolume() const { return fCCLV; }

protected:
    G4LogicalVolume* fCCLV;

private:
    G4String fName;
};

class ComptonCameraStore: public std::vector< std::shared_ptr<ComptonCamera> >
{
public:
    static auto GetInstance()
    {
        static ComptonCameraStore fInstance;
        return &fInstance;
    }
    static void Register(std::shared_ptr<ComptonCamera> pCC)
    { GetInstance()->push_back(pCC); }

    std::shared_ptr<ComptonCamera> GetComptonCamera(const G4String& name) const;

private:
    ComptonCameraStore() = default;
    ~ComptonCameraStore() = default;
};

class SimpleScAbCC: public ComptonCamera
{
public:
    SimpleScAbCC(G4String name, G4double sc2abDistance);

    std::shared_ptr<Detector> GetScatter() const { return fScatter; }
    std::shared_ptr<Detector> GetAbsorber() const { return fAbsorber; }

protected:
    std::shared_ptr<Detector> fScatter;
    std::shared_ptr<Detector> fAbsorber;
    G4double fSc2AbDistance;
};

#endif // CCBUILDER_HH
