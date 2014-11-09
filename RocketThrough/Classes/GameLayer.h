#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__


#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

enum {
    kBackground,
    kMiddleground,
    kForeground
};

enum {
    kSpriteRocket,
    kSpritePlanet,
    kSpriteBoost,
    kSpriteStar
};

typedef enum gamestates {
    kGameIntro,
    kGamePaused,
    kGamePlay,
    kGameOver
    
}GameState;

class Rocket;
class LineContainer;
class GameSprite;


class GameLayer : public Layer {
    
	
    Rocket * _rocket;
    LineContainer * _lineContainer;
    
    SpriteBatchNode *_gameBatchNode;
    LabelBMFont * _scoreDisplay;
    
    GameSprite * _pauseBtn;
    Sprite * _intro;
    Sprite *_gameOver;
    Sprite *_paused;
    
    ParticleSystem * _star;
    ParticleSystem * _jet;
    ParticleSystem * _boom;
    ParticleSystem * _comet;
    ParticleSystem * _pickup;
    ParticleSystem * _warp;
    
    CCArray * _planets;
    Size _screenSize;
    
    GameState _state;
    
    bool _drawing;
    bool _running;

    std::vector<Vec2> _grid;
    int _gridIndex;
    
    int _minLineLength;
    float _cometInterval;
    float _cometTimer;
    
    void resetGame(void);
    void resetStar(void);
    void killPlayer(void);
    
    void createGameScreen(void);
    void createParticles(void);
    void createStarGrid(void);

	int _score;
	float _timeBetweenPickups;
    
public:
    ~GameLayer(void);
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    void update (float dt);
	
	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchMoved(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch *touch, Event *event);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameLayer);
	

};

#endif // __GAMELAYER_H__
