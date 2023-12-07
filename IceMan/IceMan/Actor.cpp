#include "Actor.h"
#include "StudentWorld.h"
//#include "Iceman.h"


void Iceman::doSomething()
{
    if (!Alive())
        
    {
        return;
    }
//    getWorld()->removeIce();
    
    
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
                
                break;
                
        }
    }
}

//void Boulder::doSomething()
//{
//    if(!Alive())
//    {
//        return;
//    }
//    
//    switch (currentState) {
//        case stable:
//            if(!isIceBelow())
//            {
//                currentState = waiting;
//                waitTime = 30;
//                
//                
//            }
//            
//            break;
//            
//        case waiting:
//            if (waitTime > 0)
//            {
//                waitTime --;
//            }
//            
//        else
//        {
//            currentState = falling;
////            getWorld()->SOUND_FALLING_ROCK;
//            
//        }
//            break;
//        case falling:
//            if(canMoveDown())
//            {
//                moveTo(getX(), getY() -1);
//                if(isNearProtestor() || isNearIceman())
//                {
//                    annoyActors();
//                    
//                }
//            }
//            else
//            {
////                setDead();
//                
//            }
//            break;
//    }
//}

//void Squirt::doSomething()
//{
//    if (!Alive())
//    {
//        return;
//    }
//
//    if (hitsIce() || hitsProtestor() || hitsBoulder())
//    {
////        setDead();
//        return;
//    }
//
//    switch (getDirection())
//    {
//        case up:
//            moveTo(getX(), getY() + 1);
//            break;
//        case down:
//            moveTo(getX(), getY() - 1);
//            break;
//        case left:
//            moveTo(getX() - 1, getY());
//            break;
//        case right:
//            moveTo(getX() + 1, getY());
//            break;
//    }
//
//    travelDistance--;
//
//    if (travelDistance <= 0)
//    {
////            setDead();
//    }
//}

//void Iceman::shootSquirt()
//{
//    // Get current location and direction of Iceman
//    int x = getX();
//    int y = getY();
//    Direction dir = getDirection();
//
//    // Adjust the starting location of the Squirt based on Iceman's direction
//    switch (dir)
//    {
//        case up:
//            if (y + 4 < 60) y += 4; // Ensure Squirt doesn't go out of bounds
//            else return;
//            break;
//        case down:
//            if (y - 4 >= 0) y -= 4;
//            else return;
//            break;
//        case left:
//            if (x - 4 >= 0) x -= 4;
//            else return;
//            break;
//        case right:
//            if (x + 4 < 64) x += 4; // Assuming the field width is 64
//            else return;
//            break;
//    }
//
//    // Create a new Squirt object at the adjusted location, facing the same direction as Iceman
//    Squirt* newSquirt = new Squirt(getWorld(), x, y, dir);
//    // Add the new Squirt to the game world
////    getWorld()->addActor(newSquirt);
//}
