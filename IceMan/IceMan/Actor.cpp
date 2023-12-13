#include "Actor.h"
#include "StudentWorld.h"
#include <queue>
#include <set>
#include <iostream>
#include <random>


//Actor Class
Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir, bool visible, int imageID, double size, int depth) : GraphObject(imageID, startX, startY, startDir, size, depth) {
    setVisible(visible);
    myWorld = world;
    isAlive = true;
}

Actor::~Actor() {

}

bool Actor::Alive() const {
    return isAlive;
}

StudentWorld* Actor::getWorld() const {
    return myWorld;
}

// Mark this actor as dead.
void Actor::setDead() {
    isAlive = false;
}

// Annoy this actor.
bool Actor::annoy(unsigned int amt) {
    return false;
}

// Can other actors pass through this actor?
bool Actor::canActorsPassThroughMe() const {
    return true;
}

// Can this actor dig through Ice?
bool Actor::canDigThroughIce() const {
    return false;
}

// Can this actor pick items up?
bool Actor::canPickThingsUp() const {
    return false;
}

// Does this actor hunt the IceMan?
bool Actor::huntsIceMan() const {
    return false;
}

// Can this actor need to be picked up to finish the level?
bool Actor::needsToBePickedUpToFinishLevel() const {
    return false;
}

// Move this actor to x,y if possible, and return true; otherwise,
// return false without moving.
bool Actor::moveToIfPossible(int x, int y) {

    
    if (getWorld()->canActorMoveTo(this, x, y)) {
        this->moveTo(x, y);
        return true;
    }
    

    return false;
}
// Actor Class End






//Ice Class start
Ice::Ice(StudentWorld* world, int startX, int startY) :
    Actor(world, startX, startY, right, true, IID_ICE, 0.25, 3) { }

void Ice::move() {

}
// Ice Class end







// Agent Class
Agent::Agent(StudentWorld* world, int startX, int startY, Direction startDir,
    int imageID, unsigned int hitPoints) : Actor(world, startX, startY,
        startDir, true, imageID, 1.0, 0)
{
    health = hitPoints;
}

// How many hit points does this actor have left?
unsigned int Agent::getHealth() const {
    return health;
}

// Decrement health -- Is Agent dead? If health drops to or below zero, return true
bool Agent::annoy(unsigned int amount) {
    health -= amount;
    if (health > 0) {
        return false;
    }
    else {
        return true;
    }
}

bool Agent::canPickThingsUp() const {
    return false;
}
// Agent Class End








//Squirt Class
Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction dir) : 
    Actor(world, startX, startY, dir, true, IID_WATER_SPURT, 1.0, 1)
{
    setVisible(true);
    travelDistance = 4;
    initial = 0;
}

void Squirt::move()
{
    if (getWorld()->annoyAllNearbyActors(this, 2, 3) > 0) {
        setVisible(false);
        setDead();
    }
    if (getDirection() == up) {
        if (!moveToIfPossible(getX(), getY() + 1)) {
            setVisible(false);
            setDead();
        }
    }
    else if (getDirection() == down) {
        if (!moveToIfPossible(getX(), getY() - 1)) {
            setVisible(false);
            setDead();
        }
    }
    else if (getDirection() == right) {
        if (!moveToIfPossible(getX() + 1, getY())) {
            setVisible(false);
            setDead();
        }
    }
    else if (getDirection() == left) {
        if (!moveToIfPossible(getX() - 1, getY())) {
            setVisible(false);
            setDead();
        }
    }
    if (initial >= travelDistance) {
        setVisible(false);
        setDead();
    }
    if (Alive()) {
        initial++;
    }
}
// Squirt Class End








// Iceman Class Start
Iceman::Iceman(StudentWorld* world, int startX, int startY)
    : Agent(world, startX, startY, right, IID_PLAYER, 10)

{

}

