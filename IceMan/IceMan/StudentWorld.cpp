#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <queue>
#include <utility>
#include <iostream>
#include <random>
#include <exception>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir) { }

StudentWorld::~StudentWorld() {

}

int StudentWorld::init()
{
    //create Iceman
    int initialX = 30;
    int initialY = 60;
    myIceman = new Iceman(this, initialX + 1, initialY + 1);

    //create oil field
    for (int x = 0; x < maxIceWidth; ++x) {
        for (int y = 0; y < 64; ++y) {
            //std::unique_ptr<Ice> ice = std::make_unique<Ice>(this, x + 1, y + 1);
            if (y >= 60) {
                oilField[x][y] = new Ice(this, x + 1, y + 1);
                oilField[x][y]->setVisible(false);
                oilField[x][y]->setDead();
            }
            else {
                oilField[x][y] = new Ice(this, x + 1, y + 1);
            }
        }
    }

    //clear starting columns in oil field
    for (int x = 30; x < 34; x++) {
        for (int y = 4; y < 60; y++) {
            oilField[x][y]->setVisible(false);
            oilField[x][y]->setDead();
        }

    }

    //create Boulders
    Boulder* boulder = new Boulder(this, 15, 40);
    boulder->move();
    addActor(boulder);

    //create Gold
    GoldNugget* gold = new GoldNugget(this, 15, 33, true);
    gold->move();
    addActor(gold);

    //create Oil Barrels
    OilBarrel* barrel = new OilBarrel(this, 45, 38);
    barrel->move();
    addActor(barrel);


    //set protester numbers
    ticksBeforeProtester = std::max(25, 0);
    numTargetProtesters = std::min(15, 2 + static_cast<int>(getLevel()));
    numProtesters = 0;
    numTicks = 0;


    setGameStatText(getDisplayText());

    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    numTicks++;
    
    // create sonar or water pool
    std::random_device rd;
    std::mt19937 gen(rd());
    const int totalOutcomes = static_cast<int>(getLevel()) * 25 + 300;

    std::uniform_int_distribution<> G(1, totalOutcomes);
    int randomNumber = G(gen);

    if (randomNumber == 1) {
        std::uniform_int_distribution<> N(1, 5);
        int chance = N(gen);
        if (chance != 1) {
            // water pool 
            int findX = 31;
            int findY = 61;
            std::uniform_int_distribution<> Y(7, 61);
            findY = Y(gen);
            WaterPool* water = new WaterPool(this, findX, findY);
            addActor(water);
        }
        else {
            // sonar

        }
    }
    

    // call move on everything
    for (int i = 0; i < actors.size(); i++) {
        Actor* actor = actors[i];
        if (actor->Alive()) {
            //myIceman->doSomething();
            actor->move();
            if (!myIceman->Alive()) {

                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
        else {
            actors.erase(actors.begin() + i);
            i--;
            inactive.push(actor);
            //actor->moveTo(-10, -10);
        }
    }

    //iceman stuff
    myIceman->doSomething();

    // clear ice
    int icemanX = myIceman->getX();
    int icemanY = myIceman->getY();

    clearIce(icemanX, icemanY);
    if (!myIceman->Alive()) {

        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    //create new protester
    if (numTicks == ticksBeforeProtester && numProtesters < numTargetProtesters) {

        RegularProtester* p = new RegularProtester(this, 61, 61, IID_PROTESTER);

        addActor(p);
    }

    setGameStatText(getDisplayText());

    while (!inactive.empty()) {
        delete inactive.front();
        inactive.pop();
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {

    while (!inactive.empty()) {
        delete inactive.front();
        inactive.pop();
    }

    while (!actors.empty()) {
        delete actors.back();
        actors.pop_back();
    }

    delete myIceman;

    //delete oilField
    for (int i = 0; i < maxIceWidth; ++i) {
        for (int j = 0; j < maxIceHeight; ++j) {
            //std::unique_ptr<Ice>& ice = oilField[i][j];
            delete oilField[i][j];
                
                //oilField[i][j] = std::move(ice); // Reset each unique pointer

        }
    }


}

// Add an actor to the world.
void StudentWorld::addActor(Actor* a) {
    actors.push_back(a);
}

// Clear a 4x4 region of Ice.
void StudentWorld::clearIce(int x, int y) {
    // Define the range for clearing ice (4x4 area around object)
    int leftBound = std::max(x - 1, 0);
    int rightBound = std::min(x + 2, maxIceWidth - 1);
    int lowerBound = std::max(y - 1, 0);
    int upperBound = std::min(y + 2, maxIceHeight - 1);

    // Loop through the 4x4 area and clear ice
    for (int x = leftBound; x <= rightBound; x++) {
        for (int y = lowerBound; y <= upperBound; y++) {
            Ice* current = oilField[x][y];
            if (current != nullptr && current->Alive() == true) {
                oilField[x][y]->setVisible(false);
                oilField[x][y]->setDead();
                playSound(SOUND_DIG);
            }
            
        }
    }

}

// Can actor move to x,y?
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const {
    try {
        if (x >= 1 && x < 62 && y >= 1 && y < 62) {
            for (auto& currentActor : actors) {
                if (currentActor->canActorsPassThroughMe() == false) {

                    // Check if the entities are touching in the x-axis
                    bool xTouching = (currentActor->getX() <= x + 3) && (x <= currentActor->getX() + 3);

                    // Check if the entities are touching in the y-axis
                    bool yTouching = (currentActor->getY() <= y + 3) && (y <= currentActor->getY() + 3);

                    // If both x and y touching, then the entities are touching
                    return !(xTouching && yTouching);

                }
            }
        }
        else {
            return false;
        }
        return true;
    }
    catch (exception e) {
        cerr << e.what() << endl;
        throw e;
    }
}

// Annoy all other actors within radius of annoyer, returning the
// number of actors annoyed.
int StudentWorld::annoyAllNearbyActors(Actor* annoyer, int points, int radius) {
    
    // Get annoyer's current location
    int objX = annoyer->getX();
    int objY = annoyer->getY();
  
    int ret = 0;
    for (auto& currentActor : actors) {

            // Check if the entities are touching in the x-axis
            bool xTouching = (currentActor->getX() <= objX + radius) && (objX <= currentActor->getX() + radius);

            // Check if the entities are touching in the y-axis
            bool yTouching = (currentActor->getY() <= objY + radius) && (objY <= currentActor->getY() + radius);

            // If both x and y touching, then the entities are touching
            if (xTouching && yTouching) {
                ret++;
                currentActor->annoy(static_cast<unsigned int>(points));
            }
        
    }

    return ret;
}

// Reveal all objects within radius of x,y.
void StudentWorld::revealAllNearbyObjects(int x, int y, int radius) {

}

// If the IceMan is within radius of a, return a pointer to the
// IceMan, otherwise null.
Actor* StudentWorld::findNearbyIceMan(Actor* a, int radius) const {
    return a;
}

// If at least one actor that can pick things up is within radius of a,
// return a pointer to one of them, otherwise null.
Actor* StudentWorld::findNearbyPickerUpper(Actor* a, int radius) const {
    return a;
}

// Annoy the IceMan.
void StudentWorld::annoyIceMan(unsigned int amount) {
    // Annoy checks the health of Iceman, if it returns true, iceman has died
    playSound(SOUND_PLAYER_ANNOYED);

    if (myIceman->annoy(amount)) {
        // Set state of Alive to false
        myIceman->setDead();
    }
}

// Give IceMan some sonar charges.
void StudentWorld::giveIceManSonar() {

}

// Give IceMan some water.
void StudentWorld::giveIceManWater() {
    myIceman->addWater();
}

// Is the Actor a facing toward the IceMan?
bool StudentWorld::facingTowardIceMan(Actor* a) const {
    // Retrieve Iceman's current position
    int icemanX = myIceman->getX();
    int icemanY = myIceman->getY();

    // Get Actor's current location and direction
    int objX = a->getX();
    int objY = a->getY();
    GraphObject::Direction dir = a->getDirection();

    // Compare
    bool ret = false;
    if (dir == GraphObject::right && objX < icemanX && icemanY < objY + 4 * a->getSize() && icemanY >= objY - 3 * a->getSize()) {
        ret = true;
    }
    else if (dir == GraphObject::left && objX > icemanX && icemanY < objY + 4 * a->getSize() && icemanY >= objY - 3 * a->getSize()) {
        ret = true;
    }
    else if (dir == GraphObject::up && objY < icemanY && icemanX >= objX - 3 * a->getSize() && icemanX < objX + 4 * a->getSize()) {
        ret = true;
    }
    else if (dir == GraphObject::down && objY > icemanY && icemanX >= objX - 3 * a->getSize() && icemanX < objX + 4 * a->getSize()) {
        ret = true;
    }

    return ret;
}

// If the Actor a has a clear line of sight to the IceMan, return
// the direction to the IceMan, otherwise GraphObject::none.
GraphObject::Direction StudentWorld::lineOfSightToIceMan(Actor* a) const {
    int objX = a->getX();
    int objY = a->getY();

    int icemanX = myIceman->getX();
    int icemanY = myIceman->getY();

    // iceman to the right of object
    if (objX < icemanX && icemanY == objY) {
        bool isClear = true;
        for (int x = objX; x < icemanX; x++) {
            for (int y = objY - 1; y <= objY + 2; y++) {
                if (hasIceAt(x, y)) {
                    isClear = false;
                }
            }
        }
        if (isClear) {
            return GraphObject::Direction::right;
        }
        else {
            return GraphObject::Direction::none;
        }
    }
    // iceman to the left of object
    if (objX > icemanX && icemanY == objY) {
        bool isClear = true;
        for (int x = objX; x > icemanX; x--) {
            for (int y = objY - 1; y <= objY + 2; y++) {
                if (hasIceAt(x, y)) {
                    isClear = false;
                }
            }
        }
        if (isClear) {
            return GraphObject::Direction::left;
        }
        else {
            return GraphObject::Direction::none;
        }
    }
    // iceman above object
    if (objY < icemanY && icemanX == objX) {
        bool isClear = true;
        for (int x = objX - 1; x <= objX + 2; x++) {
            for (int y = objY; y < icemanY; y++) {
                if (hasIceAt(x, y)) {
                    isClear = false;
                }
            }
        }
        if (isClear) {
            return GraphObject::Direction::up;
        }
        else {
            return GraphObject::Direction::none;
        }
    }
    // iceman below
    if (icemanY < objY && icemanX == objX) {
        bool isClear = true;
        for (int x = objX - 1; x <= objX + 2; x++) {
            for (int y = objY; y > icemanY; y--) {
                if (hasIceAt(x, y)) {
                    isClear = false;
                }
            }
        }
        if (isClear) {
            return GraphObject::Direction::down;
        }
        else {
            return GraphObject::Direction::none;
        }
    }

    return GraphObject::Direction::none;
}

// Return whether the Actor a is within radius of IceMan. (Set radius to 3 when calling)
bool StudentWorld::isNearIceMan(Actor* a, int radius) const {


    // Check if the entities are touching in the x-axis
    bool xTouching = (myIceman->getX() <= a->getX() + radius) && (a->getX() <= myIceman->getX() + radius);

    // Check if the entities are touching in the y-axis
    bool yTouching = (myIceman->getY() <= a->getY() + radius) && (a->getY() <= myIceman->getY() + radius);

    // If both x and y touching, then the entities are touching
    return (xTouching && yTouching);

}


// Determine the direction of the first move a hardcore protester
// makes to approach the IceMan.
GraphObject::Direction StudentWorld::determineFirstMoveToIceMan(int x, int y) {
    return GraphObject::Direction::right;
}


bool StudentWorld::hasIceAt(int x, int y) const {
    // Check if the coordinates are within the bounds of the oil field
    if (x < 0 || x > 64 || y < 0 || y > 64) {
        return false;
    }

    // Check if there is ice at the specified location
    return oilField[x][y]->Alive() == true;

}


// Helper Function that sets the text at the top of the screen
std::string StudentWorld::getDisplayText() const {
    int level = static_cast<int>(getLevel());
    int lives = static_cast<int>(getLives());
    int health = static_cast<int>(myIceman->getHealth()) * 10;
    int water = static_cast<int>(myIceman->getWater());
    int score = static_cast<int>(getScore());


    string ret = "Lvl: " + std::to_string(level)
        + " Lives: " + to_string(lives) + " Health : " + to_string(health) + "% Wtr : " + to_string(water)
        + "Gld : 5 Oil Left : 2 Sonar : 1 Scr : " + to_string(score);

    return ret;
}

// closest so far
GraphObject::Direction StudentWorld::determineFirstMoveToExit(int start_x, int start_y) {
   /* cout << "Starting at (" << start_x << ", " << start_y << ")" << endl;
    cout << "index: [" << start_y - 1 << "][" << start_x - 1 << "]" << endl;
    
    std::queue<std::pair<Ice*, int>> q;  // Pair: Ice pointer and iteration
    Ice* guess;
    int visited[64][64] = { 0 };

    q.push({ oilField[start_x - 1][start_y - 1], 1 });
    visited[start_y - 1][start_x - 1] = 1;

    cout << endl;

    while (!q.empty()) {
        guess = q.front().first;
        int iteration = q.front().second;
        q.pop();

        if (guess == nullptr) {
            //cout << " no way right? " << endl;
            if (guess->Alive()) {
                //cout << "nawwww" << endl;
                continue;
            }
            continue;
        }
        

        cout << "Checking (" << guess->getX() << ", " << guess->getY() << ") with iteration " << iteration << endl;
        int xindex = guess->getX() - 1;
        int yindex = guess->getY() - 1;


        cout << "( Current index: [" << xindex << "][" << yindex << "] )" << endl;

         
        // Check if this is an exit (60, 60 is the top-right corner)
        if (guess->getX() == 61 && guess->getY() == 61) {
            std::cout << "Exit found!" << endl << endl;
            // Backtrace to find the shortest path
            return backtraceShortestPath(start_x - 1, start_y - 1, visited);
        }

        int x = guess->getX();
        int y = guess->getY();

        if (y <= 64 && visited[yindex + 1][xindex] == 0) {  // Up
            cout << "Pushing (" << x << ", " << y + 1 << ") with iteration " << iteration + 1 << endl;
            q.push({ oilField[yindex + 1][xindex], iteration + 1 });
            visited[yindex + 1][xindex] = iteration + 1;

            
            //cout << "q value just pushed x:" << q.front().first->getX() << endl;


            if (x == 61 && y + 1 == 61) {
                std::cout << "Exit found!" << endl << endl;
                // Backtrace to find the shortest path
                return backtraceShortestPath(start_x - 1, start_y - 1, visited);
            }
        }
        if (guess->getX() <= 64 && visited[yindex][xindex + 1] == 0) {  // Right
            cout << "Pushing (" << x + 1 << ", " << y << ") with iteration " << iteration + 1 << endl;
            q.push({ oilField[yindex][xindex + 1], iteration + 1 });
            visited[yindex][xindex + 1] = iteration + 1;
            //cout << "Queue size: " << q.size() << endl;

            if (x + 1 == 61 && y == 61) {
                std::cout << "Exit found!" << endl << endl;
                // Backtrace to find the shortest path
                return backtraceShortestPath(start_x - 1, start_y - 1, visited);
            }
        }
        if (y > 0 && visited[yindex - 1][xindex] == 0) {  // Down
            cout << "Pushing (" << x << ", " << y - 1 << ") with iteration " << iteration + 1 << endl;
            q.push({ oilField[yindex - 1][xindex], iteration + 1 });
            visited[yindex - 1][xindex] = iteration + 1;

            if (x == 61 && y - 1 == 61) {
                std::cout << "Exit found!" << endl << endl;
                // Backtrace to find the shortest path
                return backtraceShortestPath(start_x - 1, start_y - 1, visited);
            }
        }

        if (guess->getX() > 0 && visited[yindex][xindex - 1] == 0) {  // Left
            cout << "Pushing (" << x - 1 << ", " << y << ") with iteration " << iteration + 1 << endl;
            q.push({ oilField[yindex][xindex - 1], iteration + 1 });
            visited[yindex][xindex - 1] = iteration + 1;

            if (x - 1 == 61 && y == 61) {
                std::cout << "Exit found!" << endl << endl;
                // Backtrace to find the shortest path
                return backtraceShortestPath(start_x - 1, start_y - 1, visited);
            }
        }

        
        
        //
        cout << endl;
    }*/
    return GraphObject::Direction::none; // No valid path to an exit found
}


GraphObject::Direction StudentWorld::backtraceShortestPath(int origin_x, int origin_y, int visited[64][64]) {
    /*
    cout << "made it to backtrace" << endl;
     
    cout << "origin index x: " << origin_y << "  y: " << origin_x << endl;
    // Start at the exit (index)
    int current_x = 60; // exit x
    int current_y = 60; // exit y

    cout << "origin values x: " << oilField[origin_x][origin_y]->getX() << "  y: " << oilField[origin_x][origin_y]->getY() << endl;

    cout << "iteration value at exit: " << visited[current_y][current_x] << endl;

    //last direction the index shifted (index shifted to the ____)
    GraphObject::Direction lastMovement = GraphObject::Direction::none;
    auto count = 0;
    
    int lastX = 0;
    int lastY = 0;

    while (true) {
        count++;
        if (count > 20) break;
        if (lastX == current_x && lastY == current_y) {
            cout << "come on now blud" << endl;
            break;
        }
        // get the neighbor's iteration
        cout << "current index x: " << current_y << "  y: " << current_x << "  current iteration value: "<< visited[current_y][current_x] << endl;
        /*
         // uhhh
        int up = (current_y + 1 < 64 && visited[current_y][current_x - 1] != 0)
            ? visited[current_y + 1][current_x] : 70;
        int down = (current_y - 1 > 0 && visited[current_y][current_x + 1] != 0)
            ? visited[current_y - 1][current_x] : 70;
        int left = (current_x - 1 > 0 && visited[current_y + 1][current_x] != 0)
            ? visited[current_y][current_x - 1] : 70;
        int right = (current_x + 1 < 64 && visited[current_y - 1][current_x] != 0)
            ? visited[current_y][current_x + 1] : 70;
        
        // close
        int up = (current_y + 1 < 64 && visited[current_y + 1][current_x] != 0) 
            ? visited[current_y + 1][current_x] : 70;
        int down = (current_y - 1 > 0 && visited[current_y][current_x] != 0) 
            ? visited[current_y - 1][current_x] : 70;
        int left = (current_x - 1 > 0 && visited[current_y][current_x - 1] != 0) 
            ? visited[current_y][current_x - 1] : 70;
        int right = (current_x + 1 < 64 && visited[current_y][current_x + 1] != 0) 
            ? visited[current_y][current_x + 1] : 70;
        */

    /*
    //working
        int up = (current_y + 1 < 64 && visited[current_y + 1][current_x] > 0)
            ? visited[current_y + 1][current_x] : 70;
        int down = (current_y - 1 > 0 && visited[current_y - 1][current_x] > 0)
            ? visited[current_y - 1][current_x] : 70;
        int left = (current_x - 1 > 0 && visited[current_y][current_x - 1] > 0)
            ? visited[current_y][current_x - 1] : 70;
        int right = (current_x + 1 < 64 && visited[current_y][current_x + 1] > 0)
            ? visited[current_y][current_x + 1] : 70;

        cout << "up: " << visited[current_y + 1][current_x] << " evaluates to " << up << endl;
        cout << "down: " << visited[current_y - 1][current_x] << " evaluates to "<< down << endl;
        cout << "left: " << visited[current_y][current_x - 1] << " evaluates to " << left << endl;
        cout << "right: " << visited[current_y][current_x + 1]<< " evaluates to "<< right << endl;
        
        // Find the minimum value among neighbors
        int minNeighbor = std::min({ up, down, left, right });

        // Move to the neighbor with the smallest iteration
        if (up == minNeighbor) {
            lastY = current_y;
            lastX = current_x;
            current_y++;
            //lastMovement = GraphObject::Direction::up;

            if (current_x == origin_x && current_y == origin_y) {
                std::cout << "Reached starting position\n";
                break;
            }
        }
        else if (down == minNeighbor) {
            lastY = current_y;
            lastX = current_x;
            current_y--;
            //lastMovement = GraphObject::Direction::down;

            if (current_x == origin_x && current_y == origin_y) {
                std::cout << "Reached starting position\n";
                break;
            }
        }
        else if (left == minNeighbor) {
            lastY = current_y;
            lastX = current_x;
            current_x--;
            //lastMovement = GraphObject::Direction::left;

            if (current_x == origin_x && current_y == origin_y) {
                std::cout << "Reached starting position\n";
                break;
            }
        }
        else if (right == minNeighbor) {
            lastY = current_y;
            lastX = current_x;
            current_x++;
            //lastMovement = GraphObject::Direction::right;

            if (current_x == origin_x && current_y == origin_y) {
                std::cout << "Reached starting position\n";
                break;
            }
        }

    }
    //last thing i did, switch opposites
    if (lastY > current_y && lastX == current_x) {
        return GraphObject::Direction::down;
    }
    else if (lastY < current_y && lastX == current_x) {
        return GraphObject::Direction::up;
    }
    else if (lastX > current_x && lastY == current_y) {
        return GraphObject::Direction::right;
    }
    else if (lastX < current_x && lastY == current_y) {
        return GraphObject::Direction::left;
    }

    /*
    // Determine the direction to move from the starting position
    if (lastMovement == GraphObject::Direction::left) {
        return GraphObject::Direction::right;
    }
    else if (lastMovement == GraphObject::Direction::right) {
        return GraphObject::Direction::left;
    }
    else if (lastMovement == GraphObject::Direction::up) {
        return GraphObject::Direction::down;
    }
    else if (lastMovement == GraphObject::Direction::down) {
        return GraphObject::Direction::up;
    }*/

    return GraphObject::Direction::none;  // Should not reach here
}


bool StudentWorld::isThereIceInSquare(int x, int y) {
    for (int r = x - 1; r < x + 3; r++) {
        for (int c = y - 1; c < y + 3; c++) {

            // If in bounds, check if there is ice at coordinates
            if (r >= 0 && r <= 63 && c >= 1 && c <= 59) {
                if (hasIceAt(r, c)) {
                    return true;
                }
            }

        }
    }
    return false;
}

