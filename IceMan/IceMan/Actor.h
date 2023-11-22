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
    
};


class Iceman : public Actor
{
public: 
    Iceman(StudentWorld* world, int startX, int startY) : Actor(world, IID_PLAYER, startX, startY, right, 1)
    {
        setVisible(true);
        
    }
    
};
#endif // ACTOR_H_
