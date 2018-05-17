#include "AppDelegate.h"
#include "Game.h"

USING_NS_CC;
typedef struct tagResource
    {
        cocos2d::CCSize size;
        char directory[100];
    }Resource;

    static Resource smallResource  =  { cocos2d::CCSizeMake(480, 320),   "iphone" };
    static Resource mediumResource =  { cocos2d::CCSizeMake(1024, 768),  "ipad"   };
    static Resource largeResource  =  { cocos2d::CCSizeMake(2048, 1536), "ipadhd" };
    
    //static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);
    
    //1024×768
     static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(800,600);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Gravity");
        director->setOpenGLView(glview);
    }
        
     glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionExactFit);
     CCSize screenSize = glview->getFrameSize();
     
     if ((screenSize.height > mediumResource.size.height))
{ 
    director->setContentScaleFactor(largeResource.size.height/designResolutionSize.height);


}
// if the frame's height is larger than the height of small resource size, select medium resource.
else if ((screenSize.height > smallResource.size.height)) 

{ 
    director->setContentScaleFactor(mediumResource.size.height/designResolutionSize.height);

}
// if the frame's height is smaller than the height of medium resource size, select small resource.
else
{
    director->setContentScaleFactor(smallResource.size.height/designResolutionSize.height);

}    
    director->setContentScaleFactor(1.0f);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = Game::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    //CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
   // CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
}
