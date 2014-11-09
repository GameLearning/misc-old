/*
 
 Background music:
 8bit Dungeon Level by Kevin MacLeod (incompetech.com)
 
 */

#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "rocketthrough/Rocket.h"
#include "rocketthrough/LineContainer.h"


USING_NS_CC;


GameLayer::~GameLayer () {
    CC_SAFE_RELEASE(_planets);
}

CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	//init game values
	_screenSize = Director::getInstance()->getWinSize();
    _drawing = false;
    _minLineLength = _screenSize.width * 0.07f;
    _state = kGameIntro;
    
    createGameScreen();
    
    createParticles();
    
    createStarGrid();
    
    auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    //create main loop
    this->schedule(schedule_selector(GameLayer::update));
    
    
    
    return true;
}

void GameLayer::update (float dt) {
	if (!_running) return;

	if (_lineContainer->getLineType() != LINE_NONE) {
	    _lineContainer->setTip (_rocket->getPosition() );
	}

	//track collision with sides
	if (_rocket->collidedWithSides()) {
	    _lineContainer->setLineType ( LINE_NONE );
	}

	_rocket->update(dt);

	//update jet particle so it follows rocket
	if (!_jet->isActive()) _jet->resetSystem();
	_jet->setRotation(_rocket->getRotation());
	_jet->setPosition(_rocket->getPosition());

	_cometTimer += dt;
	float newY;

	if (_cometTimer > _cometInterval) {
	    _cometTimer = 0;
	    if (_comet->isVisible() == false) {
	        _comet->setPositionX(0);
	        newY = (float)rand()/((float)RAND_MAX/_screenSize.height * 0.6f) + _screenSize.height * 0.2f;
	        if (newY > _screenSize.height * 0.9f)
	           newY = _screenSize.height * 0.9f;
	           _comet->setPositionY(newY);
	           _comet->setVisible(true);
	           _comet->resetSystem();
	    }
	}

	if (_comet->isVisible()) {
	    //collision with comet
	    if (pow(_comet->getPositionX() - _rocket->getPositionX(), 2) + pow(_comet->getPositionY() - _rocket->getPositionY(), 2) <= pow (_rocket->getRadius() , 2)) {
	        if (_rocket->isVisible()) killPlayer();
	    }
	    _comet->setPositionX(_comet->getPositionX() + 50 * dt);

	    if (_comet->getPositionX() > _screenSize.width * 1.5f) {
	        _comet->stopSystem();
	        _comet->setVisible(false);
	    }
	}


	_lineContainer->update(dt);
	_rocket->setOpacity(_lineContainer->getEnergy() * 255);

	int count = _planets->count();
	GameSprite * planet;
	for (int i = 0; i < count; i++) {
	    planet = (GameSprite *) _planets->objectAtIndex(i);
	    if (pow(planet->getPositionX() - _rocket->getPositionX(), 2)
	    + pow(planet->getPositionY() - _rocket->getPositionY(), 2) <=   pow (_rocket->getRadius() * 0.8f + planet->getRadius() * 0.65f, 2)) {

	        if (_rocket->isVisible()) killPlayer();
	        break;
	    }
	}

	if (pow(_star->getPositionX() - _rocket->getPositionX(), 2)
	    + pow(_star->getPositionY() - _rocket->getPositionY(), 2) <=
	    pow (_rocket->getRadius() * 1.2f, 2)) {

	    _pickup->setPosition(_star->getPosition());
	    _pickup->resetSystem();
	    if (_lineContainer->getEnergy() + 0.25f < 1) {
	        _lineContainer->setEnergy(_lineContainer->getEnergy() + 0.25f);
	    } else {
	        _lineContainer->setEnergy(1.0);
	    }
	    _rocket->setSpeed(_rocket->getSpeed() + 2);
	    if (_rocket->getSpeed() > 70) _rocket->setSpeed(70);
	        _lineContainer->setEnergyDecrement(0.0002f);
	        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pickup.wav");
	        resetStar();

	        int points = 100 - _timeBetweenPickups;
	        if (points < 0) points = 0;

	        _score += points;
	        CCString * value = CCString::createWithFormat("%i", _score);
	        _scoreDisplay->setString(value->getCString());

	        _timeBetweenPickups = 0;
	}

	_timeBetweenPickups += dt;
	if (_lineContainer->getEnergy() == 0) {
	    if (_rocket->isVisible()) killPlayer();
	}
}

