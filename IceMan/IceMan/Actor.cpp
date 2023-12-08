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

            break;

        }
    }
}


void Iceman::move() {

}
bool Iceman::annoy(unsigned int amount) {
    return false;
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
        Direction::left, imageID, hitPoints) 
{
    mustLeaveOilField = false;
    ticksToWaitBetweenMoves = max(0, 3 - static_cast<int>(getWorld()->getLevel()) / 4);
    ticksToNextMove = ticksToWaitBetweenMoves;

}

void Protester::move() {
    if (Alive()) {
        if (ticksToNextMove != 0) {
            setTicksToNextMove();
        }
        else {
            //Iceman* iceman = getWorld()->();
            if (mustLeaveOilField) {
                //leave oil field
                
            }
            else if (true) {
                // Check position and direction of protester, annoy iceman
                
            }
        }
    }
    // If Protestor is not alive, function will return
}

//private helper function
bool Protester::leaveOilField() {
    queue<Ice*> searchQueue;



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