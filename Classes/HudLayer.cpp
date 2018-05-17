/* 
 * File:   HudLayer.cpp
 * Author: yoo
 * 
 * Created on December 21, 2014, 12:21 PM
 */

#include "HudLayer.h"

bool HudLayer::init()
{
    _menuSet = false;

    if (Layer::init())
    {
        _points = 0;

        Size visibleSize = Director::getInstance()->getVisibleSize();
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        //  _levelLabel = CCLabelTTF::create("", "Arial", 12);
        //   _scoreLabel = LabelAtlas::create("", "fonts/labelatlas.png", 16, 24, '.');
        //     _scoreTransient = LabelAtlas::create("", "fonts/labelatlas.png", 16, 24, '.');

        //        _scorePoints = LabelAtlas::create("", "fonts/labelatlas.png", 16, 24, '.');
        //    _lbGas = LabelAtlas::create("", "fonts/labelatlas.png", 16, 24, '.');

        // _levelLabel = LabelAtlas::create("", "fonts/labelatlas.png", 16, 24, '.');
        //        int margin = 10;
        //        _label->setPosition(ccp(winSize.width - (_label->getContentSize().width/2) - margin, 
        //                _label->getContentSize().height/2 + margin));
        //        
        auto displaylive = Sprite::create("Hud_1.png");
        this->addChild(displaylive);
        displaylive->setPosition(82, visibleSize.height - 44);

        //        auto displaylavel = Sprite::create("livedisplay.png");
        //        this->addChild(displaylavel);
        //        displaylavel->setPosition(160, visibleSize.height - 20);


        //        this->addChild(_label);
        //        _label->setPosition( 60,visibleSize.height+22);

        auto s = Director::getInstance()->getWinSize();
        //- LIFES   
        _lbLifes = Label::createWithBMFont("fonts/futura-48.fnt", "");
        addChild(_lbLifes);
        _lbLifes->setPosition(50, visibleSize.height - 46);

        //- CURRENT LEVEL
        _lbLevel = Label::createWithBMFont("fonts/futura-48.fnt", "");
        addChild(_lbLevel);
        _lbLevel->setPosition(s.width - 100, visibleSize.height - 40);

        //- SCORE
        _lbScorel = Label::createWithBMFont("fonts/futura-48.fnt", "");
        addChild(_lbScorel);
        _lbScorel->setPosition((s.width / 2) - 18, visibleSize.height - 40);


        //- GAS
        TTFConfig ttfConfig("fonts/arial.ttf", 32);
        _lbGas = Label::createWithTTF(ttfConfig, "", TextHAlignment::CENTER, s.width);
        addChild(_lbGas, 0, 2);
        _lbGas->setTextColor(Color4B::BLUE);
        _lbGas->setPosition(126, visibleSize.height - 46);

        //- CURRENT LEVEL
        lbScoreTransient = Label::createWithBMFont("fonts/futura-48.fnt", "");
        addChild(lbScoreTransient);
        //    lbScoreTransient->setPosition(s.width -100, visibleSize.height - 40);




        // LabelTTF
        /*
         TTFConfig ttfConfig("fonts/arial.ttf", 32);
         auto label2 = Label::createWithTTF(ttfConfig, "12", TextHAlignment::CENTER, s.width);
         addChild(label2, 0, 2);
         label2->setPosition(Vec2(s.width / 2, s.height / 2));
         */
        /*
         auto s = Director::getInstance()->getWinSize();
        TTFConfig config("fonts/tahoma.ttf", 16);
        auto ttf0 = Label::createWithTTF(config, "Alignment 0\nnew line", TextHAlignment::LEFT);
        ttf0->setColor( Color3B::RED );
        ttf0->setPosition(Vec2(s.width / 2, (s.height / 6)*2 - 30));
        this->addChild(ttf0);
         */
        /*
          auto label1 = Label::createWithBMFont("fonts/bitmapFontTest2.fnt", "Test");
          label1->setColor( Color3B::RED );
          label1->setAnchorPoint(Vec2(0, 0));
          addChild(label1, 0, 1 );
          auto fade = FadeOut::create(1.0f);
          auto fade_in = fade->reverse();
          auto seq = Sequence::create(fade, fade_in, nullptr);
          auto repeat = RepeatForever::create(seq);
          label1->runAction(repeat);
         */
        /*
                // LabelBMFont
                auto s = Director::getInstance()->getWinSize();
                auto label1 = Label::createWithBMFont("fonts/bitmapFontTest3.fnt", "LabelBMFont", TextHAlignment::CENTER, s.width);
                addChild(label1, 0, 1);
                label1->setPosition(Vec2(s.width / 2, s.height - 100));

                // LabelTTF
                TTFConfig ttfConfig("fonts/arial.ttf", 24);
                auto label2 = Label::createWithTTF(ttfConfig, "LabelTTF", TextHAlignment::CENTER, s.width);
                addChild(label2, 0, 2);
                label2->setPosition(Vec2(s.width / 2, s.height / 2));

                auto label3 = Label::createWithCharMap("fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ');
                addChild(label3, 0, 3);
                label3->setPosition(Vec2(s.width / 2, 100));
                label3->setString("LabelTTF2");
         * 
         * 
         * auto label1 = static_cast<Label*>( getChildByTag(kTagBitmapAtlas1) );
         */
        /*
                // LabelBMFont
                auto s = Director::getInstance()->getWinSize();
                auto label1 = Label::createWithBMFont("fonts/konqa32.fnt", "TESTING RETINA DISPLAY");
                addChild(label1);
                label1->setPosition(Vec2(s.width / 2, s.height / 2));
        

        // LabelBMFont
          auto s = Director::getInstance()->getWinSize();
        auto label1 = Label::createWithBMFont("fonts/futura-48.fnt", "Testing Glyph Designer 12 01");
        addChild(label1);
        label1->setPosition(Vec2(s.width / 2, s.height / 2));
         */
       
    }
    
    return true;

}





