#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir) { }

StudentWorld::~StudentWorld() {

}

int StudentWorld::init()
{
    //create Iceman
    int initialX = 30;
    int initialY = 60;
    myIceman = new Iceman(this, initialX, initialY);

    //create oil field
    for (int x = 0; x < maxIceWidth; ++x) {
        for (int y = 0; y < maxIceHeight; ++y) {
            std::unique_ptr<Ice> ice = std::make_unique<Ice>(this, x, y);

            oilField[x][y] = std::move(ice);
        }
    }
    
    //create Boulders

    //create Gold

    //create Oil Barrels
    //200 - static_cast<int>(getLevel())
    //set protester numbers
    Protester* p = new Protester(this, 64, 64, IID_PROTESTER, 5, 0);
    addActor(p);

    ticksBeforeProtester = std::max(25, 0);
    numTargetProtesters = std::min(15, 2 + static_cast<int>(getLevel()));
    numProtesters = 0;


    setGameStatText(getDisplayText());

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    numTicks++;
    for (auto& actor : actors) {
        if (actor->Alive()) {
            myIceman->doSomething();

            if (!myIceman->Alive()) {

                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }
    myIceman->doSomething();
    int icemanX = myIceman->getX();
    int icemanY = myIceman->getY();

    clearIce(icemanX, icemanY);
    if (!myIceman->Alive()) {

        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    //create new protester
    if (numTicks == ticksBeforeProtester && numProtesters < numTargetProtesters) {
        Protester* p = new Protester(this, 30, 60, IID_PROTESTER, 5, 0);
        addActor(p);
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {

}

// Add an actor to the world.
void StudentWorld::addActor(Actor* a) {
    actors.push_back(a);
}

void StudentWorld::clearIce(int x, int y) {
    // Retrieve Iceman's current position
    int icemanX = myIceman->getX();
    int icemanY = myIceman->getY();

    // Define the range for clearing ice (4x4 area around Iceman)
    int leftBound = std::max(icemanX, 0);
    int rightBound = std::min(icemanX + 3, maxIceWidth - 1);
    int lowerBound = std::max(icemanY, 0);
    int upperBound = std::min(icemanY + 3, maxIceHeight - 1);

    // Loop through the 4x4 area and clear ice
    for (int x = leftBound; x <= rightBound; x++) {
        for (int y = lowerBound; y <= upperBound; y++) {
            std::unique_ptr<Ice>& ice = oilField[x][y];
            if (ice) {
                ice->setVisible(false);
                ice.reset(); // Optionally remove the ice object
            }
        }
    }
}


// Can actor move to x,y?
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {
    return false;
}

// Annoy all other actors within radius of annoyer, returning the
// number of actors annoyed.
int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius) {
    return 0;
}

// Reveal all objects within radius of x,y.
void StudentWorld::revealAllNearbyObjects(int x, int y, int radius) {

}

// If the IceMan is within radius of a, return a pointer to the
// IceMan, otherwise null.
Actor* StudentWorld::findNearbyIceMan(Actor* a, int radius) const {
    return a;
}

// If at least one actor that can pick things up is within radius of a,
// return a pointer to one of them, otherwise null.
Actor* StudentWorld::findNearbyPickerUpper(Actor* a, int radius) const {
    return a;
}

// Annoy the IceMan.
void StudentWorld::annoyIceMan() {

}

// Give IceMan some sonar charges.
void StudentWorld::giveIceManSonar() {

}

// Give IceMan some water.
void StudentWorld::giveIceManWater() {

}

// Is the Actor a facing toward the IceMan?
bool StudentWorld::facingTowardIceMan(Actor* a) const {
    return false;
}

// If the Actor a has a clear line of sight to the IceMan, return
// the direction to the IceMan, otherwise GraphObject::none.
GraphObject::Direction StudentWorld::lineOfSightToIceMan(Actor* a) const {
    return GraphObject::Direction::right;
}

// Return whether the Actor a is within radius of IceMan.
bool StudentWorld::isNearIceMan(Actor* a, int radius) const {
    return false;
}

// Determine the direction of the first move a quitting protester
// makes to leave the oil field.
GraphObject::Direction StudentWorld::determineFirstMoveToExit(int x, int y) {
    return GraphObject::Direction::right;
}

// Determine the direction of the first move a hardcore protester
// makes to approach the IceMan.
GraphObject::Direction StudentWorld::determineFirstMoveToIceMan(int x, int y) {
    return GraphObject::Direction::right;
}

// Helper Function that sets the text at the top of the screen
std::string StudentWorld::getDisplayText() const {
    int level = static_cast<int>(getLevel());
    int lives = static_cast<int>(getLives());
    int health = static_cast<int>(myIceman->getHealth()) * 10;
    //int gold = static_cast<int>()
    
    string ret = "Lvl: " + std::to_string(level)
        + " Lives: " + to_string(lives) + " Health : " + to_string(health) + "% Wtr : "
        + "Gld : 5 Oil Left : 2 Sonar : 1 Scr : 0";

    return ret;
}
