#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

enum {
    kSpriteBomb,
    kSpriteShockwave,
    kSpriteMeteor,
    kSpriteHealth,
    kSpriteHalo,
    kSpriteSparkle
};

enum {
    kBackground,
    kMiddleground,
    kForeground
};


class GameLayer : public Layer {
private:

	CCArray * _meteorPool;
    int _meteorPoolIndex;
    
    CCArray * _healthPool;
    int _healthPoolIndex;
    
    
	CCArray * _fallingObjects;
    CCArray * _clouds;

    SpriteBatchNode * _gameBatchNode;
    Sprite * _bomb;
    Sprite * _shockWave;
    
    Sprite * _introMessage;
    Sprite * _gameOverMessage;
    
    LabelBMFont * _energyDisplay;
    LabelBMFont * _scoreDisplay;
    
    Action * _growBomb;
    Action * _rotateSprite;
    Action * _shockwaveSequence;
    Action * _swingHealth;

    Action * _groundHit;
    Action * _explosion;
    
    
    Size _screenSize;
	
	float _meteorInterval;
	float _meteorTimer;
	float _meteorSpeed;
    float _healthInterval;
    float _healthTimer;
    float _healthSpeed;
    float _difficultyInterval;
    float _difficultyTimer;

	int _energy;
    int _score;
    int _shockwaveHits;
    bool _running;
    
	void resetMeteor(void);
    void resetHealth(void);
    void resetGame(void);
    void stopGame(void);
    void increaseDifficulty(void);
    
    void createGameScreen (void);
    void createPools(void);
    void createActions(void);
	
public:
    
    ~GameLayer(void);
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static Scene* createScene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameLayer);
    
    void fallingObjectDone(CCNode* pSender);
    void animationDone(CCNode* pSender);
    void shockwaveDone();
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
   
    virtual void update (float dt);
};

#endif // __GAMELAYER_H__
