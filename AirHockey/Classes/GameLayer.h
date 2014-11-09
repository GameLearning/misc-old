#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#define GOAL_WIDTH 400

#include "cocos2d.h"
#include "GameSprite.h"

class GameLayer : public cocos2d::Layer
{
    GameSprite * _player1;
    GameSprite * _player2;
    GameSprite * _ball;
    
    CCArray * _players;
    CCLabelTTF * _player1ScoreLabel;
    CCLabelTTF * _player2ScoreLabel;

    CCSize _screenSize;

    int _player1Score;
    int _player2Score;
        
    void playerScore (int player);
public:
    ~GameLayer();
    
    virtual bool init();

    static CCScene* createScene();
    
    CREATE_FUNC(GameLayer);
    
    void onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    void update (float dt);
};
#endif // __GAMELAYER_H__