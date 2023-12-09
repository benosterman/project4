#include "Actor.h"
#include "StudentWorld.h"
#include <queue>

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

    return false;
}
// Actor Class End



//Ice Class start
Ice::Ice(StudentWorld* world, int startX, int startY)
    : Actor(world, startX, startY, right, true, IID_ICE, 0.25, 3) { }

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
Squirt::Squirt(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, right, true, IID_WATER_SPURT, 1.0, 1)
{
    setVisible(true);
    
    
}

void Squirt::move()
{
    moveTo(getX(), getY() - 1);
    
}


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
            if (getX() > 0)
                moveTo(getX() - 1, getY());
            break;
        case KEY_PRESS_RIGHT:
            if (getX() < VIEW_WIDTH - 1)
                moveTo(getX() + 1, getY());
            break;
        case KEY_PRESS_UP:

            if (getY() < VIEW_HEIGHT - 1)
                moveTo(getX(), getY() + 1);
            break;
        case KEY_PRESS_DOWN:

            if (getY() > 0)
                moveTo(getX(), getY() - 1);
            break;
        case KEY_PRESS_SPACE:
            Squirt* squirt = new Squirt(getWorld(), getX(), getY());
            getWorld()->addActor(squirt);
            squirt->move();
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
        return false;
    }
    else {
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
    return 0;
}
// Iceman Class End


// Protestor Class (parent class)
Protester::Protester(StudentWorld* world, int startX, int startY, int imageID,
    unsigned int hitPoints, unsigned int score) : Agent(world, startX, startY,
        Direction::right, imageID, hitPoints)
{
    mustLeaveOilField = false;
    ticksToWaitBetweenMoves = max(0, 3 - static_cast<int>(getWorld()->getLevel()) / 4);
    ticksToNextMove = ticksToWaitBetweenMoves;
    numSquaresToMoveInCurrentDirection = min(64, static_cast<int>(getWorld()->getScore())%31);
}

void Protester::move() {
    if (Alive()) {
        if (ticksToNextMove != 0) {
            setTicksToNextMove();
        }
        else {
            if (mustLeaveOilField) {
                //leave oil field
                
            }
            else if (getWorld()->isNearIceMan(this, 2) && getWorld()->facingTowardIceMan(this)) {
                // Check position and direction of protester, annoy iceman
                getWorld()->annoyIceMan(2);
            }
            
            //take a step in direction it is facing
            if (getDirection() == right) {
                if (moveToIfPossible(getX() + 1, getY())) {

                }
            }
            else if (getDirection() == left) {
                if (moveToIfPossible(getX() - 1, getY())) {

                }
            }
            else if (getDirection() == up) {
                if (moveToIfPossible(getX(), getY() + 1)) {

                }
            }
            else if (getDirection() == down) {
                if (moveToIfPossible(getX(), getY() - 1)) {

                }
            }
            
            
            // Reset ticks until next move
            ticksToNextMove = ticksToWaitBetweenMoves;
        }
    }
    // If Protestor is not alive, function will return
}

//private helper function
bool Protester::leaveOilField() {
    queue<Ice*> searchQueue;


    return false;
}

// Decrement health -- Is Agent dead? If health drops to or below zero, return true
bool Protester::annoy(unsigned int amount) {
    health -= amount;
    if (getHealth() > 0) {
        return false;
    }
    else {
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
// Protester Class End


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

    for (int i = 0; i < 10; ++i) {
        if (world->hasIceAt(x, y - 2) ||
            world->hasIceAt(x-1, y - 2) ||
            world->hasIceAt(x+1, y - 2) ||
            world->hasIceAt(x+2, y - 2)) {
            return true;
        }
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
    getWorld()->annoyAllNearbyActors(this, 100, 1);
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
            if (!isIceBelow())
            {

                moveTo(getX(), getY() - 1);
                if (isNearIceman() && getWorld()->facingTowardIceMan(this)) {
                    getWorld()->annoyIceMan(100);
                }
                /*
                if (isNearProtestor() || isNearIceman())
                {
                    annoyActors();

                }*/
            }
            else {
                currentState = dead;
            }
            break;
        case dead:
            setVisible(false);
            break;
        }
        

    }
}
