#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <vector>
#include <string>
using namespace std;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
private:
    std::vector<Actor*> actors;
    Iceman* myIceman;
    static const int maxIceWidth = 63;
    static const int maxIceHeight = 59;
    unique_ptr<Ice> oilField[maxIceWidth][maxIceHeight];

    unsigned int numTicks;
    int ticksBeforeProtester;
    int numTargetProtesters;
    int numProtesters;

public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();

    virtual int init();
    virtual int move() override;
    virtual void cleanUp();

    // Add an actor to the world.
    void addActor(Actor* a);

    // Clear a 4x4 region of Ice.
    void clearIce(int x, int y);

    // Can actor move to x,y?
    bool canActorMoveTo(Actor* a, int x, int y) const;

    // Annoy all other actors within radius of annoyer, returning the
    // number of actors annoyed.
    int annoyAllNearbyActors(Actor* annoyer, int points, int radius);

    // Reveal all objects within radius of x,y.
    void revealAllNearbyObjects(int x, int y, int radius);

    // If the IceMan is within radius of a, return a pointer to the
    // IceMan, otherwise null.
    Actor* findNearbyIceMan(Actor* a, int radius) const;

    // If at least one actor that can pick things up is within radius of a,
    // return a pointer to one of them, otherwise null.
    Actor* findNearbyPickerUpper(Actor* a, int radius) const;

    // Annoy the IceMan.
    void annoyIceMan(unsigned int amount);

    // Give IceMan some sonar charges.
    void giveIceManSonar();

    // Give IceMan some water.
    void giveIceManWater();
    
    // Is the Actor a facing toward the IceMan?
    bool facingTowardIceMan(Actor* a) const;

    // If the Actor a has a clear line of sight to the IceMan, return
    // the direction to the IceMan, otherwise GraphObject::none.
    GraphObject::Direction lineOfSightToIceMan(Actor* a) const;

    // Return whether the Actor a is within radius of IceMan.
    bool isNearIceMan(Actor* a, int radius) const;

    // Determine the direction of the first move a quitting protester
    // makes to leave the oil field.
    GraphObject::Direction determineFirstMoveToExit(int x, int y);

    // Determine the direction of the first move a hardcore protester
    // makes to approach the IceMan.
    GraphObject::Direction determineFirstMoveToIceMan(int x, int y);

    // Helper Function that sets the text at the top of the screen
    std::string getDisplayText() const;

    bool hasIceAt(int x, int y) const;
};

#endif // STUDENTWORLD_H_