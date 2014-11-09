#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "victorianrushhour/Terrain.h"
#include "victorianrushhour/Player.h"

USING_NS_CC;

typedef enum {
    kGameIntro,
    kGamePlay,
    kGameOver
    
} GameState;


class GameLayer : public cocos2d::Layer
{
    
    Terrain * _terrain;
    Player * _player;
    
    SpriteBatchNode * _gameBatchNode;
    
    Size _screenSize;
    
    bool _running;
    int _speedIncreaseInterval;
	float _speedIncreaseTimer;
    
    void createGameScreen();
    void resetGame();
    
    
public:
    
    ~GameLayer();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::Scene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameLayer);
    
    void update (float dt);
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
};


#endif // __GAMESCENE_H__


