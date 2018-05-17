

#ifndef Cocos2DxFirstIosSample_CCParallaxNodeExtras_h
#define Cocos2DxFirstIosSample_CCParallaxNodeExtras_h

#include "cocos2d.h"

USING_NS_CC;

/*

class CCParallaxNodeExtras : public CCParallaxNode {
 
    public :
 
    // Need to provide a constructor 
    CCParallaxNodeExtras();
 
    // just to avoid ugly later cast and also for safety
    static CCParallaxNodeExtras * node();
 
    // Facility method (it’s expected to have it soon in COCOS2DX)    
    void incrementOffset(CCPoint offset, CCNode* node);  
} ; 
*/



class ParallaxNodeExtras : public ParallaxNode {

public:

	// Need to provide a constructor
	ParallaxNodeExtras();

	// just to avoid ugly later cast and also for safety
	static ParallaxNodeExtras * create();

	// Facility method (it�s expected to have it soon in COCOS2DX)
	void incrementOffset(Point offset, Node* node);
};


#endif