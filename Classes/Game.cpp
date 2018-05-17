#include "Game.h"
#include "audio/include/SimpleAudioEngine.h"

#define MAPFILE_LEVEL_1 "maps/Gravity_Level_1.tmx"
#define PARALAXFILE__LEVEL_1 "bckgrdLevel_1_1024x800.png"
#define GROUNDFILE__LEVEL_1 "ground_lavel_1.png"
#define PARALAXSCROLL_SPEED 100
#define SHOWLINES 0

#define LANDING_SOUND "Sounds/Flip1.wav"
//#define BACKGROUND_SOUND "Sounds/bg/seashore.mp3"// "Sounds/Song.wav"
#define DIE_SOUND "Sounds/Explosion1b.wav"
#define CARGO_SOUND "Sounds/DialogueOpen.wav"
#define GAS_SOUND "Sounds/Energy1.wav"
#define POINTS_SOUND "Sounds/Missile3.wav"

#define GRAVITYFORCE  1.2f
#define CARGOGRAVITY 1.2f // NOT USED
#define STARTLEVEL  1
#define MAXGASVALUE 10
#define MAXLIFES 3


static const char s_stars1[] = "Images/stars.png";
static const char s_fire[] = "Images/fire.png";
static const char s_stars2[] = "Images/stars2.png";


USING_NS_CC;

Scene* Game::createScene()
{
    auto scene = Scene::create(); // 'scene' is an autorelease object
    auto layer = Game::create(); // 'layer' is an autorelease object
    scene->addChild(layer); // add layer as a child to scene


    layer->_startHud = StartHUD::create();
    layer->_startHud->init();
    scene->addChild(layer->_startHud);

    layer->_startHud->setMainMenuCallBacks
            (
            CC_CALLBACK_1(Game::menuCloseCallback, layer),
            CC_CALLBACK_1(Game::menuStartCallback, layer)
            );

    layer->_hudLayer = HudLayer::create();
    layer->_hudLayer->init();
    scene->addChild(layer->_hudLayer);



    layer->_startHud->Show();


    //  scene->addChild(layer->_startHud);


    /*
    

     */
    return scene; // return the scene
}

// on "init" you need to initialize your instance

bool Game::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(Game::menuCloseCallback, this));

    //closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
      //      origin.y + closeItem->getContentSize().height / 2));

    closeItem->setPosition(Vec2((closeItem->getContentSize().width / 2) + 1,
            origin.y +  (closeItem->getContentSize().height / 2) +1 ) );

    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    _isDie = false;
    _isLanding = false;
    _lifes = MAXLIFES;
    _level = STARTLEVEL;
    _lockUpdateSet = true;
    _gravityForce = GRAVITYFORCE;
    _cargodeposites = 1;
    _isHudSet = false;
    // _hudLayer = nullptr;
    _finishLevel = false;



    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(LANDING_SOUND);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(DIE_SOUND);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(CARGO_SOUND);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(POINTS_SOUND);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GAS_SOUND);


    _drawNode = DrawNode::create();
    this->addChild(_drawNode);

    // Player
    _player = Player::create();
    this->addChild(_player, 3);
    _player->setAnchorPoint(Vec2(0, 0));


    _level = STARTLEVEL;
    loadLevel(_level);


    // Update//
    this->scheduleUpdate();
    this->schedule(schedule_selector(Game::UpdateTimer), 5.0f);


    // set default volume
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(2.5);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);

    //
    //--Touch Listeners setup
    //
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
    getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 100);

    setOnFireParticle(false);


    return true;
}

/*******************************************************************
 * loadLevel
 * 
 *******************************************************************/