void HudLayer::setMainMenuCallBacks(
                                    const ccMenuCallback& callbackOnMenuOpen,
                                    const ccMenuCallback& callbackOnQuit,
                                    const ccMenuCallback& callbackOnReplay)
{
    this->_callbackOnMenuOpen = callbackOnMenuOpen;
    this->_callbackOnQuit = callbackOnQuit;
    this->_callbackOnReplay = callbackOnReplay;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           _callbackOnMenuOpen);

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
                                origin.y + closeItem->getContentSize().height / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

}

void HudLayer::menuOpen()
{
    if (!_menuSet || getChildByName("menu") == nullptr)
    {
        _menuSet = true;
        MenuItemFont::setFontName("fonts/Marker Felt.ttf");
        auto item2 = MenuItemFont::create("Quit", this->_callbackOnQuit);
        auto item1 = MenuItemFont::create("Play again", this->_callbackOnReplay);

        this->_mainMenu = Menu::create(item1, item2, nullptr);
        _mainMenu->alignItemsVertically();

        addChild(_mainMenu, 1000, "menu");

    }
    // elastic effect
    auto s = Director::getInstance()->getWinSize();

    int i = 0;
    for (const auto &child : _mainMenu->getChildren())
    {
        auto dstPoint = child->getPosition();
        int offset = (int) (s.width / 2 + 50);
        if (i % 2 == 0)
            offset = -offset;

        child->setPosition(Vec2(dstPoint.x + offset, dstPoint.y));
        child->runAction(
                         EaseElasticOut::create(MoveBy::create(2, Vec2(dstPoint.x - offset, 0)), 0.35f)
                         );
        i++;
    }

    _mainMenu->setPosition(Vec2(s.width / 2, s.height / 2));
    _mainMenu->setScale(0);
    _mainMenu->runAction(ScaleTo::create(1, 1));
}

void HudLayer::setScoreValue(const int value)
{
    CCString *labelCollected = new CCString();
    labelCollected->initWithFormat("%d", value);
    _lbScorel->cleanup();
    _lbScorel->setString("");
    _lbScorel->setString(labelCollected->getCString());
}

void HudLayer::setLifesValue(const int value)
{
    char content[32];
    sprintf(content, "%d", value);
    _lbLifes->cleanup();
    _lbLifes->setString(content);
}

void HudLayer::setLevelValue(const int value)
{
    char content[32];
    sprintf(content, "Level: %d", value);
    _lbLevel->cleanup();
    _lbLevel->setString(content);
}

void HudLayer::setGasValue(const int value)
{
    char content[32];
    sprintf(content, "%d", value);
    _lbGas->cleanup();
    _lbGas->setString(content);
}

void HudLayer::setScoreTransient(const int value, Vec2 position)
{
    char content[32];
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto winsize = Director::getInstance()->getWinSize();
    //auto target = Vec2((winsize.width /2), winsize.height);

    auto target = Vec2((visibleSize.width / 2) - 50, visibleSize.height + 100);


    // CCString *labelCollected = new CCString();
    //   labelCollected->initWithFormat("Level %d",value);


    sprintf(content, "%d", value);
    lbScoreTransient->cleanup();
    lbScoreTransient->setPosition(Vec2(position.x, position.y));
    lbScoreTransient->setString(content);


    auto movement = Sequence::create(
                                     MoveTo::create(0.5, target), CallFunc::create([&]()
                                     {

                                     }), nullptr);

    lbScoreTransient->runAction(movement);
    /*
   _scoreTransient->runAction( MoveTo::create(0.5, target),    
           CallFunc::create( [&](){
            
            
           }),nullptr);
     */


    _points += value;
    sprintf(content, " %d", _points);
    _lbScorel->cleanup();
    _lbScorel->setString(content);

    //  _scoreTransient->runAction(
    //                      EaseElasticOut::create(MoveBy::create(12, Vec2(position.x-100,  position.y)), 0.1f));
}
