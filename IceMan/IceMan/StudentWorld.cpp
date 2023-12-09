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
            std::unique_ptr<Ice> ice = std::make_unique<Ice>(this, x + 1, y + 1);

            oilField[x][y] = std::move(ice);
        }
    }

    //clear starting columns in oil field
    for (int x = 29; x < 33; x++) {
        for (int y = 4; y < 59; y++) {
            oilField[x][y].reset(); // Reset each unique pointer
        }

    }
    
    //create Boulders
    Boulder* boulder = new Boulder(this, 15, 40);
    boulder->move();
    addActor(boulder);

    //create Gold

    //create Oil Barrels
    
    //set protester numbers
    ticksBeforeProtester = std::max(25, 0);
    numTargetProtesters = std::min(15, 2 + static_cast<int>(getLevel()));
    numProtesters = 0;
    numTicks = 0;


    setGameStatText(getDisplayText());

    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    numTicks++;
    for (auto& actor : actors) {
        if (actor->Alive()) {
            //myIceman->doSomething();
//            actor->doSomething();
            actor->move();
            if (!myIceman->Alive()) {

                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }

    //iceman stuff
    myIceman->doSomething();

    // clear ice
    int icemanX = myIceman->getX();
    int icemanY = myIceman->getY();

    clearIce(icemanX, icemanY);
    if (!myIceman->Alive()) {

        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    //create new protester
    if (numTicks == ticksBeforeProtester && numProtesters < numTargetProtesters) {
        Protester* p = new Protester(this, 40, 60, IID_PROTESTER, 5, 0);
        addActor(p);
    }

    setGameStatText(getDisplayText());

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    
    while (!actors.empty()){
        delete actors.back();
        actors.pop_back();
    }

    delete myIceman;

    //delete oilField
    for (int i = 0; i < maxIceWidth; ++i) {
        for (int j = 0; j < maxIceHeight; ++j) {
            std::unique_ptr<Ice>& ice = oilField[i][j];
            if (ice) {
                oilField[i][j].reset(); // Reset each unique pointer
            }
            
        }
    }

    
    cout << "cleanup!!!" << endl;
    
}

// Add an actor to the world.
void StudentWorld::addActor(Actor* a) {
    actors.push_back(a);
}

// Clear a 4x4 region of Ice.
void StudentWorld::clearIce(int x, int y) {
    // Define the range for clearing ice (4x4 area around object)
    int leftBound = std::max(x - 1, 0);
    int rightBound = std::min(x + 2, maxIceWidth - 1);
    int lowerBound = std::max(y - 1, 0);
    int upperBound = std::min(y + 2, maxIceHeight - 1);

    // Loop through the 4x4 area and clear ice
    for (int x = leftBound; x <= rightBound; x++) {
        for (int y = lowerBound; y <= upperBound; y++) {
            std::unique_ptr<Ice>& ice = oilField[x][y];
            if (ice) {
                ice->setVisible(false);
                ice.reset(); // Optionally remove the ice object
                playSound(SOUND_DIG);
            }
        }
    }

}

// Can actor move to x,y?
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {
    bool ret = true;
    for (auto& currentActor : actors) {
        if (currentActor->canActorsPassThroughMe() == false) {
            // left and lower bounds are ON the actor, right and upper bounds are ONE SQUARE past the actor
            int leftBound = currentActor->getX();
            int rightBound = currentActor->getX() + currentActor->getSize() * 4;
            int lowerBound = currentActor->getY();
            int upperBound = currentActor->getY() + currentActor->getSize() * 4;

            // check to see if new coordinates are within the range of the immovable actor
            if (x >= leftBound && x < rightBound || y >= lowerBound && y < upperBound) {
                ret = false;
            }
            
        }
    }
    return ret;
}

// Annoy all other actors within radius of annoyer, returning the
// number of actors annoyed.
int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius) {
    // Retrieve Iceman's current position
    int icemanX = myIceman->getX();
    int icemanY = myIceman->getY();

    // Get annoyer's current location
    int objX = annoyer->getX();
    int objY = annoyer->getY();

    // Check if X position is between (radius) units to the right and left of Iceman
    if (objX < icemanX + 4 * radius && icemanX - 3 * radius <= objX && objY < icemanY + 4 * radius && icemanY - radius * 3 <= objY) {
        annoyIceMan(points);
    }

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
void StudentWorld::annoyIceMan(unsigned int amount) {
    // Annoy checks the health of Iceman, if it returns true, iceman has died
    if (myIceman->annoy(amount)) {
        // Set state of Alive to false
        myIceman->setDead();
    }
}

// Give IceMan some sonar charges.
void StudentWorld::giveIceManSonar() {

}

// Give IceMan some water.
void StudentWorld::giveIceManWater() {

}

// Is the Actor a facing toward the IceMan?
bool StudentWorld::facingTowardIceMan(Actor* a) const {
    // Retrieve Iceman's current position
    int icemanX = myIceman->getX();
    int icemanY = myIceman->getY();

    // Get Actor's current location and direction
    int objX = a->getX();
    int objY = a->getY();
    GraphObject::Direction dir = a->getDirection();

    // Compare
    bool ret = false;
    if (dir == GraphObject::right && objX < icemanX && icemanY < objY + 4 * a->getSize() && icemanY >= objY - 3 * a->getSize()) {
        ret = true;
    }
    else if (dir == GraphObject::left && objX > icemanX && icemanY < objY + 4*a->getSize() && icemanY >= objY - 3*a->getSize()) {
        ret = true;
    }
    else if (dir == GraphObject::up && objY < icemanY && icemanX >= objX - 3 * a->getSize() && icemanX < objX + 4 * a->getSize()) {
        ret = true;
    }
    else if (dir == GraphObject::down && objY > icemanY && icemanX >= objX - 3 * a->getSize() && icemanX < objX + 4 * a->getSize()) {
        ret = true;
    }

    return ret;
}

// If the Actor a has a clear line of sight to the IceMan, return
// the direction to the IceMan, otherwise GraphObject::none.
GraphObject::Direction StudentWorld::lineOfSightToIceMan(Actor* a) const {
    GraphObject::Direction dir = GraphObject::Direction::none;
    if (facingTowardIceMan(a)) {
        int x = a->getX();
        int y = a->getY();

        
    }

    return dir;
}

// Return whether the Actor a is within radius of IceMan. (Set radius to 3 when calling)
bool StudentWorld::isNearIceMan(Actor* a, int radius) const {
    // Retrieve Iceman's range with radius
    int icemanX = myIceman->getX();
    int icemanY = myIceman->getY();

    // Set Iceman's bounds
    int iceLeftSide = icemanX - radius + 1;
    int iceRightSide = icemanX + radius;
    int iceBottomSide = icemanY - radius + 1;
    int iceTopSide = icemanX + radius;


    // Get Actor's outer coordinates
    int leftbound = a->getX() - 1;
    int rightbound = a->getX() + 2;
    int lowerbound = a->getY() - 1;
    int upperbound = a->getY() + 2;

    bool ret = false;

    // Check if (radius) units around iceman are in object bounds

    //check if left side of Iceman collides with object
    if ((iceLeftSide <= rightbound && !(iceLeftSide <= leftbound - 4)) &&
            ((icemanY - radius + 1 <= upperbound && icemanY + radius >= upperbound) ||
                (icemanY - radius + 1 <= lowerbound && icemanY + radius >= lowerbound))) {
        ret = true;
    }
    //check if right side collies with object
    else if ((icemanX + radius >= leftbound && !(iceRightSide >= rightbound + 4)) &&
        ((icemanY - radius + 1 <= upperbound && icemanY + radius >= upperbound) ||
            (icemanY - radius + 1 <= lowerbound && icemanY + radius >= lowerbound))) {
        ret = true;
    }
    //check if bottom collides with object
    else if ((icemanY - radius + 1 >= upperbound) &&
        ((icemanX - radius + 1 <= rightbound && icemanX + radius >= rightbound) ||
            (icemanX - radius + 1 <= leftbound && icemanX + radius >= leftbound))) {
        ret = true;
    }
    //check if top collides with object
    else if( (icemanY + radius >= lowerbound) &&
            ((icemanX - radius + 1 <= rightbound && icemanX + radius >= rightbound) ||
                (icemanX - radius + 1 <= leftbound && icemanX + radius >= leftbound))) {
        ret = true;
    }

    
    return ret;
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
    int score = static_cast<int>(getScore());
    
    string ret = "Lvl: " + std::to_string(level)
        + " Lives: " + to_string(lives) + " Health : " + to_string(health) + "% Wtr : "
        + "Gld : 5 Oil Left : 2 Sonar : 1 Scr : " + to_string(score);

    return ret;
}

bool StudentWorld::hasIceAt(int x, int y) const {
    // Check if the coordinates are within the bounds of the oil field
    if (x < 0 || x >= maxIceWidth || y < 0 || y >= maxIceHeight) {
        return false;
    }

    // Check if there is ice at the specified location
    return oilField[x][y] != nullptr;
}
