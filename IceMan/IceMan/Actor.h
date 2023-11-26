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
    
    Actor(StudentWorld* world, int imageID, double startX, double startY, Direction dir = right, int depth = 0) : GraphObject(imageID, startX, startY), myWorld(world), isAlive(true) {}
    
    virtual ~Actor() {}
    bool Alive() const
    {
        return isAlive;
    }
    
    StudentWorld* getWorld() const
    {
        return myWorld;
    }
};


class Iceman : public Actor
{
private:

public: 
    Iceman(StudentWorld* world, int startX, int startY) : Actor(world, IID_PLAYER, startX, startY, right, 1)
    {
        setVisible(true);
        
    }
    void doSomething();
    
};

class Ice : public Actor
{
private:
public:
    Ice(StudentWorld* world, int startX, int startY) : Actor(world, IID_ICE, startX, startY, right, 0)
    {
        setVisible(true);
        
    }
    
};
#endif // ACTOR_H_
