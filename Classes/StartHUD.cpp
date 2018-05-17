/* 
 * File:   StartHUD.cpp
 * Author: yoo
 * 
 * Created on August 7, 2015, 1:14 PM
 */

#include "StartHUD.h"

StartHUD::StartHUD()
{
    _sprite = nullptr;
}

bool StartHUD::init()
{
    
    if (!Layer::init())
        return false;
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    //CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // Vec2 origin = Director::getInstance()->getVisibleOrigin();
     
     
     _sprite = Sprite::create("startHud.png");
     addChild(_sprite, 1);
     
     _sprite->setVisible( false);
     _sprite->setPosition(0,0);
    return true;
}
          
void StartHUD::Show()
{
  setVisible(true);
  
  _sprite->setVisible( true);
  _sprite->setPosition(0,0);
   Size visibleSize = Director::getInstance()->getVisibleSize();
  _sprite->runAction(EaseElasticOut::create(MoveBy::create(2, 
          Vec2(visibleSize.width/2,visibleSize.height/2))));
  
  
}
void StartHUD::setMainMenuCallBacks(const ccMenuCallback& callbackOnQuit, const ccMenuCallback& callbackOnStart)
{

    
     MenuItemFont::setFontName("fonts/Marker Felt.ttf");
     auto item1= MenuItemFont::create("Exit", callbackOnQuit);
     auto item2= MenuItemFont::create("Start", callbackOnStart);
       
     auto menu = Menu::create(item1,item2,NULL);
     
    
     menu->setPosition(Vec2(_sprite->getContentSize().width/2 , _sprite->getContentSize().height/2));
     menu->alignItemsVertically();
     menu->alignItemsInColumns(2, 2, NULL);
     
    _sprite->addChild(menu);
      
}

void StartHUD::onQuit(Ref* sender)
{

}

