#ifndef __GAMESPRITE_H__
#define __GAMESPRITE_H__
#include "cocos2d.h"

USING_NS_CC;

class GameSprite : public CCSprite {
public:

    CC_SYNTHESIZE(Vec2, _nextPosition, NextPosition);
    CC_SYNTHESIZE(Vec2, _vector, Vector);
    CC_SYNTHESIZE(CCTouch *, _touch, Touch);
   
   GameSprite(void);
   ~GameSprite(void);
    
   static GameSprite* gameSpriteWithFile(const char * pszFileName);

   virtual void setPosition(const Vec2& pos);
   float radius();
    
};
#endif // __GAMESPRITE_H__