void Game::loadLevel(const int level)
{
    char filename[100] = {0};
    Size visibleSize = Director::getInstance()->getVisibleSize();

    setOnFireParticle(false);

    this->removeChildByName("map", true);
    this->removeChildByName("paralax", true);
    //
    // _parallax 
    //
    _parallax = ParallaxNodeExtras::create();
    this->addChild(_parallax, -1000, "paralax");

    sprintf(filename, "bckgrdLevel_%d_1024x800.png", level);
    this->_paralaxBackground1 = Sprite::create(filename);
    this->_paralaxBackground2 = Sprite::create(filename);

    _paralaxBackgroundList = new Vector<Sprite*>(2);
    _paralaxBackgroundList->pushBack(_paralaxBackground1);
    _paralaxBackgroundList->pushBack(_paralaxBackground2);

    auto paralaxSpeed = Vect(0.1, 0.1);

    _parallax->addChild(_paralaxBackground1, 0, paralaxSpeed, Point(0, visibleSize.height / 2));
    _parallax->addChild(_paralaxBackground2, 0, paralaxSpeed, Point(_paralaxBackground2->getContentSize().width, visibleSize.height / 2));


    //
    // map 
    //
    sprintf(filename, "maps/Gravity_Level_%d.tmx", level);
    _tiledMap = TMXTiledMap::create(filename);
    addChild(_tiledMap, 0, "map");


    //
    // BG Music
    //
    sprintf(filename, "Sounds/bg/Level_%d.wav", level);
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(filename, true);

    CreateMapObjects();

    _lifes = 3;
    _isDie = false;
    _pauseSet = true;
    _lockUpdateSet = false;
    _cargodeposites = 0;
    _currentCargo = 0;
    _isHudSet = false;
    _gasValue = MAXGASVALUE;
    _isLanding = false;
    _finishLevel = false;

    _depositedcargos = new std::vector<int>();
    _maxcargodeposites = countMapObjectByProperty("cargo");


    setPlayerStartPosition();

}

/*******************************************************************
 * setPlayerStartPosition
 * 
 *******************************************************************/
void Game::setPlayerStartPosition()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto group = _tiledMap->getObjectGroup("Objects");

    _player->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    if (group != nullptr) {
        ValueMap startPoint = group->getObject("PlayerStartPoint");
        if (!startPoint.empty()) {
            float x = startPoint["x"].asFloat();
            float y = startPoint["y"].asFloat();

            _player->setPosition(x, y);

        }
    }
}

/*******************************************************************
 * setCurrentCargoStartPosition
 * 
 *******************************************************************/
void Game::setCurrentCargoStartPosition()
{
    auto cargo = _player->getCargo();
    if (cargo == nullptr)
        return;

    auto& p = getMapObjectByProperty("cargo", _currentCargo);
    if (p != nullptr)
        cargo->setPosition(p->properties["x"].asFloat(), p->properties["y"].asFloat());

    _player->attachCargo(nullptr);
}

/*******************************************************************
 * attachNextCargoByPlatform
 * 
 *******************************************************************/
void Game::attachNextCargoByPlatform(int platform)
{

    for (const auto& obj : _mapObjects) {
        auto cvalue = obj->properties["cargo"].asInt();
        if (cvalue == 0)
            continue;

        auto p = obj->properties["platform"].asInt();
        if (p != platform)
            continue;

        for (int c : *_depositedcargos)
            if (c == cvalue)
                return;

        _currentCargo = cvalue;
        _player->attachCargo(obj->animObject);
    }


}

/*******************************************************************
 * getNextCargoByPlatform
 * 
 *******************************************************************/
std::unique_ptr<MapObject> const& Game::getNextCargoByPlatform(const int platform)
{

    for (const auto& obj : _mapObjects) {
        auto cvalue = obj->properties["cargo"].asInt();
        if (cvalue == 0)
            continue;

        auto p = obj->properties["platform"].asInt();
        if (p != platform)
            continue;


        for (int c : *_depositedcargos) {
            log(" _depositedcargos %d ", c);

            if (c == cvalue)
                return (std::unique_ptr<MapObject>)nullptr;
        }

        _currentCargo = cvalue;
        log(" _currentCargo %d ", cvalue);
        return obj;
    }

    return nullptr;
}

/*******************************************************************
 * countMapObjectByProperty
 * 
 *******************************************************************/
int Game::countMapObjectByProperty(const std::string name) const
{
    int counter = 0;
    for (const auto& obj : _mapObjects) {
        auto svalue = obj->properties[name].asInt();
        if (svalue != 0) {
            counter++;
            continue;
        }
    }

    return counter;
}

/*******************************************************************
 * getValueMapObjectByProperties
 * 
 *******************************************************************/
