#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__


#include "cocos2d.h"
#include "ParallaxNodeExtras.h"
#include "Player.h"
#include "MapObject.h"
#include "HudLayer.h"
#include "StartHUD.h"

/*
#include "MessageHUD.h"
#include "HudLayer.h"


#include "AnimatedSprite.h"

*/

//#include "audio/include/AudioEngine.h"


using namespace cocos2d;
using namespace std;

class Game : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuStartCallback(Ref* pSender);
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(Game);
    
    
    
    
    
    
private:   
    TMXTiledMap* _tiledMap;  
    ParallaxNodeExtras* _parallax;
    Sprite* _paralaxBackground1;
    Sprite* _paralaxBackground2;
    Vector<Sprite*>* _paralaxBackgroundList;
    Player* _player;
    int _level;
    
    HudLayer* _hudLayer;
    StartHUD* _startHud;
    
    
    float _gravityForce;
    int _gasValue;
    bool _isLanding;
    bool _isDie;
    bool _isHudSet;
    std::vector<std::unique_ptr<MapObject>> _mapObjects;

    std::vector<int>* _depositedcargos;
    int _currentCargo;

    int _score;
    int _lifes;
    int _cargodeposites;
    int _maxcargodeposites;
    DrawNode* _drawNode;

    bool _finishLevel;
    bool _lockUpdateSet;
    bool _pauseSet;
            
    ParticleSun* _emitterFire;
    ParticleFlower* _emitter;
    ParticleSystemQuad* _emitter2;
    
    void update(const float dt);
    void UpdateTimer(const float dt);
    void loadLevel(const int level);
    void updateParalax(const float dt);
    void setViewPointCenter(const Point position);
    
    void CreateMapObjects();
    void setCurrentCargoStartPosition();
    void setPlayerStartPosition();
    
    AnimatedSprite* CreateAnimObject(ValueMap& dict);
    std::unique_ptr<MapObject> const& getMapObjectByProperty(const std::string name) const;
    std::unique_ptr<MapObject> const& getMapObjectByProperty(const std::string name, const float value) const;
    int countMapObjectByProperty(const std::string name) const;
    int getValueMapObjectByProperties(const std::string name1, const std::string name2) const;
    int countValueMapObjectByPropertyName(const std::string p) const;
    std::unique_ptr<MapObject> const& getNextCargoByPlatform(const int platform);
    void attachNextCargoByPlatform(int platform);
    
    void onTouchEnded(Touch* touch, Event *event);
    bool onTouchBegan(Touch* touch, Event *event);
    
    void checkCollisions();
    void dieAnimation();
    void ParticleFromFile();
    void setParticles();
    void setOnFireParticle(bool visible);
    
};

#endif // __GAME_SCENE_H__
