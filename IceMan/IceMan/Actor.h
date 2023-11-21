#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : GraphObject {
public:
    Actor(int ID, int x, int y, Direction dir, float size, unsigned int depth) : GraphObject(ID, x, y, dir, size, depth){}
    void virtual doSomething() = 0;
};


#endif // ACTOR_H_