int Game::getValueMapObjectByProperties(const std::string name1, const std::string name2) const
{
    int counter = 0;
    for (const auto& obj : _mapObjects) {

        auto svalue = obj->properties[name1].asString();

        if (svalue.length() == 0) {
            auto v = obj->properties[name1].asFloat();
            if (v == 0.0)
                continue;
        }

        return obj->properties[name2].asInt();

    }

    return counter;
}

/*******************************************************************
 * countValueMapObjectByPropertyName
 * 
 *******************************************************************/
int Game::countValueMapObjectByPropertyName(const std::string p) const
{
    int counter = 0;
    for (const auto& obj : _mapObjects) {

        auto svalue = obj->properties["name"].asString();

        if (svalue.length() > 0 && svalue == p) {
            counter++;
        }

    }

    return counter;
}

/*******************************************************************
 * getMapObjectByProperty
 * 
 *******************************************************************/
std::unique_ptr<MapObject> const& Game::getMapObjectByProperty(const std::string name) const
{
    for (const auto& obj : _mapObjects) {
        auto avalue = obj->properties[name].asString();
        if (avalue.length() > 1)
            return obj;
    }

    return nullptr;

}

/*******************************************************************
 * getMapObjectByProperty
 * 
 *******************************************************************/
std::unique_ptr<MapObject> const& Game::getMapObjectByProperty(const std::string name, const float value) const
{

    for (const auto& obj : _mapObjects) {
        if (obj->properties.empty())
            continue;

        auto avalue = obj->properties[name].asFloat();
        if (avalue == value)
            return obj;
    }

    return nullptr;
}

/*******************************************************************
 * CreateMapObjects
 * 
 *******************************************************************/
void Game::CreateMapObjects()
{
    auto group = _tiledMap->getObjectGroup("Objects");
    if (group == nullptr)
        return;


    for (const auto& obj : _mapObjects) {
        if (obj != nullptr)
            this->removeChild(obj->animObject, true);
    }

    _mapObjects.clear();

    auto& objects = group->getObjects();


    for (auto& obj : objects) {
        ValueMap& dict = obj.asValueMap();

        float x = dict["x"].asFloat();
        float y = dict["y"].asFloat();
        float width = dict["width"].asFloat();
        float height = dict["height"].asFloat();
        string key = dict["name"].asString();
        string type = dict["type"].asString();

        Rect *rec = new Rect(x, y, width, height);
        AnimatedSprite* anim = nullptr;

        if (type.length() > 0)
            anim = CreateAnimObject(dict);

        _mapObjects.push_back(std::unique_ptr<MapObject>(new MapObject(key, rec, anim, dict, &dict)));
    }

#if (SHOWLINES == 1)

    auto drawNode = DrawNode::create();
    _tiledMap->addChild(drawNode);
    Color4F color(1.0, 1.0, 1.0, 1.0);

    for (const auto& obj : _mapObjects) {
        drawNode->drawRect(Vec2(obj->rectangle->getMinX(), obj->rectangle->getMinY()),
                Vec2(obj->rectangle->getMaxX(), obj->rectangle->getMaxY()), color);
    }
#endif
}

/*******************************************************************
 * CreateAnimObject
 * 
 *******************************************************************/
