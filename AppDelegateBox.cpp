#include "AppDelegateBox.h"

#include "SimpleAudioEngine.h"
#include "jsb_cocos2dx_auto.hpp"
#include "jsb_cocos2dx_ui_auto.hpp"
#include "jsb_cocos2dx_studio_auto.hpp"
#include "jsb_cocos2dx_builder_auto.hpp"
#include "jsb_cocos2dx_spine_auto.hpp"
#include "jsb_cocos2dx_extension_auto.hpp"
#include "jsb_cocos2dx_3d_auto.hpp"
#include "jsb_cocos2dx_3d_extension_auto.hpp"
#include "3d/jsb_cocos2dx_3d_manual.h"
#include "ui/jsb_cocos2dx_ui_manual.h"
#include "cocostudio/jsb_cocos2dx_studio_manual.h"
#include "cocosbuilder/js_bindings_ccbreader.h"
#include "spine/jsb_cocos2dx_spine_manual.h"
#include "extension/jsb_cocos2dx_extension_manual.h"
#include "localstorage/js_bindings_system_registration.h"
#include "chipmunk/js_bindings_chipmunk_registration.h"
#include "jsb_opengl_registration.h"
#include "network/XMLHTTPRequest.h"
#include "network/jsb_websocket.h"
#include "network/jsb_socketio.h"
#include "jsb_cocos2dx_physics3d_auto.hpp"
#include "physics3d/jsb_cocos2dx_physics3d_manual.h"
#include "jsb_cocos2dx_navmesh_auto.hpp"
#include "navmesh/jsb_cocos2dx_navmesh_manual.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "jsb_cocos2dx_experimental_video_auto.hpp"
#include "experimental/jsb_cocos2dx_experimental_video_manual.h"
#include "jsb_cocos2dx_experimental_webView_auto.hpp"
#include "experimental/jsb_cocos2dx_experimental_webView_manual.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "jsb_cocos2dx_audioengine_auto.hpp"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/CCJavascriptJavaBridge.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "platform/ios/JavaScriptObjCBridge.h"
#endif

//#include "AppDelegate.h"
#include "LogerSingleton.h"
#include "LogerScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegateBox::AppDelegateBox()
{
}

AppDelegateBox::~AppDelegateBox()
{
    ScriptEngineManager::destroyInstance();
}

void AppDelegateBox::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}


bool AppDelegateBox::applicationDidFinishLaunching()
{
    Director::getInstance()->getConsole()->addClientCallback( std::bind(&AppDelegateBox::runXXXXXX, this) );
    Director::getInstance()->getConsole()->listenOnTCP(6678);
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        glview = cocos2d::GLViewImpl::create("test10js");
#else
        glview = cocos2d::GLViewImpl::createWithRect("test10js", Rect(0,0,LogerSingleton::getInstance()->getLogerSize().width,LogerSingleton::getInstance()->getLogerSize().height));
#endif
        director->setOpenGLView(glview);
}
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
//    log("%s", LogerSingleton::getInstance()->getProjectPath());
    
    ScriptingCore* sc = ScriptingCore::getInstance();
    sc->addRegisterCallback(register_all_cocos2dx);
    sc->addRegisterCallback(register_cocos2dx_js_core);
    sc->addRegisterCallback(jsb_register_system);

    // extension can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_extension);
    sc->addRegisterCallback(register_all_cocos2dx_extension_manual);

    // chipmunk can be commented out to reduce the package
    sc->addRegisterCallback(jsb_register_chipmunk);
    // opengl can be commented out to reduce the package
    sc->addRegisterCallback(JSB_register_opengl);
    
    // builder can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_builder);
    sc->addRegisterCallback(register_CCBuilderReader);
    
    // ui can be commented out to reduce the package, attension studio need ui module
    sc->addRegisterCallback(register_all_cocos2dx_ui);
    sc->addRegisterCallback(register_all_cocos2dx_ui_manual);

    // studio can be commented out to reduce the package, 
    sc->addRegisterCallback(register_all_cocos2dx_studio);
    sc->addRegisterCallback(register_all_cocos2dx_studio_manual);
    
    // spine can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_spine);
    sc->addRegisterCallback(register_all_cocos2dx_spine_manual);
    
    // XmlHttpRequest can be commented out to reduce the package
    sc->addRegisterCallback(MinXmlHttpRequest::_js_register);
    // websocket can be commented out to reduce the package
    sc->addRegisterCallback(register_jsb_websocket);
    // sokcet io can be commented out to reduce the package
    sc->addRegisterCallback(register_jsb_socketio);

    // 3d can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_3d);
    sc->addRegisterCallback(register_all_cocos2dx_3d_manual);
    
    // 3d extension can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_3d_extension);
    
