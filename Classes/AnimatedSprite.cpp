/* 
*
 * File:   AnimatedSprite.cpp
 * Author: yoo
 * 
 * Created on December 26, 2014, 3:18 PM
 */

#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite() {
}


AnimatedSprite::~AnimatedSprite() {
}


void AnimatedSprite::setName(const string name)
{
    this->_name = name;
    this->_fileName = name +".png";
    this->_ListName = name +".plist";
}

void AnimatedSprite::setFrames(const int frames)
{
    this->_frames = frames;
}

void AnimatedSprite::createAnimation()
{
   auto cache = SpriteFrameCache::getInstance();
   cache->addSpriteFramesWithFile(_ListName);

   //auto spritebatchbee = SpriteBatchNode::create(_fileName);
   Vector<SpriteFrame*> frames(_frames);
   
   char str[50] = {0};
   for(int i = 1; i < 3; i++) 
    {
        sprintf(str, +"%s%d",_name.c_str(), i);
        auto frame = cache->getSpriteFrameByName( str );

        frames.pushBack(frame);
    }
   
   _animation = Animation::createWithSpriteFrames(frames, 0.3f);
    Sprite::createWithSpriteFrameName(_name+"1");
   
    auto  action  = RepeatForever::create( Animate::create(_animation) );
    runAction( action );
}

string AnimatedSprite::getFilename()
{
    return this->_fileName;
}


AnimatedSprite* AnimatedSprite::create(const std::string name, const int frames )
{
    AnimatedSprite* obj = new AnimatedSprite();
    obj->setName( name );
    obj->setFrames(frames);
    
    obj->initWithFile(obj->getFilename());
    
    if (frames == 0 )
    {
        obj->autorelease();
        //pSprite->addEvents();

        return obj;
    }
    else if( frames > 0 )
    {
        
        obj->autorelease();
        obj->createAnimation();
        
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return NULL;
}


