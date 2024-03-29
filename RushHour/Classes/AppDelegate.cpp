//
//  Victorian_Rush_HourAppDelegate.cpp
//  Victorian Rush Hour
//
//  Created by Roger Engelbert on 11/17/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;

AppDelegate::AppDelegate(){

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
	Size designSize = Size(2048, 1536);

	pEGLView->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);

	if (screenSize.height > 768) {
		pDirector->setContentScaleFactor(1536/designSize.height);
	} else if (screenSize.height > 320) {
		pDirector->setContentScaleFactor(768/designSize.height);
	} else {
		pDirector->setContentScaleFactor(380/designSize.height);
	}
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    Scene *pScene = GameLayer::scene();

    // run
    pDirector->runWithScene(pScene);

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