bool GameLayer::onTouchBegan(Touch *touch, Event *event) {
	if (!_running) return true;
	Vec2 tap = touch->getLocation();
	//track if tapping on ship
	float dx = _rocket->getPositionX() - tap.x;
	float dy = _rocket->getPositionY() - tap.y;

	if (dx * dx + dy * dy <= pow(_rocket->getRadius() * 1.2, 2) ) {
	   //clear lines
	   _lineContainer->setLineType ( LINE_NONE );
	   _rocket->setRotationOrientation ( ROTATE_NONE );
	   _drawing = true;
	}
	return true;
}

void GameLayer::onTouchMoved(Touch *touch, Event *event) {
	if (!_running) return;

	if (_drawing) {
		Vec2 tap = touch->getLocation();
		float dx = _rocket->getPositionX() - tap.x;
		float dy = _rocket->getPositionY() - tap.y;

		if (dx * dx + dy * dy > pow (_minLineLength, 2)) {
			_rocket->select(true);
			_lineContainer->setPivot ( tap );
			_lineContainer->setLineType ( LINE_TEMP );
		} else {
			_rocket->select(false);
			_lineContainer->setLineType ( LINE_NONE );
		}
	}
}

void GameLayer::onTouchEnded(Touch *touch, Event *event) {
    
    if (_state == kGameIntro) {
        
        _intro->setVisible(false);
        _pauseBtn->setVisible(true);
        _state = kGamePlay;
        resetGame();
        return;
        
    } else if (_state == kGameOver) {
        
        _gameOver->setVisible(false);
        _pauseBtn->setVisible(true);
        _state = kGamePlay;
        resetGame();
        return;
        
    } else if (_state == kGamePaused) {
        
        _pauseBtn->setDisplayFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("btn_pause_off.png"));
        _paused->setVisible(false);
        _state = kGamePlay;
        _running = true;
        return;
    }
    
    if (!_running) return;

	Vec2 tap = touch->getLocation();
	if (_pauseBtn->boundingBox().containsPoint(tap)) {
		_paused->setVisible(true);
		_state = kGamePaused;
		_pauseBtn->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName ("btn_pause_on.png"));
		_running = false;
		return;
	}

	_drawing = false;
	_rocket->select(false);

	//if we are showing a temp line
	if (_lineContainer->getLineType() == LINE_TEMP) {
	   _lineContainer->setPivot (tap);
	   _lineContainer->setLineLength ( _rocket->getPosition().getDistance(tap) );
	   //set up rocket
	   _rocket->setPivot (tap);
	   float circle_length = _lineContainer->getLineLength() * 2 * M_PI;
	   int iterations = floor(circle_length / _rocket->getSpeed());
	   _rocket->setAngularSpeed ( 2 * M_PI / iterations);

	   Vec2 clockwise = ccpRPerp(_rocket->getPosition() - _rocket->getPivot());

	   float dot = ccpDot ( clockwise, _rocket->getVector() );

	   if (dot > 0) {
		   _rocket->setAngularSpeed ( _rocket->getAngularSpeed() * -1 );
		   _rocket->setRotationOrientation ( ROTATE_CLOCKWISE );
		   _rocket->setTargetRotation ( CC_RADIANS_TO_DEGREES( atan2(clockwise.y, clockwise.x) ) );
	   } else {
		   _rocket->setRotationOrientation ( ROTATE_COUNTER );
		   _rocket->setTargetRotation ( CC_RADIANS_TO_DEGREES (atan2(-1 * clockwise.y, -1 * clockwise.x) ) );
	   }
	   _lineContainer->setLineType ( LINE_DASHED );
	}
}

