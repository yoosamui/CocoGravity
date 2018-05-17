/* 
 * File:   StartHUD.h
 * Author: yoo
 *
 * Created on August 7, 2015, 1:14 PM
 */

#ifndef STARTHUD_H
#define	STARTHUD_H

#include "cocos2d.h"

using namespace cocos2d;


class StartHUD: public Layer
{
    public:
        StartHUD();
        
        virtual bool init();

        // implement the "static create()" method manually
        CREATE_FUNC(StartHUD);

        void Show();
        void onQuit(Ref* sender);
        void setMainMenuCallBacks(const ccMenuCallback& callbackOnQuit, const ccMenuCallback& callbackOnStart);


    private:
         Sprite* _sprite;
         
         
      
           
};

#endif	/* STARTHUD_H */

