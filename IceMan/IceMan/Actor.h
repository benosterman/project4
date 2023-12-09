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
    virtual void doSomething();
//    virtual void move();
    
    //returns a pointer to myWorld
    StudentWorld* getWorld() const;

};


class Agent : public Actor
{
private:
    unsigned int health;
public:
    Agent(StudentWorld* world, int startX, int startY, Direction startDir,
        int imageID, unsigned int hitPoints);
    
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsIceMan() const;
    unsigned int getHealth() const;

    // Set state to having gien up protest
    void setMustLeaveOilField();

    // Set number of ticks until next move
    void setTicksToNextMove();
};


class Iceman : public Agent
{
private:

public:
    Iceman(StudentWorld* world, int startX, int startY);
    void doSomething();

};


//protestor parent class
class Protester : public Agent
{
public:
    Protester(StudentWorld* world, int startX, int startY, int imageID,
        unsigned int hitPoints, unsigned int score);
    /*
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsIceMan() const;

    // Set state to having gien up protest
    void setMustLeaveOilField();

    // Set number of ticks until next move
    void setTicksToNextMove(); */
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
    enum State {waiting, stable, falling};
    State currentState;
    int waitTime;
    bool isIceBelow();
    bool canMoveDown();
    bool isNearProtestor();
    bool isNearIceman();
    void annoyActors();
public:
    Boulder(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual void move();
    virtual bool canActorsPassThroughMe() const;
};

class Squirt : public Actor
{
private:
public:
    Squirt(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual void doSomething();
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
