//
//  Sky_DefenseAppDelegate.cpp
//  Sky Defense
//
//  Created by Roger Engelbert.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//
//
// Background music:
// Blipotron by Kevin MacLeod (incompetech.com)
//
//
/*
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
    Director* director = Director::sharedDirector();
    auto glview = director->getOpenGLView();
	if(!glview) {
		glview = GLView::create("My Game");
		director->setOpenGLView(glview);
	}
    
    director->setOpenGLView(glview);
    
    auto designSize = Size(2048, 1536);
	auto screenSize = glview->getFrameSize();
	std::vector<std::string> resDirOrders;

	director->setContentScaleFactor(screenSize.height/designSize.height);
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);

	if (screenSize.height > 768) {
		resDirOrders.push_back("ipadhd");
	} else if (screenSize.height > 320) {
		resDirOrders.push_back("ipad");
	} else {
		resDirOrders.push_back("iphone");
	}

	FileUtils::getInstance()->setSearchPaths(resDirOrders);
    
    //preload sound effects and background music
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("background.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bombFail.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bombRelease.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("boom.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("health.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("fire_truck.wav");
    
    //lower playback volume for effects
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.4f);
    
    // turn on display FPS
	director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = GameLayer::scene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->pause();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
*/

#include "AppDelegate.h"
#include "GameLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    auto designSize = Size(2048, 1536);
	auto screenSize = glview->getFrameSize();
	std::vector<std::string> resDirOrders;

	director->setContentScaleFactor(screenSize.height/designSize.height);
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);

	if (screenSize.height > 768) {
		resDirOrders.push_back("ipadhd");
	} else if (screenSize.height > 320) {
		resDirOrders.push_back("ipad");
	} else {
		resDirOrders.push_back("iphone");
	}

	FileUtils::getInstance()->setSearchPaths(resDirOrders);

	//preload sound effects and background music
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("background.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bombFail.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bombRelease.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("boom.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("health.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("fire_truck.wav");

	//lower playback volume for effects
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.4f);

    // create a scene. it's an autorelease object
    auto scene = GameLayer::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

