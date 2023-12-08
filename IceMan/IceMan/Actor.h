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

    // 
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
private:
    bool mustLeaveOilField;
    int ticksToWaitBetweenMoves;
    int ticksToNextMove;
    int goldAmount;
    bool leaveOilField();
public:
    Protester(StudentWorld* world, int startX, int startY, int imageID,
        unsigned int hitPoints, unsigned int score);
    
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsIceMan() const;

    // Set state to having gien up protest
    void setMustLeaveOilField();

    // Set number of ticks until next move
    void setTicksToNextMove();
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