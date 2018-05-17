/* 
 * File:   MapObject.h
 * Author: yoo
 *
 * Created on December 20, 2014, 2:39 PM
 */

#ifndef MAPOBJECT_H
#define	MAPOBJECT_H

#include "cocos2d.h"
#include "AnimatedSprite.h"

using namespace cocos2d;
using namespace std;

class MapObject
{
    public:
        MapObject(const string name, Rect* rectangle, AnimatedSprite* animObject, ValueMap& properties, ValueMap* prop)
        {
            this->name = name;
            this->rectangle = rectangle;
            this->animObject = animObject;
            this->properties = properties;
            this->prop = prop;
            
        };
        
        string name;
        Rect* rectangle;
        AnimatedSprite* animObject;
        ValueMap properties;
        ValueMap* prop;
};

#endif	/* MAPOBJECT_H */

