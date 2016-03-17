//
//  LogerScene.cpp
//
//  Created by crh on 16-3-2.
//
//

#include "LogerScene.h"


USING_NS_CC;

Scene* LogerScene::createScene( LCB callback )
{
    auto scene = Scene::create();
    
    auto layer = LogerScene::create();
    
    scene->addChild(layer);
    
    layer->addLogerCallBcak( callback );
    
    return scene;
}

bool LogerScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->scheduleOnce(schedule_selector(LogerScene::runGame), 1);
    
    return true;
}

void LogerScene::addLogerCallBcak( LCB callback )
{
    this->cb = callback;
}

void LogerScene::runGame( float dt )
{
    this->cb();
}

