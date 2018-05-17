/* 
 * File:   Player.h
 * Author: yoo
 *
 * Created on December 20, 2014, 12:45 PM
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class Player : public cocos2d::Sprite
{
public:
    Player();
    //Player(const Player& orig);
    virtual ~Player();
    
    static Player* create();
    void initOptions();
    
    Rect getBoundingBoxDown();
    Rect getBoundingBoxUp();
    Rect getBoundingLanding();
    void attachCargo(Sprite* cargo);
    
    bool isCollided(Rect rect);
    Sprite* getCargo();
private:
      Sprite* _cargo;
};

#endif	/* PLAYER_H */

