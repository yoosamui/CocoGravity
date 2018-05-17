/* 
 * File:   HudLayer.h
 * Author: yoo
 *
 * Created on December 21, 2014, 12:21 PM
 */

#ifndef HUDLAYER_H
#define	HUDLAYER_H


#include "cocos2d.h"
using namespace cocos2d;



class HudLayer: public Layer
{
public:
      // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
      // implement the "static create()" method manually
    CREATE_FUNC(HudLayer);

  
    
    void setScoreValue(const int value);
    
    void setLevelValue(const int value);
    void setLifesValue(const int value);
    
    void setScoreTransient(const int value, Vec2 position);
    void setGasValue(const int value);
    
    void menuOpen();
    void setMainMenuCallBacks(
        const ccMenuCallback& callbackOnMenuOpen,
        const ccMenuCallback& callbackOnQuit,
        const ccMenuCallback& callbackOnReplay);
    
     Menu *_mainMenu;
     
private:
   //LabelAtlas*  _scoreLabel;
 //   LabelAtlas*  _scoreTransient;
 //    LabelAtlas*  _scorePoints;
   //  LabelAtlas*  _gasLabel;
     int _points;
     
     Label* _lbLevel; 
     Label* _lbLifes; 
     Label* _lbGas; 
     Label*_lbScorel;
     Label* lbScoreTransient;
    CCLabelTTF* _levelLabel;
    
    ccMenuCallback _callbackOnMenuOpen;
    ccMenuCallback _callbackOnQuit;
    ccMenuCallback _callbackOnReplay;
    
    bool _menuSet ;
    
    
    
};

#endif	/* HUDLAYER_H */