void GameLayer::resetGame () {

    _rocket->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.1f));
    _rocket->setOpacity(255);
    _rocket->setVisible(true);
    _rocket->reset();
    
    _cometInterval = 4;
    _cometTimer = 0;
    _timeBetweenPickups = 0.0;
    
    _score = 0;
    char szValue[100] = {0};
    sprintf(szValue, "%i", _score);
    _scoreDisplay->setString(szValue);
    
    _lineContainer->reset();
    
    //shuffle grid cells
    std::random_shuffle(_grid.begin(), _grid.end());
    _gridIndex = 0;
    
    resetStar();
    
    _warp->stopSystem();
    
    _running = true;
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.mp3", true);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("rocket.wav", true);

}

void GameLayer::resetStar() {
	 CCPoint position = _grid[_gridIndex];
	    _gridIndex++;
	    if (_gridIndex == _grid.size()) _gridIndex = 0;
	    //reset star particles
	    _star->setPosition(position);
	    _star->setVisible(true);
	    _star->resetSystem();
}

void GameLayer::killPlayer() {
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("shipBoom.wav");

	//update our particle systems, _boom and _jet
	_boom->setPosition(_rocket->getPosition());
	_boom->resetSystem();
	_rocket->setVisible(false);
	_jet->stopSystem();
	_lineContainer->setLineType ( LINE_NONE );

	_running = false;
	_state = kGameOver;
	_gameOver->setVisible(true);
	_pauseBtn->setVisible(false);
}


