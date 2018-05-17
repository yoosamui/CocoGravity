/* 
 * File:   Player.cpp
 * Author: yoo
 * 
 * Created on December 20, 2014, 12:45 PM
 */

#include "Player.h"

Player::Player() {
}

//Player::Player(const Player& orig) {
//}

Player::~Player() {
}


void Player::attachCargo(Sprite* cargo)
{
    _cargo = cargo;
}

Sprite* Player::getCargo()
{
    return _cargo;
}

Player* Player::create()
{
    
    
    Player* pSprite = new Player();
    
    if (pSprite->initWithFile("globo.png"))
    {
    
        pSprite->_cargo = nullptr;
        pSprite->autorelease();
        pSprite->initOptions();

        
        //pSprite->addEvents();

        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return NULL;
    
}
bool Player::isCollided(Rect rect)
{
    if( !rect.intersectsRect( getBoundingBox() ))
        return false;
    
    if( rect.intersectsRect( getBoundingBoxUp() ))
        return true;
    
    if( rect.intersectsRect( getBoundingBoxDown() ))
        return true;
    
    if( rect.intersectsRect( getBoundingLanding() ))
        return true;
    
    return false;
    
}
Rect Player::getBoundingLanding()
{
    
     Rect rect = Rect(
            getBoundingBox().getMinX()+40, 
            getBoundingBox().getMinY(),
            getBoundingBox().size.width-80,
            4);
   
   return rect;
}

Rect Player::getBoundingBoxUp()
{
    
    /*
     Rect rect = Rect(
            getBoundingBox().getMinX(), 
            getBoundingBox().getMidY(),
            getBoundingBox().size.width,
            getBoundingBox().size.height/2);
     
     */
     Rect rect = Rect(
            getBoundingBox().getMinX()+4, 
            getBoundingBox().getMidY()-4,
            getBoundingBox().size.width-8,
            (getBoundingBox().size.height/2)-8);
     
   
   return rect;
}

Rect Player::getBoundingBoxDown()
{
   
    
     Rect rect = Rect(
            getBoundingBox().getMinX()+16, 
            getBoundingBox().getMinY(),
            getBoundingBox().size.width-32,
            getBoundingBox().size.height/2);
   
   return rect;
}
void Player::initOptions()
{
   
  
    // do things here like setTag(), setPosition(), any custom logic.
}