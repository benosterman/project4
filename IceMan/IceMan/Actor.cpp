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


void Actor::doSomething()
{
    
}



//Ice Class start
Ice::Ice(StudentWorld* world, int startX, int startY)
    : Actor(world, startX, startY, right, true, IID_ICE, 0.25, 3) { }

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


Boulder::Boulder(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, right, true, IID_BOULDER, 1.0, 1)
{
    setVisible(true);
}

bool Boulder::isIceBelow()
{
    StudentWorld* world = getWorld();
       int x = getX();
       int y = getY();

    for (int i = 0; i < 10; ++i) {
            if (world->hasIceAt(x, y - i)) {
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
    cout << "Annoyeed";
    
}

bool Boulder::canMoveDown()
{
    StudentWorld* world = getWorld();
       int x = getX();
       int y = getY();

    for (int i = 3; i < 4; ++i) {
            if (world->hasIceAt(x, y - i)) {
                return true;
            }
        }
       return false;
}
bool Boulder::canActorsPassThroughMe() const {
    return false;
}

void Boulder::move()
{
    switch (currentState) {
        case stable:
            if(!isIceBelow())
            {
                currentState = waiting;
                waitTime = 0;
            }

            break;

        case waiting:
            if (waitTime > 30)
            {
                waitTime --;
            }

        else
        {
            currentState = falling;
//            getWorld()->SOUND_FALLING_ROCK;

        }
        break;
        case falling:
            if(!isIceBelow())
            {
                
                moveTo(getX(), getY() -1);
                if(isNearProtestor() || isNearIceman())
                {
                    annoyActors();

                }
            }
            break;
    }
}

void Boulder::doSomething()
{
    if (!Alive())
    {
        return;
    }
    move();
    
}
//Squirt Class
Squirt::Squirt(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, right, true, IID_WATER_SPURT, 1.0, 1)
{
    setVisible(true);
    
    
}

void Squirt::move()
{
    moveTo(getX(), getY() - 1);
    
}

void Squirt::doSomething()
{
    if (!Alive()) return;
    else move();
}





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
                Squirt* squirt = new Squirt(getWorld(), getX(), getY());
                getWorld()->addActor(squirt);
                squirt->doSomething();
                break;

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
