#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <vector>
#include <string>
using namespace std;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
	private:
std::vector<std::unique_ptr<Actor>> actors;
Iceman* myIceman;
int maxWidth = 63;
int maxHeight = 59;
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	virtual int init()
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
        if (!myIceman->Alive()) {
            
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        
        
        return GWSTATUS_CONTINUE_GAME;
    }

	virtual void cleanUp()
	{
	}


};

#endif // STUDENTWORLD_H_