void Iceman::doSomething()
{
    if (!Alive())

    {
        return;
    }

    int ch;
    if (getWorld()->getKey(ch) == true) {

        switch (ch) {
        case KEY_PRESS_LEFT:
            setDirection(left);
            if (getX() > 0)
                moveToIfPossible(getX() - 1, getY());
            break;
        case KEY_PRESS_RIGHT:
            setDirection(right);
            if (getX() < VIEW_WIDTH - 1)
                moveToIfPossible(getX() + 1, getY());
            break;
        case KEY_PRESS_UP:
            setDirection(up);
            if (getY() < VIEW_HEIGHT - 1)
                moveToIfPossible(getX(), getY() + 1);
            break;
        case KEY_PRESS_DOWN:
            setDirection(down);
            if (getY() > 0)
                moveToIfPossible(getX(), getY() - 1);
            break;
        case KEY_PRESS_SPACE:
            if (water > 0) {
                if (!createSquirtIfPossible()) {
                    water--;
                    cout << "didn't squirt" << endl;
                }
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
            }
            break;
        case 'z':
            addWater();
            break;
        }
    }
}

void Iceman::move() {

}

// Check health of Iceman
bool Iceman::annoy(unsigned int amount) {
    health -= amount;
    
    if (static_cast<int>(health) > 0) {
        getWorld()->playSound(SOUND_PLAYER_ANNOYED);
        return false;
    }
    else {
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        return true;
    }
}

void Iceman::addGold() {

}

bool Iceman::canDigThroughIce() const {
    return false;
}

// Pick up a sonar kit.
void Iceman::addSonar() {

}

// Pick up water.
void Iceman::addWater() {
    water += 5;
}

// Get amount of gold
unsigned int Iceman::getGold() const {
    return 0;
}

// Get amount of sonar charges
unsigned int Iceman::getSonar() const {
    return 0;
}

// Get amount of water
unsigned int Iceman::getWater() const {
    return water;
}


bool Iceman::isThereIceInSquare(int x, int y) {
    for (int r = x - 1; r < x + 3; r++) {
        for (int c = y - 1; c < y + 3; c++) {

            // If in bounds, check if there is ice at coordinates
            if ( r >= 0 && r <= 63 && c >= 1 && c <= 59) {
                if (getWorld()->hasIceAt(r, c)) {
                    return true;
                }
            }

        }
    }
    return false;
}


bool Iceman::createSquirtIfPossible() {
    bool canCreateSquirt = true;

    if (getDirection() == right) {
        for (int x = getX() + 3; x < getX() + 5; x++) {
            if (!getWorld()->canActorMoveTo(this, x, getY())) {
                canCreateSquirt = false;
            }
        }
        if (isThereIceInSquare(getX() + 4, getY())) {
            canCreateSquirt = false;
        }
        if (canCreateSquirt) {
            Squirt* squirt = new Squirt(getWorld(), getX() + 4, getY(), right);
            getWorld()->addActor(squirt);
            water--;
        }
    }
    else if (getDirection() == left) {
        for (int x = getX() - 4; x > getX() - 6; --x) {
            if (!getWorld()->canActorMoveTo(this, x, getY())) {
                canCreateSquirt = false;
            }
        }
        if (isThereIceInSquare(getX() - 5, getY())) {
            canCreateSquirt = false;
        }
        if (canCreateSquirt) {
            Squirt* squirt = new Squirt(getWorld(), getX() - 4, getY(), left);
            getWorld()->addActor(squirt);
            water--;
        }
    }
    else if (getDirection() == up) {
        for (int y = getY() + 3; y < getY() + 5; ++y) {
            if (!getWorld()->canActorMoveTo(this, getX(), y)) {
                canCreateSquirt = false;
            }
        }
        if (isThereIceInSquare(getX(), getY() + 4)) {
            canCreateSquirt = false;
        }
        if (canCreateSquirt) {
            Squirt* squirt = new Squirt(getWorld(), getX(), getY() + 4, up);
            getWorld()->addActor(squirt);
            water--;
        }
    }
    else if (getDirection() == down) {
        for (int y = getY() - 4; y > getY() - 6; --y) {
            if (!getWorld()->canActorMoveTo(this, getX(), y)) {
                canCreateSquirt = false;
            }
        }
        if (isThereIceInSquare(getX(), getY() - 5)) {
            canCreateSquirt = false;
        }
        if (canCreateSquirt) {
            Squirt* squirt = new Squirt(getWorld(), getX(), getY() - 4, down);
            getWorld()->addActor(squirt);
            water--;
        }
    }


    return canCreateSquirt;
}