AnimatedSprite* Game::CreateAnimObject(ValueMap& dict)
{
    string name = dict["name"].asString();
    if (name != "AnimObject")
        return nullptr;

    float x = dict["x"].asFloat();
    float y = dict["y"].asFloat();
    string type = dict["type"].asString();
    int frames = dict["frames"].asInt();
    int xduration = dict["xduration"].asInt();
    int yduration = dict["yduration"].asInt();

    const string actions[] = {"xflip", "xmoveto", "movetothis", "ymoveto", "delay"}; // Works
    Vector<FiniteTimeAction*> arrayOfActions;

    for (int i = 1; i < 10; i++) {
        for (const auto& a : actions) {
            char actionName[32];
            sprintf(actionName, "%d_%s", i, a.c_str());

            float value = dict[actionName].asFloat();

            if (xduration == 0)
                xduration = 8;

            if (yduration == 0)
                yduration = 8;


            //  log("%d %s %s %f",i,a.c_str(),actionName,value);

            if (value == 0.0)
                continue;

            if (a == "xflip") {
                // log("--------%d %s %s %f",i,a.c_str(),actionName,value);
                arrayOfActions.pushBack(FlipX::create(value == (int) 1));
            } else if (a == "xmoveto") {
                //  log("--------%d %s %s %f",i,a.c_str(),actionName,value);
                arrayOfActions.pushBack(MoveTo::create(xduration, Vec2(x + (int) value, y)));
            } else if (a == "ymoveto") {
                //   log("--------%d %s %s %f",i,a.c_str(),actionName,value);
                arrayOfActions.pushBack(MoveTo::create(xduration, Vec2(x, y + (int) value)));
            } else if (a == "movetothis") {
                //   log("%d %s %s",i,a.c_str(),actionName);
                arrayOfActions.pushBack(MoveTo::create(xduration, Vec2(x, y)));
            } else if (a == "delay") {
                auto delay = DelayTime::create(value);
                arrayOfActions.pushBack(delay->clone());
            }
        }
    }



    auto anim = AnimatedSprite::create(type, frames);
    this->addChild(anim, 3);
    anim->setPosition(x, y);


    if (!arrayOfActions.empty()) {
        Sequence* move = Sequence::create(arrayOfActions);
        anim->runAction(RepeatForever::create(move));
    }

    return anim;
}

/*******************************************************************
 * UpdateTimer
 * 
 *******************************************************************/
void Game::UpdateTimer(const float dt)
{
    if (_pauseSet || _isDie)
        return;


    if (_finishLevel) {
        _emitter->setVisible(false);
        _level++;
        loadLevel(_level);

        _startHud->setVisible(true);
        return;
    }

    if (_isLanding)
        return;

    _gasValue--;
    _hudLayer->setGasValue(_gasValue);
}

/*******************************************************************
 * updateParalax
 * 
 *******************************************************************/
void Game::updateParalax(float dt)
{
    auto backgroundScrollVert = Point(-PARALAXSCROLL_SPEED, 0);

    this->_parallax->setPosition(this->_parallax->getPosition() +
            (backgroundScrollVert * dt));

    for (auto bck : *_paralaxBackgroundList) {
        float xPosition = _parallax->convertToWorldSpace(bck->getPosition()).x;
        float size = bck->getContentSize().width;
        if (xPosition < -size) {
            _parallax->incrementOffset(Point(bck->getContentSize().width * 2, 0), bck);
        }
    }
}

/*******************************************************************
 * setViewPointCenter
 * 
 *******************************************************************/
void Game::setViewPointCenter(const Point position)
{
    Size winSize = Director::getInstance()->getWinSize();

    int x = MAX(position.x, winSize.width / 2);
    int y = MAX(position.y, winSize.height / 2);

    x = MIN(x, (_tiledMap->getMapSize().width * this->_tiledMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tiledMap->getMapSize().height * _tiledMap->getTileSize().height) - winSize.height / 2);
    //Point actualPosition = ccp(x, y);
    Point actualPosition = Vect(x, y);

    //Point centerOfView = ccp(winSize.width / 2, winSize.height / 2);
    Point centerOfView = Vect(winSize.width / 2, winSize.height / 2);
    //Point viewPoint = ccpSub(centerOfView, actualPosition);
    Point viewPoint = centerOfView - actualPosition;

    this->setPosition(viewPoint);
}

/*******************************************************************
 * setViewPointCenter
 * 
 *******************************************************************/
void Game::update(const float dt)
{

    if (_lockUpdateSet || _pauseSet || _isDie)
        return;

    updateParalax(dt);
    checkCollisions();

    if (!_isLanding) {
        Point playerPosition = _player->getPosition();
        playerPosition.y -= _gravityForce;
        _player->setPositionY(playerPosition.y);
    }

    if (_gasValue < 4) {
        _emitterFire->setPosition(Vect(_player->getPosition().x + 30, _player->getPosition().y + 100));
        _emitterFire->setVisible(true);
    } else {
        //  _emitterFire->setVisible(false);
    }

    if (_player->getCargo() != nullptr) {
        _player->getCargo()->setPosition(_player->getPosition().x + 50, _player->getPosition().y + 10);
        _gravityForce = CARGOGRAVITY;
    } else {
        _gravityForce = GRAVITYFORCE;
    }
    setViewPointCenter(_player->getPosition());

    if (!_isHudSet) {
        _isHudSet = true;
        _hudLayer->setLifesValue(_lifes);
        _hudLayer->setLevelValue(_level);
        _hudLayer->setGasValue(_gasValue);

    }
}

