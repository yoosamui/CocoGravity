/* 
 * File:   AnimatedSprite.h
 * Author: yoo
 *
 * Created on December 26, 2014, 3:18 PM
 */

#ifndef ANIMATEDSPRITE_H
#define	ANIMATEDSPRITE_H

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class AnimatedSprite : public cocos2d::Sprite
{

    public:
        AnimatedSprite();
        virtual ~AnimatedSprite();
     
        static AnimatedSprite* create(const std::string name, const int frames );
        void createAnimation();
        
      //  void initOptions();
       // void setFilename();
        void setName(const string name);
        void setFrames(const int frames);
        
        string getFilename();
        
    protected:
        std::string _name;
        std::string _fileName;
        std::string _ListName;
        int _frames;
        
        CCAnimation* _animation;
};

#endif	/* ANIMATEDSPRITE_H */