// Iceman Class End









// Protestor Class (parent class)
Protester::Protester(StudentWorld* world, int startX, int startY,
    int imageID, unsigned int hitPoints) : Agent(world, startX, startY,
        Direction::left, imageID, hitPoints)
{
    mustLeaveOilField = false;
    ticksToWaitBetweenMoves = max(0, 3 - static_cast<int>(getWorld()->getLevel()) / 4);
    ticksToNextMove = ticksToWaitBetweenMoves;
    numSquaresToMoveInCurrentDirection = 0;
    resetNumSquares();
    currentState = hunting;
    goldAmount = 0;
    turnTimer = 0;
    restCounter = max(50, 100 - static_cast<int>(getWorld()->getLevel()) * 10);
    ticksRestedFor = 0;

}

void Protester::move() {

}

// Decrement health -- Is Agent dead? If health drops to or below zero, return true
bool Protester::annoy(unsigned int amount) {
    if (getState() == leaving) {
        return true;
    }
    health -= amount;

    if (static_cast<int>(getHealth()) > 0 && getState() != leaving) {
        setState(stunned);
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        return false;
    }
    else {
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setVisible(false);
        setDead();
        setState(leaving);
        return true;
    }
}

void Protester::addGold() {
    goldAmount++;
}


bool Protester::huntsIceMan() const {
    return false;
}

// Set state to having gien up protest
void Protester::setMustLeaveOilField() {
    if (getHealth() <= 0) {
        mustLeaveOilField = true;
    }
}

// Set number of ticks until next move
void Protester::setTicksToNextMove() {
    ticksToNextMove--;
}


// reset numSquaresInCurrentDirection to a random int
void Protester::resetNumSquares() {
    std::random_device rd;  // Create a random device to seed the generator
    std::mt19937 gen(rd()); // Create a Mersenne Twister pseudo-random generator

    // Create a uniform distribution within the specified range
    std::uniform_int_distribution<int> distribution(8, 60);

    // Generate a random number within the specified range
    int random_number = distribution(gen);

    numSquaresToMoveInCurrentDirection = random_number;
}

// Set state
void Protester::setState(state newState) {
    currentState = newState;
}

// Get state
Protester::state Protester::getState() const {
    return currentState;
}

// Check if there is ice at coordinates in front
bool Protester::isThereIceAt(int x, int y, Direction dir) {
    if (dir == right) {
        if (getWorld()->hasIceAt(x + 3, y) ||
            getWorld()->hasIceAt(x + 3, y - 1) ||
            getWorld()->hasIceAt(x + 3, y + 1) ||
            getWorld()->hasIceAt(x + 3, y + 2)) {
            return true;
        }
    }
    else if (dir == left) {
        if (getWorld()->hasIceAt(x - 2, y) ||
            getWorld()->hasIceAt(x - 2, y - 1) ||
            getWorld()->hasIceAt(x - 2, y + 1) ||
            getWorld()->hasIceAt(x - 2, y + 2)) {
            return true;
        }
    }
    else if (dir == up) {
        if (getWorld()->hasIceAt(x - 1, y + 3) ||
            getWorld()->hasIceAt(x, y + 3) ||
            getWorld()->hasIceAt(x + 1, y + 3) ||
            getWorld()->hasIceAt(x + 2, y + 3)) {
            return true;
        }
    }
    else if (dir == down) {
        if (getWorld()->hasIceAt(x - 1, y - 2) ||
            getWorld()->hasIceAt(x, y - 2) ||
            getWorld()->hasIceAt(x + 1, y - 2) ||
            getWorld()->hasIceAt(x + 2, y - 2)) {
            return true;
        }
    }
    return false;
}

