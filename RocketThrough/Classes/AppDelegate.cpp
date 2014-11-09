//
//  Rocket_ThroughAppDelegate.cpp
//  Rocket Through
//
//  Created by Roger Engelbert
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//

/*
 
 Background music:
 8bit Dungeon Level by Kevin MacLeod (incompetech.com)
 
 */

#include "AppDelegate.h"

#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto pDirector = Director::getInstance();
	auto pEGLView = pDirector->getOpenGLView();
	if(!pEGLView) {
		pEGLView = GLView::create("My Game");
		pDirector->setOpenGLView(pEGLView);
	}
    
    Size screenSize = pEGLView->getFrameSize();
    Size designSize = Size(768, 1024);
    pEGLView->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);

    float screenRatio = screenSize.height / screenSize.width;
    auto fileUtils = FileUtils::getInstance();
    std::vector<std::string> resDirOrders;

    if (screenSize.width > 768) {
    	resDirOrders.push_back("ipadhd");
    } else if (screenSize.width > 320) {
        if (screenRatio >= 1.5f) {
        	resDirOrders.push_back("iphonehd");
        } else {
        	resDirOrders.push_back("ipad");
        }
    } else {
    	resDirOrders.push_back("iphone");
    }
    pDirector->setContentScaleFactor(screenSize.height/designSize.height);

    fileUtils->setSearchPaths(resDirOrders);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("background.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pickup.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bombRelease.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("rocket.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("shipBoom.wav");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.4f);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5f);
    

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = GameLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
