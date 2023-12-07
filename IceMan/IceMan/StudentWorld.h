#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <memory>
using namespace std;


class StudentWorld : public GameWorld
{
private:
    
    std::vector<std::unique_ptr<Actor>> actors;
    Iceman* myIceman;
    int maxWidth = 63;
    int maxHeight = 60;
public:
    StudentWorld(std::string assetDir)
        : GameWorld(assetDir)
    {
    }

    virtual int init() override
    {
        int initialX = 30;
        int initialY = 60;
        
        myIceman = new Iceman(this, initialX, initialY);
        for (int x = 0; x < maxWidth; ++x) {
                for (int y = 0; y < maxHeight; ++y) {
                    std::unique_ptr<Ice> ice = std::make_unique<Ice>(this,x,y);
                    
                    
                    actors.push_back(std::move(ice));
                }
            }
        return GWSTATUS_CONTINUE_GAME;
    }

    virtual int move() override
    {
        for (auto& actor : actors) {
            if (actor->Alive()) {
                myIceman->doSomething();
                
                if (!myIceman->Alive()) {
                    
                    decLives();
                    return GWSTATUS_PLAYER_DIED;
                }
            }
        }
        
        myIceman->doSomething();
        removeIce(myIceman->getX(), myIceman->getY());
        
        
        if (!myIceman->Alive()) {
            
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
    void cleanUp()
    {
    }
    
    void removeIce(int x, int y);
    
        
};

    


#endif // STUDENTWORLD_H_
