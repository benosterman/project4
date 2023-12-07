#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//#include "StudentWorld.h"
class StudentWorld;
class Actor : public GraphObject

{
private:
StudentWorld* myWorld;
bool isAlive;
public:
    
    Actor(StudentWorld* world, int imageID, double startX, double startY, Direction dir = right, float size = 1.0, unsigned int depth = 1) : GraphObject(imageID, startX, startY, right, size, depth), myWorld(world), isAlive(true) {}
    
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
    Iceman(StudentWorld* world, int startX, int startY) : Actor(world, IID_PLAYER, startX, startY, right, 1.0, 1)
    {
        
        setVisible(true);
        
    }
    void doSomething();
    std::pair<int, int> getIcemanLocation();
    void shootSquirt();
    
    
};

class Ice : public Actor
{
private: 
public:
    Ice(StudentWorld* world, int startX, int startY) : Actor(world, IID_ICE, startX, startY, right, 0.25, 3)
    {
        setVisible(true);
        
    }
    
    void disappear();
    
    
    
};

class Boulder : public Actor
{
private:
    enum State {waiting, stable, falling};
    State currentState;
    int waitTime;
    
    bool isIceBelow() const;
    bool canMoveDown() const;
    bool isNearProtestor() const;
    bool isNearIceman() const;
    void annoyActors();
public:
    Boulder(StudentWorld* world, int startX, int startY) : Actor(world, IID_BOULDER, startX, startY, down, 1, 1), currentState(waiting), waitTime(0)
    {
        setVisible(true);
        
    }
    virtual void move();
    virtual bool canActorsPassThroughMe() const;
    void doSomething();
    
};

//class Squirt : public Actor
//{
//private:
//    int travelDistance;
//    bool hitsIce() const;
//    bool hitsProtestor() const;
//    bool hitsBoulder() const;
//    
//public:
//    Squirt(StudentWorld* world, int startX, int startY, Direction dir) : Actor(world, IID_WATER_SPURT, startX, startY, dir, 1.0, 1)
//    {
//        setVisible(true);
//        
//        
//    }
//    void doSomething();
//    
//};






//
//class ActivatingObject : public Actor
//{
//private:
//public:
//    ActivatingObject(StudentWorld* world, int startX, int startY, int imageID, int soundToPlay, bool activateOnPlayer, bool activateOnProtestor, bool initiallyActive);
//    
//    
//    
//    
//    
//    virtual void move();
//    void setTicksToLive();
//    
//    
//};






#endif // ACTOR_H_
