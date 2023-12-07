#include "Actor.h"
#include "StudentWorld.h"
//#include "Iceman.h"

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
Ice::Ice(StudentWorld* world, int startX, int startY): Actor(world, startX, startY, right, true, IID_ICE, 0.25, 3) { }

void Ice::move() {

}
//Agent Class
Agent::Agent(StudentWorld* world, int startX, int startY, Direction startDir,
    int imageID, unsigned int hitPoints) : Actor(world, startX, startY,
        startDir, true, imageID, 1.0, 0)
{
    health = hitPoints;
}
void Agent::move() {

}

bool Agent::annoy(unsigned int amount) {
    return false;
}

void Agent::addGold() {

}

bool Agent::huntsIceMan() const {
    return false;
}

unsigned int Agent::getHealth() const {
    return health;
}

// Set state to having gien up protest
void Agent::setMustLeaveOilField() {

}

// Set number of ticks until next move
void Agent::setTicksToNextMove() {

}
//Agent Class end

//Iceman Class
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
//Iceman Class end


//Protestor class (parent class)
Protester::Protester(StudentWorld* world, int startX, int startY, int imageID,
    unsigned int hitPoints, unsigned int score) : Agent(world, startX, startY,
        Direction::left, imageID, hitPoints)
{


}
//Protestor class end