#if CC_USE_3D_PHYSICS && CC_ENABLE_BULLET_INTEGRATION
    // Physics 3d can be commented out to reduce the package
    sc->addRegisterCallback(register_all_cocos2dx_physics3d);
    sc->addRegisterCallback(register_all_cocos2dx_physics3d_manual);
#endif

#if CC_USE_NAVMESH
    sc->addRegisterCallback(register_all_cocos2dx_navmesh);
    sc->addRegisterCallback(register_all_cocos2dx_navmesh_manual);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sc->addRegisterCallback(register_all_cocos2dx_experimental_video);
    sc->addRegisterCallback(register_all_cocos2dx_experimental_video_manual);
    sc->addRegisterCallback(register_all_cocos2dx_experimental_webView);
    sc->addRegisterCallback(register_all_cocos2dx_experimental_webView_manual);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    sc->addRegisterCallback(register_all_cocos2dx_audioengine);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sc->addRegisterCallback(JavascriptJavaBridge::_js_register);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    sc->addRegisterCallback(JavaScriptObjCBridge::_js_register);
#endif
    sc->start();    
    sc->runScript("script/jsb_boot.js");
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    sc->enableDebugger();
#endif
    
    
//    FileUtils::getInstance()->addSearchPath("/Users/rains/Documents/cocos/game/", true);
    
    ScriptEngineProtocol *engine = ScriptingCore::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
//    ScriptingCore::getInstance()->runScript("main.js");
//    ScriptingCore::getInstance()->runScript("main.js");
    
    
//    auto scene = Scene::create();
//    
//    // 'layer' is an autorelease object
//    auto layer = Layer::create();
//    
//    // add layer as a child to scene
//    scene->addChild(layer);
//    
//    auto closeItem = MenuItemImage::create(
//                                           "res/CloseNormal.png",
//                                           "res/CloseSelected.png",
//                                           CC_CALLBACK_1(AppDelegateBox::menuCloseCallback, this));
//    
////    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
////                                origin.y + closeItem->getContentSize().height/2));
//    
//    // create menu, it's an autorelease object
//    auto menu = Menu::create(closeItem, NULL);
//    menu->setPosition(Vec2::ZERO);
//    layer->addChild(menu, 1);
//
//    
//    // run
    auto scene = LogerScene::createScene( std::bind(&AppDelegateBox::runXXXXXX, this) );
    director->runWithScene(scene);

    return true;
}

void AppDelegateBox::menuCloseCallback(Ref* pSender)
{
    //    Director::getInstance()->end();
    //
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    exit(0);
    //#endif
    //    AppDelegate::runScript();
//    FileUtils::getInstance()->addSearchPath("/Users/rains/Documents/cocos/game/", true);
    ScriptingCore::getInstance()->runScript("main.js");
//    AppDelegate app;
//    Application::getInstance()->run();
//    Director::getInstance()->restart();
//    Director::getInstance()->getOpenGLView()->end();
//    auto glview = cocos2d::GLViewImpl::createWithRect("test10js", Rect(0,0,960/2,640/2));
//    
//    Director::getInstance()->setOpenGLView(glview);
//    Director::getInstance()->getOpenGLView()->setFrameSize(500, 700);
    
}

void AppDelegateBox::runXXXXXX()
{
    FileUtils::getInstance()->addSearchPath(LogerSingleton::getInstance()->getProjectPath(), true);
    ScriptingCore::getInstance()->runScript("main.js");
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegateBox::applicationDidEnterBackground()
{
    auto director = Director::getInstance();
    director->stopAnimation();
    director->getEventDispatcher()->dispatchCustomEvent("game_on_hide");
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();    
}

// this function will be called when the app is active again
void AppDelegateBox::applicationWillEnterForeground()
{
    auto director = Director::getInstance();
    director->startAnimation();
    director->getEventDispatcher()->dispatchCustomEvent("game_on_show");
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
}

void AppDelegateBox::runScript()
{
        ScriptEngineProtocol *engine = ScriptingCore::getInstance();
        ScriptEngineManager::getInstance()->setScriptEngine(engine);
        ScriptingCore::getInstance()->runScript("main.js");
}
