//
//  LogerScene.h
//
//  Created by crh on 16-3-2.
//
//

#ifndef __test10js__LogerScene__
#define __test10js__LogerScene__

#include "cocos2d.h"

typedef std::function<void ()> LCB;

class LogerScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene( LCB callback );
    
    virtual bool init();
    
    void addLogerCallBcak( LCB callback );
    
    void runGame( float dt );
    
    CREATE_FUNC(LogerScene);
private:
    LCB cb;
    
};

#endif
