#include "Actor.h"
#include "StudentWorld.h"
//#include "Iceman.h"


void Iceman::doSomething()
{
    if (!Alive())
        
    {
        return;
    }
    
    
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