// Check to see if motion is possible, and move if possible
bool Protester::moveIfPossible() {

    
    // take a step in direction it is facing if not blocked by boulder or ice or out of bounds
    if (getDirection() == right) {
        if (getWorld()->canActorMoveTo(this, getX() + 1, getY()) && !isThereIceAt(getX(), getY(), right)) {
            moveTo(getX() + 1, getY());
            return true;
        }
        
    }
    else if (getDirection() == left) {
        if (getWorld()->canActorMoveTo(this, getX() - 1, getY()) && !isThereIceAt(getX(), getY(), left)) {
            moveTo(getX() - 1, getY());
            return true;
        }
    }
    else if (getDirection() == up) {
        if (getWorld()->canActorMoveTo(this, getX(), getY() + 1) && !isThereIceAt(getX(), getY(), up)) {
            moveTo(getX(), getY() + 1);
            return true;
        }
    }
    else if (getDirection() == down) {
        if (getWorld()->canActorMoveTo(this, getX(), getY() - 1) && !isThereIceAt(getX(), getY(), down)) {
            moveTo(getX(), getY() - 1);
            return true;
        }
    }
    return false;

    /*
    cout << "back gere" << endl;


    // Check if there is NO ice in pathway
    //if (!isThereIceAt(getX(), getY(), getDirection())) {
        cout << "in gere" << endl;

        // take a step in direction it is facing if not blocked by boulder
        if (getDirection() == right) {
            if (moveToIfPossible(getX() + 1, getY())) {
                cout << "here x: " << getX() << " y: " << getY() << endl;
                return true;
            }
            else {
                //cout << "here" << endl;
            }
        }
        else if (getDirection() == left) {
            if (moveToIfPossible(getX() - 1, getY())) {
                return true;
            }
        }
        else if (getDirection() == up) {
            if (moveToIfPossible(getX(), getY() + 1)) {
                return true;
            }
        }
        else if (getDirection() == down) {
            if (moveToIfPossible(getX(), getY() - 1)) {
                return true;
            }
        }
    //}
    cout << "seriousky?" << endl;
    return false;
    */
}

GraphObject::Direction Protester::findNewDirection(std::set<int> mySet) {
    std::random_device rd;  // Create a random device to seed the generator
    std::mt19937 gen(rd()); // Create a Mersenne Twister pseudo-random generator
    //cout << "nahhhh" << endl;
    
    // Create a uniform distribution within the specified range
    std::uniform_int_distribution<int> distribution(1, 4);

    Direction ret = none;


    while (ret == none) {
        // Generate a random number within the specified range
        int rand = distribution(gen);
        if (rand == up && mySet.count(up) == 0) {
            if (getWorld()->canActorMoveTo(this, getX(), getY() + 1) && !isThereIceAt(getX(), getY(), up)) {
                ret = up;
            }
            else {
                mySet.insert(up);
                continue;
            }
        }
        else if (rand == down && mySet.count(down) == 0) {
            if (getWorld()->canActorMoveTo(this, getX(), getY() - 1) && !isThereIceAt(getX(), getY(), down)) {
                ret = down;
            }
            else {
                mySet.insert(down);
                continue;
            }
        }
        else if (rand == left && mySet.count(left) == 0) {
            if (getWorld()->canActorMoveTo(this, getX() - 1, getY()) && !isThereIceAt(getX(), getY(), left)) {
                ret = left;
            }
            else {
                mySet.insert(left);
                continue;
            }
        }
        else if (rand == right && mySet.count(right) == 0) {
            if (getWorld()->canActorMoveTo(this, getX() + 1, getY()) && !isThereIceAt(getX(), getY(), right)) {
                ret = right;
            }
            else {
                mySet.insert(right);
                continue;
            }
        }
    }

    return ret;
}

set<int> Protester::isPerpendicular() {
    StudentWorld* world = getWorld();
    set<int> notPerpendicular = {};

    // if current direction is up or down
    if (getDirection() - 2 <= 0) {
        if (!isThereIceAt(getX(), getY(), right) && getX() + 1 <= 61) {
            notPerpendicular.insert(up);
            notPerpendicular.insert(down);
            return notPerpendicular;
        }
        else if (!isThereIceAt(getX(), getY(), left) && getX() - 1 >= 1) {
            notPerpendicular.insert(up);
            notPerpendicular.insert(down);
            return notPerpendicular;
        }
        
        
    }
    // if current direction is left or right
    else {
        if (!isThereIceAt(getX(), getY(), up) && getY() + 1 <= 61) {
            notPerpendicular.insert(left);
            notPerpendicular.insert(right);
            return notPerpendicular;
        }
        else if (!isThereIceAt(getX(), getY(), down) && getY() - 1 >= 1) {
            notPerpendicular.insert(left);
            notPerpendicular.insert(right);
            return notPerpendicular;
        }
    }
    
    
    return notPerpendicular;
}
// Protester Class End