/*******************************************************************
 * setViewPointCenter
 * 
 *******************************************************************/
void Game::checkCollisions()
{
    if (_isDie || _finishLevel)
        return;

    // safety check on the bounds of the map
    Point playerPos = _player->getPosition();
    if ((playerPos.x + _player->getContentSize().width > (_tiledMap->getMapSize().width * _tiledMap->getTileSize().width)) ||
            (playerPos.y + _player->getContentSize().height > (_tiledMap->getMapSize().height * _tiledMap->getTileSize().height)) ||
            (playerPos.x < 0 || playerPos.y < 0)) {
        _isDie = true;
        dieAnimation();

        return;
    }

    if (_gasValue < 1) {
        _gasValue = MAXGASVALUE;
        _hudLayer->setGasValue(_gasValue);

        _isDie = true;
        dieAnimation();

        return;
    }

#if (SHOWLINES == 1)
    Rect playerBox = _player->getBoundingBox(); //_player->getBoundingLanding(); //getBoundingBox();

    Color4F color(1.0, 1.0, 1.0, 1.0);
    _drawNode->clear();
    //    _drawNode->drawRect(Vec2(playerBox.getMinX(), playerBox.getMinY()),
    //                        Vec2(playerBox.getMaxX(), playerBox.getMaxY()), color);

    playerBox = _player->getBoundingLanding();
    _drawNode->drawRect(Vec2(playerBox.getMinX(), playerBox.getMinY()),
            Vec2(playerBox.getMaxX(), playerBox.getMaxY()), color);

    playerBox = _player->getBoundingBoxUp();
    _drawNode->drawRect(Vec2(playerBox.getMinX(), playerBox.getMinY()),
            Vec2(playerBox.getMaxX(), playerBox.getMaxY()), color);


    playerBox = _player->getBoundingBoxDown();
    _drawNode->drawRect(Vec2(playerBox.getMinX(), playerBox.getMinY()),
            Vec2(playerBox.getMaxX(), playerBox.getMaxY()), color);

#endif
    // Check for landing and cargos
    for (const auto& obj : _mapObjects) {
        if (obj->name == "Landing" && _player->getBoundingLanding().intersectsRect(*obj->rectangle)) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(LANDING_SOUND);
            auto platform = obj->properties["platform"].asInt();

            if (_cargodeposites <= _maxcargodeposites) {

                if (_player->getCargo() == nullptr) {
                    log("get cargo %d", platform);
                    attachNextCargoByPlatform(platform);
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CARGO_SOUND);
                } else if (obj->properties["deposit"].asInt() != 0 && _player->getCargo() != nullptr) {
                    _depositedcargos->push_back(_currentCargo);
                    _player->attachCargo(nullptr);
                    _currentCargo = 0;
                    _cargodeposites++;

                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(CARGO_SOUND);

                    // finish level
                    if (_cargodeposites == _maxcargodeposites) {
                        // Next level
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAS_SOUND);
                        setParticles();

                        _emitter->setPosition(Vect(_player->getPosition().x + 30, _player->getPosition().y + 30));
                        _emitter->setVisible(true);

                        _finishLevel = true;

                    }
                }
            }
            _player->stopAllActions(); //TODO: by tag
            _player->setPositionY(_player->getPosition().y + 1);
            _isLanding = true;

            return;

        }
    }

    for (const auto& obj : _mapObjects) {
        if (obj->name == "Landing")
            continue;

        if (obj->properties["type"].asString() == "honeyJarsmall")
            continue;

        
        if (obj->name == "Solid" && _player->isCollided(*obj->rectangle)) {
            _isDie = true;
            dieAnimation();

            return;
        }

        Size visibleSize = Director::getInstance()->getVisibleSize();

       
            
        //- Checks for all items except landing
        if (obj->name == "AnimObject" && obj->animObject != nullptr &&
                _player->isCollided(obj->animObject->getBoundingBox())) 
        {

            //- Checks for all gas items
            if (obj->properties["type"].asString() == "Bubble") //TODO: rename to gas
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAS_SOUND);
                obj->animObject->runAction(MoveBy::create(0.5, Vec2(visibleSize.height + 512, visibleSize.width + 512)));
                setOnFireParticle(false);
                _gasValue = MAXGASVALUE;
                _hudLayer->setGasValue(_gasValue);
                obj->animObject = nullptr;

                return;
            }

            //- Checks for all  items with points.
            int points = obj->properties["Points"].asInt();
            if (points != 0) {
                auto location = this->convertToWindowSpace(_player->getPosition());
                obj->animObject->runAction(MoveBy::create(0.5, Vec2(1000, 1000)));
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(POINTS_SOUND);
                _hudLayer->setScoreTransient(points, Vect(location.x, _player->getPosition().y));
                obj->animObject = nullptr;

                return;
            }

            auto acargo = obj->properties["cargo"].asInt();
            if (acargo > 0) {

                return;
            }

            _isDie = true;
            dieAnimation();

        }
    }
    /*
    for (const auto& obj : _mapObjects)
    {
        if (obj->name == "Landing")
            continue;

        if (obj->name == "Solid" && _player->isCollided(*obj->rectangle))
        {
            _isDie = true;
            dieAnimation();

            return;
        }

        Size visibleSize = Director::getInstance()->getVisibleSize();

        //- Checks for all items except landing
        if (obj->name == "AnimObject" && obj->animObject != nullptr &&
                _player->isCollided(obj->animObject->getBoundingBox()))
        {

      
//                //    Ignore cargo collision
            auto acargo = obj->properties["cargo"].asInt();
            if (acargo > 0 )
                return;
       
//
//               if (obj->properties["type"].asString() == "honeyJarsmall") //TODO: rename to gas
//                   return;
//                        
            //- Checks for all gas items
            if (obj->properties["type"].asString() == "Bubble") //TODO: rename to gas
            {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAS_SOUND);
                obj->animObject->runAction(MoveBy::create(0.5, Vec2(visibleSize.height + 512, visibleSize.width + 512)));
                setOnFireParticle(false);
                _gasValue = MAXGASVALUE;
                _hudLayer->setGasValue(_gasValue);
                obj->animObject = nullptr;

                return;
            }
            
            
           
            
            //- Checks for all  items with points.
            int points = obj->properties["Points"].asInt();
            if (points != 0)
            {
                auto location = this->convertToWindowSpace(_player->getPosition());
                obj->animObject->runAction(MoveBy::create(0.5, Vec2(1000, 1000)));
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(POINTS_SOUND);
                _hudLayer->setScoreTransient(points, Vect(location.x, _player->getPosition().y));
                obj->animObject = nullptr;

                return;
            }

     
            _isDie = true;
            dieAnimation();

            return;
        }
        
    }
     */

}

