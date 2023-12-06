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
    unsigned int health;
public:
    Agent(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, unsigned int hitPoints);
};


class Iceman : public Agent
{
private:

public:
    Iceman(StudentWorld* world, int startX, int startY);
    void doSomething();

};


class Ice : public Actor
{
private:
public:
    Ice(StudentWorld* world, int startX, int startY);

};
#endif // ACTOR_H_