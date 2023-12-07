#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
    
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp



void StudentWorld::removeIce(int x, int y)
{
    
//    std::pair<int, int> IcemanLocation = myIceman->getIcemanLocation();
    x = myIceman->getX();
    y = myIceman->getY();
    
    
    for (auto& actor : actors) {
        Ice* ice = dynamic_cast<Ice*>(actor.get());  //Check if the actor is an Ice object
        
            int iceX = ice->getX();
            int iceY = ice->getY();
            
            if (iceX == x && iceY == y) {
                ice->setVisible(false);
            }
            
            
        
    }
}