/*******************************************************************
 * onTouchBegan
 * 
 *******************************************************************/
void Game::dieAnimation()
{
    ParticleFromFile();

    _emitter2->setPosition(Vect(_player->getPosition().x + 20, _player->getPosition().y + 20));
    _emitter2->release();

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(DIE_SOUND);

    _player->stopAllActions();
    //  _player->runAction(Blink::create(2, 10));
    auto end = Sequence::create(
            MoveTo::create(0.5, Vec2(_player->getPositionX(), 0)),
            CallFunc::create(
            // lambda
            [&]() {

                _isDie = false;
                _isLanding = false;
                        _lockUpdateSet = false;


                        setPlayerStartPosition();
                        setCurrentCargoStartPosition();
                        setOnFireParticle(false);

                if (_lifes-- < 0)
                        _lifes = 0;

                        _hudLayer->setLifesValue(_lifes);


                    if (_lifes == 0) {
                        //////////   menuOpenMainMenuCallback(nullptr);
                        _lifes = MAXLIFES;
                                _hudLayer->setLifesValue(_lifes);
                        return;

                    }

            }), nullptr);



    _player->runAction(end);


}

/*******************************************************************
 * ParticleFromFile
 * 
 *******************************************************************/
void Game::ParticleFromFile()
{

    std::string filename = "Particles/ExplodingRing.plist";
    _emitter2 = ParticleSystemQuad::create(filename);
    _emitter2->retain();
    // _emitter2->setBlendAdditive(false);
    addChild(_emitter2, 10);

}