void GameLayer::createGameScreen () {
    
    Sprite * bg = Sprite::create("bg.png");
    bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg, kBackground);
    
    
    _lineContainer = LineContainer::create();
    this->addChild(_lineContainer);
    
    
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    _gameBatchNode = SpriteBatchNode::create("sprite_sheet.png", 100);

    this->addChild(_gameBatchNode, kForeground);
    
    _rocket = Rocket::create();
    _rocket->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.1f));
    _gameBatchNode->addChild(_rocket, kForeground, kSpriteRocket);
    

    //add planets
    GameSprite * planet;
    _planets = CCArray::create();
    _planets->retain();
    
    planet = GameSprite::createWithFrameName("planet_1.png");
    planet->setPosition(ccp(_screenSize.width * 0.25f,
                            _screenSize.height * 0.8f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets->addObject(planet);
    
    planet = GameSprite::createWithFrameName("planet_2.png");
    planet->setPosition(ccp(_screenSize.width * 0.8f,
                            _screenSize.height * 0.45f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets->addObject(planet);
    
    planet = GameSprite::createWithFrameName("planet_3.png");
    planet->setPosition(ccp(_screenSize.width * 0.75f,
                            _screenSize.height * 0.8f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets->addObject(planet);
    
    planet = GameSprite::createWithFrameName("planet_4.png");
    planet->setPosition(ccp(_screenSize.width * 0.5f,
                            _screenSize.height * 0.5f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets->addObject(planet);
    
    planet = GameSprite::createWithFrameName("planet_5.png");
    planet->setPosition(ccp(_screenSize.width * 0.18f,
                            _screenSize.height * 0.45f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets->addObject(planet);
    
    planet = GameSprite::createWithFrameName("planet_6.png");
    planet->setPosition(ccp(_screenSize.width * 0.8f,
                            _screenSize.height * 0.15f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets->addObject(planet);
    
    planet = GameSprite::createWithFrameName("planet_7.png");
    planet->setPosition(ccp(_screenSize.width * 0.18f,
                            _screenSize.height * 0.1f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets->addObject(planet);
    
    Sprite * scoreLabel = Sprite::createWithSpriteFrameName("label_score.png");
    scoreLabel->setPosition(Vec2(_screenSize.width * 0.4f, _screenSize.height * 0.95));
    _gameBatchNode->addChild(scoreLabel, kBackground);
    
    _scoreDisplay = CCLabelBMFont::create("0", "font.fnt", _screenSize.width * 0.5f, kCCTextAlignmentLeft);
    _scoreDisplay->setAnchorPoint(ccp(0,0.5));
    _scoreDisplay->setPosition(ccp(_screenSize.width * 0.48f, _screenSize.height * 0.95f));
    this->addChild(_scoreDisplay, kBackground);
    
    _pauseBtn = GameSprite::createWithFrameName("btn_pause_off.png");
    _pauseBtn->setVisible(false);
    _pauseBtn->setPosition(ccp(_screenSize.width * 0.06f, _screenSize.height * 0.95f));
    _gameBatchNode->addChild(_pauseBtn, kBackground);
    
    
    _intro = Sprite::createWithSpriteFrameName("logo.png");
    _intro->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    Sprite *play = Sprite::createWithSpriteFrameName("label_play.png");
    play->setPosition(ccp(_intro->boundingBox().size.width * 0.5f, -_intro->boundingBox().size.height * 0.5f));
    _intro->addChild(play);
    _gameBatchNode->addChild(_intro, kForeground);
    
    _gameOver = CCSprite::createWithSpriteFrameName("gameOver.png");
    _gameOver->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    _gameOver->setVisible(false);
    _gameBatchNode->addChild(_gameOver, kForeground);
    
    _paused = CCSprite::createWithSpriteFrameName("label_paused.png");
    _paused->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    _paused->setVisible(false);
    _gameBatchNode->addChild(_paused, kForeground);
}


void GameLayer::createParticles() {
	_jet = ParticleSystemQuad::create("jet.plist");
	_jet->setSourcePosition(Vec2(-_rocket->getRadius() * 0.8f,0));
	_jet->setAngle(180);
	_jet->stopSystem();
	this->addChild(_jet, kBackground);

	_boom = ParticleSystemQuad::create("boom.plist");
	_boom->stopSystem();
	this->addChild(_boom, kForeground);

	_comet = ParticleSystemQuad::create("comet.plist");
	_comet->stopSystem();
	_comet->setPosition(ccp(0, _screenSize.height * 0.6f));
	_comet->setVisible(false);
	this->addChild(_comet, kForeground);

	_pickup = ParticleSystemQuad::create("plink.plist");
	_pickup->stopSystem();
	this->addChild(_pickup, kMiddleground);

	_warp = ParticleSystemQuad::create("warp.plist");
	_warp->setPosition(_rocket->getPosition());
	this->addChild(_warp, kBackground);

	_star = ParticleSystemQuad::create("star.plist");
	_star->stopSystem();
	_star->setVisible(false);
	this->addChild(_star, kBackground, kSpriteStar);

}

void GameLayer::createStarGrid() {
    //create grid
    float gridFrame = _screenSize.width * 0.1f;
    int tile = 32;
    int rows = (_screenSize.height - 4 * gridFrame)/tile;
    int cols = (_screenSize.width  - 2 * gridFrame)/tile;
    
    int count = _planets->count();
    GameSprite * planet;
    Vec2 cell;
    bool overlaps;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            cell = Vec2(gridFrame + c * tile, 2 * gridFrame + r * tile);
            overlaps = false;
            for (int j = 0; j < count; j++) {
                planet = (GameSprite *) _planets->objectAtIndex(j);
                if (pow(planet->getPositionX() - cell.x, 2) + pow(planet->getPositionY() - cell.y, 2) <= pow(planet->getRadius() * 1.2f, 2)) {
                    overlaps = true;
                }
            }
            if (!overlaps) _grid.push_back(cell);
        }
    }
    CCLOG("POSSIBLE STARS: %i", _grid.size());
    
}
