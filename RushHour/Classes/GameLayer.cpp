#include "GameLayer.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;

GameLayer::~GameLayer() {
    
}

CCScene* GameLayer::scene()
{
    Scene *scene = Scene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() ) {
        return false;
    }
    
    //get screen size
    _screenSize = Director::getInstance()->getWinSize();
    
    createGameScreen();
    
    resetGame();
    auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
	listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    //create main loop
    this->schedule(schedule_selector(GameLayer::update));
    
    return true;
}

void GameLayer::resetGame () {
    
    _speedIncreaseInterval = 15;
	_speedIncreaseTimer = 0;
    
    _running = true;
}


void GameLayer::update(float dt) {
	if (!_running) return;


	if (_player->getPositionY() < -_player->getHeight() ||
		_player->getPositionX() < -_player->getWidth() * 0.5f) {

			_running = false;

	}

	_player->update(dt);

	_terrain->move(_player->getVector().x);

	if (_player->getState() != kPlayerDying)
		_terrain->checkCollision(_player);

	_player->place();



	if (_player->getNextPosition().y > _screenSize.height * 0.6f) {
	        _gameBatchNode->setPositionY( (_screenSize.height * 0.6f - _player->getNextPosition().y) * 0.8f);

	    } else {
	        _gameBatchNode->setPositionY  ( 0 );
	    }


	if (_terrain->getStartTerrain() && _player->getVector().x > 0) {

	        _speedIncreaseTimer += dt;
	        if (_speedIncreaseTimer > _speedIncreaseInterval) {
	            _speedIncreaseTimer = 0;
	            _player->setMaxSpeed (_player->getMaxSpeed() + 4);
	        }
	    }
    
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event) {
	if (!_running) {

		if (_player->getState() == kPlayerDying) {
			_terrain->reset();
			_player->reset();
			resetGame();
		}
		return;
	}

	if (!_terrain->getStartTerrain()) {
		_terrain->setStartTerrain ( true );
		return;
	}

	if (_player->getState() == kPlayerFalling) {
		_player->setFloating ( _player->getFloating() ? false : true );
	} else {
		if (_player->getState() !=  kPlayerDying) _player->setJumping(true);
	}
}

void GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event){
	 _player->setJumping(false);
}

void GameLayer::createGameScreen () {

    _gameBatchNode = SpriteBatchNode::create("blank.png", 200);
    this->addChild(_gameBatchNode, kMiddleground);
    
    _terrain = Terrain::create();
    _gameBatchNode->addChild(_terrain, kMiddleground);
    
    _player = Player::create();
    _gameBatchNode->addChild(_player, kBackground);
    
}
