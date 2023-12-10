#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor : public GraphObject

{
private:
    StudentWorld* myWorld;
    bool isAlive;
public:
    //constructor
    //Actor(StudentWorld* world, int startX, int startY, Direction startDir,bool visible, int imageID, double size, int depth);
    Actor(StudentWorld* world, int startX, int startY, Direction dir, bool visible, int imageID, double size, int depth);

    //virtual destructor
    virtual ~Actor();

    //returns the value of isAlive
    bool Alive() const;

    //returns a pointer to myWorld
    StudentWorld* getWorld() const;

    // Action to perform each tick.
    virtual void move() = 0;

    // Mark this actor as dead.
    void setDead();

    // Annoy this actor.
    virtual bool annoy(unsigned int amt);

    // Can other actors pass through this actor?
    virtual bool canActorsPassThroughMe() const;

    // Can this actor dig through Ice?
    virtual bool canDigThroughIce() const;

    // Can this actor pick items up?
    virtual bool canPickThingsUp() const;

    // Does this actor hunt the IceMan?
    virtual bool huntsIceMan() const;

    // Can this actor need to be picked up to finish the level?
    virtual bool needsToBePickedUpToFinishLevel() const;

    // Move this actor to x,y if possible, and return true; otherwise,
    // return false without moving.
    bool moveToIfPossible(int x, int y);

};


class Agent : public Actor
{
private:

protected:
    unsigned int health;
public:

    Agent(StudentWorld* world, int startX, int startY, Direction startDir, 
        int imageID, unsigned int hitPoints);
   
    // Pick up a gold nugget.
    virtual void addGold() = 0;

    // How many hit points does this actor have left?
    unsigned int getHealth() const;

    // Decrement health -- Is Agent dead? If health drops to or below zero, return true
    virtual bool annoy(unsigned int amount);

    virtual bool canPickThingsUp() const;
};


class Iceman : public Agent
{
private:

public:
    Iceman(StudentWorld* world, int startX, int startY);
    void doSomething();

    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool canDigThroughIce() const;

    // Pick up a sonar kit.
    void addSonar();

    // Pick up water.
    void addWater();

    // Get amount of gold
    unsigned int getGold() const;

    // Get amount of sonar charges
    unsigned int getSonar() const;

    // Get amount of water
    unsigned int getWater() const;
};


//protestor parent class
class Protester : public Agent
{

protected:
    enum state { hunting, leaving, resting };
    state currentState;

    bool mustLeaveOilField;

    //ticks to determine move()
    int ticksToWaitBetweenMoves;
    int ticksToNextMove;

    int restingTicks;

    int goldAmount;

    int timeSinceLastTurn;

    int numSquaresToMoveInCurrentDirection;

public:
    Protester(StudentWorld* world, int startX, int startY, int imageID,
        unsigned int hitPoints);
    
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsIceMan() const;


    // Set state to having given up protest
    void setMustLeaveOilField();

    // Set number of ticks until next move
    void setTicksToNextMove();

    // Set numSquaresInCurrentDirection
    void resetNumSquares();

    // Set state
    void setState(state newState);

    // Get state
    state getState() const;

    // Check if there is ice at coordinates
    bool isThereIceAt(int x, int y, Direction dir);

    // Check to see if motion is possible, and move if possible
    bool checkMotion();

};


//Regular protestor class
class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();

};


//Hardcore protestor class
class HardcoreProtester : public Protester
{
public:
    HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();
};


//Iceblock class
class Ice : public Actor
{
private:
public:
    Ice(StudentWorld* world, int startX, int startY);
    virtual void move();

};

class Boulder : public Actor
{
private:
    enum State { waiting, stable, falling, dead };
    State currentState;
    int waitTime;
    bool isIceBelow();
    bool isNearProtestor();
    bool isNearIceman();
    void annoyActors();
public:
    Boulder(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool canActorsPassThroughMe() const;
};

class Squirt : public Actor
{
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction startDir);
    virtual void move();
};

class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
        int soundToPlay, bool activateOnPlayer,
        bool activateOnProtester, bool initallyActive);
    virtual void move();

    // Set number of ticks until this object dies
    void setTicksToLive();
};

class OilBarrel : public ActivatingObject
{
public:
    OilBarrel(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool needsToBePickedUpToFinishLevel() const;
};

class GoldNugget : public ActivatingObject
{
public:
    GoldNugget(StudentWorld* world, int startX, int startY, bool temporary);
    virtual void move();
};

class SonarKit : public ActivatingObject
{
public:
    SonarKit(StudentWorld* world, int startX, int startY);
    virtual void move();
};

class WaterPool : public ActivatingObject
{
public:
    WaterPool(StudentWorld* world, int startX, int startY);
    virtual void move();
};


#endif // ACTOR_H_