// Regular Protester
RegularProtester::RegularProtester(StudentWorld* world, int startX, int startY, int imageID) :
    Protester(world, startX, startY, imageID, 5)
{

}

void RegularProtester::move() {
    // STEP #1 chek if alive
    if (Alive()) {
        switch (currentState) {
        //STEP #2
        case stunned:
            if (ticksRestedFor >= restCounter) {
                currentState = hunting;
                ticksRestedFor = 0;
            }
            // increment rest counter
            ticksRestedFor++;
            break;

        //STEP #3
        case leaving:
            /*
            if (ticksToNextMove > 0) {
                // decrement
                setTicksToNextMove();
            }
            else {
                cout << "direction before" << getDirection() << endl;
                //setDirection(getWorld()->determineFirstMoveToExit(getX(), getY()));
                cout << "direction after" << getDirection() << endl;
                moveIfPossible();
                //moveTo(getX() + 1, getY());
                ticksToNextMove = ticksToWaitBetweenMoves;
                if (getX() == 61 && getY() == 61) {
                    setVisible(false);
                    setDead();
                }
            }
            // leave oil field
            */
            break;

        case hunting:
            if (ticksToNextMove != 0) {
                //decrement Ticks To Next Move
                setTicksToNextMove();
            }
            else {
                //cout << "step 4" << endl;
                Direction tempDirection = getDirection();
                // STEP #4: Check to see if protester is in shouting range
                if (getWorld()->isNearIceMan(this, 3) && getWorld()->facingTowardIceMan(this)) {
                    // Check position and direction of protester, annoy iceman
                    getWorld()->annoyIceMan(2);
                    getWorld()->playSound(SOUND_PROTESTER_YELL);
                    currentState = stunned;
                    break;
                }
                // STEP #5 line of sight to iceman
                //cout << "step 5" << endl;
                Direction icemanDirection = getWorld()->lineOfSightToIceMan(this);

                if ( icemanDirection != 0 ) {
                    setDirection(icemanDirection);
                    moveIfPossible();
                    ticksToNextMove = ticksToWaitBetweenMoves;
                    break;
                }
                //cout << "step 6" << endl;
                // STEP #6 decrement squares to move
                if (numSquaresToMoveInCurrentDirection > 0) {
                    numSquaresToMoveInCurrentDirection--;
                }
                //cout << "step 6.5" << endl;
                // STEP #6.5 check if squares to move is zero
                if (numSquaresToMoveInCurrentDirection <= 0) {
                    // If zero, find a new direction and reset squares to move
                    std::set<int> yay{getDirection()};
                    setDirection(findNewDirection(yay));
                    resetNumSquares();
                }
                // STEP #7 Check if the Regular Protester is at an intersection 
                // and hasn't made a perpendicular turn recently
                else if (turnTimer >= 200) {
                    //cout << "step 7" << endl;
                    auto check = isPerpendicular();
                    if (!check.empty()) {
                        setDirection(findNewDirection(check));
                    }
                }
                // STEP #8 attempt to move
                //cout << "step 8" << endl;
                if (!moveIfPossible()) {
                    numSquaresToMoveInCurrentDirection = 0;
                }
                
                //cout << "step 9" << endl;
                //check to see if it actually turned
                if ( (getDirection() - 2 <= 0 && tempDirection - 2 >= 1) || 
                    (getDirection() - 2 >= 1 && tempDirection - 2 <= 0)) {
                    turnTimer = 0;
                    resetNumSquares();
                }
                

                // Reset ticksToNextMove to default waiting time
                ticksToNextMove = ticksToWaitBetweenMoves;
                turnTimer++;
            }
        }
    }
    // If Protestor is not alive, function will return
}

void RegularProtester::addGold() {
    setState(leaving);
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
}
// Regular Protester End