/*******************************************************************
 * setOnFireParticle
 * 
 *******************************************************************/
void Game::setOnFireParticle(bool visible)
{
    if (!visible && _emitterFire != nullptr) {
        _emitterFire->setVisible(visible);
        return;
    }
    _emitterFire = ParticleSun::create();
    _emitterFire->retain();

    Color4F startColor(0.1f, 0.1f, 0.1f, 1.0f);
    _emitterFire->setStartColor(startColor);

    Color4F startColorVar(0.5f, 0.5f, 0.5f, 1.0f);
    _emitterFire->setStartColorVar(startColorVar);

    Color4F endColor(0.1f, 0.1f, 0.1f, 0.2f);
    _emitterFire->setEndColor(endColor);

    Color4F endColorVar(0.1f, 0.1f, 0.1f, 0.2f);
    _emitterFire->setEndColorVar(endColorVar);

    this->addChild(_emitterFire, 10);
    _emitterFire->setTexture(Director::getInstance()->getTextureCache()->addImage(s_fire));
    _emitterFire->setBlendAdditive(false);


}

/*******************************************************************
 * setParticles
 * 
 *******************************************************************/
void Game::setParticles()
{

    //auto s_stars1 =  "Images/fire.png";

    //   _emitter = ParticleFireworks::create();
    _emitter = ParticleFlower::create();
    //   _emitter = ParticleSystemQuad::createWithTotalParticles(300);
    _emitter->retain();
    // color of particles
    Color4F startColor(0.5f, 0.5f, 0.5f, 1.0f);
    _emitter->setStartColor(startColor);

    Color4F startColorVar(0.5f, 0.5f, 0.5f, 1.0f);
    _emitter->setStartColorVar(startColorVar);

    Color4F endColor(0.1f, 0.1f, 0.1f, 0.2f);
    _emitter->setEndColor(endColor);

    Color4F endColorVar(0.1f, 0.1f, 0.1f, 0.2f);
    _emitter->setEndColorVar(endColorVar);


    this->addChild(_emitter, 10);
    _emitter->setTexture(Director::getInstance()->getTextureCache()->addImage(s_stars1));
    _emitter->setBlendAdditive(false);
    //    auto s = Director::getInstance()->getWinSize();
    if (_emitter != nullptr) {
        //  _emitter->setPosition(Vec2(s.width / 2, s.height / 2));
        //_emitter->setPosition(_player->getPosition());
        _emitter->setPosition(Vect(_player->getPosition().x + 30, _player->getPosition().y + 30));
    }

    _emitter->setVisible(false);
}

/*******************************************************************
 * onTouchBegan
 * 
 *******************************************************************/
bool Game::onTouchBegan(Touch* touch, Event *event)
{


    return true;
}

/*******************************************************************
 * onTouchEnded
 * 
 *******************************************************************/
void Game::onTouchEnded(Touch* touch, Event *event)
{
    if (_pauseSet)
        return;

    Point touchLocation = touch->getLocationInView();
    touchLocation = Director::getInstance()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);

    Point playerPos = _player->getPosition();

    if (_isLanding)
        touchLocation.y += 100 * _gravityForce;

    auto move = Sequence::create(MoveTo::create(20.0f, touchLocation), nullptr);
    //   auto move_ease_in = EaseExponentialIn::create(move->clone());
    auto move_ease_out = EaseExponentialOut::create(move->clone());
    auto delay = DelayTime::create(0.1f);
    auto seq2 = Sequence::create(move_ease_out, delay->clone(), NULL);

    _player->runAction(seq2);
    _isLanding = false;

}

/*******************************************************************
 * menuCloseCallback
 * 
 *******************************************************************/
void Game::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

/*******************************************************************
 * menuStartCallback
 * 
 *******************************************************************/
void Game::menuStartCallback(Ref* pSender)
{
    _startHud->setVisible(false);
    _pauseSet = false;

}

