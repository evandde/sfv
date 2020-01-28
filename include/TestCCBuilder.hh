#ifndef TESTCCBUILDER_HH
#define TESTCCBUILDER_HH

#include "CCBuilder.hh"

class GAGGDetector: public Detector
{
public:
    GAGGDetector();
};

class TestCC1: public SimpleScAbCC
{
public:
    TestCC1(G4String name, G4double sc2abDistance);
};

#endif // TESTCCBUILDER_HH