// Boulder Begin
Boulder::Boulder(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, down, true, IID_BOULDER, 1.0, 1)
{
    setVisible(true);
    currentState = stable;

    getWorld()->clearIce(startX, startY);
}

bool Boulder::isIceBelow()
{
    StudentWorld* world = getWorld();
    int x = getX();
    int y = getY();

    if (world->hasIceAt(x, y - 2) ||
        world->hasIceAt(x - 1, y - 2) ||
        world->hasIceAt(x + 1, y - 2) ||
        world->hasIceAt(x + 2, y - 2)) {
        return true;
    }

    return false;
}

bool Boulder::isNearIceman() {
    return getWorld()->isNearIceMan(this, 3);
}

bool Boulder::isNearProtestor()
{
    return false;
}

void Boulder::annoyActors()
{
    getWorld()->annoyAllNearbyActors(this, 100, 3);
}

bool Boulder::canActorsPassThroughMe() const {
    return false;
}

void Boulder::move()
{
    if (Alive()) {
        switch (currentState) {
        case stable:
            if (!isIceBelow())
            {
                currentState = waiting;
                waitTime = 0;
            }

            break;

        case waiting:
            if (waitTime < 30)
            {
                waitTime++;
            }

            else
            {
                currentState = falling;
                getWorld()->playSound(SOUND_FALLING_ROCK);

            }
            break;
        case falling:
            getWorld()->annoyAllNearbyActors(this, 100, 3);
            if (!isIceBelow())
            {

                moveTo(getX(), getY() - 1);
                
            }
            else {
                currentState = dead;
            }
            break;
        case dead:
            setVisible(false);
            setDead();
            break;
        }


    }
}
//  Boulder End




// Activating Object

ActivatingObject::ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
    int soundToPlay, bool activateOnPlayer, bool activateOnProtester, bool initallyActive) 
    : Actor(world, startX, startY, right, true, imageID, 1, 2) 
{
    
}
void ActivatingObject::move() { 

}

// Set number of ticks until this object dies
void ActivatingObject::setTicksToLive() {
    lifespan = max(100, 300 - 10 * static_cast<int>(getWorld()->getLevel()));
}



GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY, bool temporary) : ActivatingObject(world, startX, startY, IID_GOLD, SOUND_GOT_GOODIE, true, true, false)
{
    setVisible(!temporary);
    
    
}

void GoldNugget::move()
{
    if(!Alive())
    {
        return;
    }
    else if (!isVisible()) {
        if (getWorld()->isNearIceMan(this, 4.0)) {
            setVisible(true);
            return;
        }
    }
    else if (getWorld()->isNearIceMan(this, 3)) {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
    }
    
    else if (!canPickThingsUp()){
        Actor* actor = getWorld()->findNearbyPickerUpper(this, 3);

        if (actor == nullptr) {
            setDead();
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
            getWorld()->increaseScore(25);

        }
        
        
    }
    
}





// Water Pool
WaterPool::WaterPool(StudentWorld* world, int startX, int startY) 
    : ActivatingObject(world, startX, startY, IID_WATER_POOL, SOUND_GOT_GOODIE, true, false, true)
{
    setVisible(true);
    setTicksToLive();
}
void WaterPool::move() {
    if (Alive()) {
        if (lifespan <= 0) {
            setVisible(false);
            setDead();
            return;
        }
        if (getWorld()->isNearIceMan(this, 3)) {
            getWorld()->giveIceManWater();
            setVisible(false);
            setDead();
        }
    }
}
// Water Pool End








// Oil Barrel
OilBarrel::OilBarrel(StudentWorld* world, int startX, int startY) : ActivatingObject(world, startX, startY, IID_BARREL, SOUND_FOUND_OIL, true, true, false)
{
    setVisible(false);

}

void OilBarrel::move()
{
    if (!Alive())
    {
        return;
    }
    else if (!isVisible()) {
        if (getWorld()->isNearIceMan(this, 4.0)) {
            setVisible(true);
            return;
        }
    }
    else if (getWorld()->isNearIceMan(this, 3) && !pickedUp) {
        setVisible(false);
        setDead();
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        pickedUp = true;

    }
}

bool OilBarrel::needsToBePickedUpToFinishLevel() const
{
    return pickedUp;
}
// Oil Barrel End