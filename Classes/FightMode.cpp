#include "FightMode.h"

USING_NS_CC;

Scene* FightMode::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = FightMode::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool FightMode::init()
{

	if (!Layer::init())
	{
		return false;
	}
	//变量初始化
	player1IsMove = false;
	lastkey1 = 'D';
	player2IsMove = false;
	lastkey2 = 'A';

	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("FightBackground.JPG");
	bg->setPosition(origin.x, origin.y);
	bg->setAnchorPoint(Vec2(0, 0));
	float bgx = bg->getTextureRect().getMaxX();
	float bgy = bg->getTextureRect().getMaxY();
	bg->setScaleX(visibleSize.height / bgy);
	bg->setScaleY(visibleSize.height / bgy);
	this->addChild(bg, 0);
	//创建一张玩家1的贴图
	auto texture1 = Director::getInstance()->getTextureCache()->addImage("player1/firzen_0.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame1 = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 81, 81)));
	//使用第一帧创建精灵
	player1 = Sprite::createWithSpriteFrame(frame1);
	player1->setPosition(Vec2(visibleSize.width / 4 + origin.x, origin.y + player1->getContentSize().height + 30));
	//设置缩放比例
	Size player1Size = player1->getContentSize();
	float scaleX = visibleSize.width * 0.1 / player1Size.width;
	player1->setScale(scaleX, scaleX);
	this->addChild(player1, 2);

	// player1静态动画
	player1Idle.reserve(1);
	player1Idle.pushBack(frame1);

	//创建一张玩家2的贴图
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("player2/woody_0.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame2 = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 81, 81)));
	//使用第一帧创建精灵
	player2 = Sprite::createWithSpriteFrame(frame2);
	player2->setPosition(Vec2(3 * visibleSize.width / 4 + origin.x, origin.y + player2->getContentSize().height + 30));
	player2->setFlippedX(true);
	//设置缩放比例
	Size player2Size = player2->getContentSize();
	float scale2X = visibleSize.width * 0.1 / player2Size.width;
	player2->setScale(scale2X, scale2X);
	this->addChild(player2, 2);


	addKeyboardListener();
	schedule(schedule_selector(FightMode::update), 0.04f, kRepeatForever, 0);
	// player2静态动画
	player2Idle.reserve(1);
	player2Idle.pushBack(frame2);

	// player1手攻击动画
	player1AttackHand.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(81 * i, 81, 81, 81)));
		player1AttackHand.pushBack(frame);
	}

	//player1脚攻击动画
	player1AttackLeg.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(81 * i + 80 * 4, 81, 81, 81)));
		player1AttackLeg.pushBack(frame);
	}

	//player1移动动画
	player1Move.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(81 * i , 162, 81, 81)));
		player1Move.pushBack(frame);
	}

	//player1死亡动画
	player1Dead.reserve(5);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(81 * i, 243, 81, 81)));
		player1Dead.pushBack(frame);
	}

	//player1气功动画
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("player1/firzen_1.png");
	player1AttackQigong.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 162, 79, 81)));
		player1AttackQigong.pushBack(frame);
	}

	// 气功形状变化动画
	auto texture4 = Director::getInstance()->getTextureCache()->addImage("player1/firzen_ball.png");
	player1Qigong.reserve(8);
	for (int i = 0; i < 8; i++) {
		int j = 0, k = i;
		if (i >= 4) {
			j = 1;
			k = i - 4;
		}
		auto frame = SpriteFrame::createWithTexture(texture4, CC_RECT_PIXELS_TO_POINTS(Rect(128 * k, j * 80, 128, 70)));
		player1Qigong.pushBack(frame);
	}

	//防御动画
	player1Defense.reserve(5);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i + 79 * 3, 0, 79, 81)));
		player1Defense.pushBack(frame);
	}

	return true;
}

void FightMode::player1AttackByHand(Ref* pSender) {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1AttackHand, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player1AttackByLeg(Ref* pSender) {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1AttackLeg, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player1MoveAnimation(Ref* pSender) {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1Move, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player1WouldDead(Ref* pSender) {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1Dead, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player1WouldDefense(Ref* pSender) {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1Defense, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

//运行气功动画并产生气功精灵移动
void FightMode::player1AttackByQigong(Ref* pSender) {

	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1AttackQigong, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
	}
	auto texture = Director::getInstance()->getTextureCache()->addImage("player1/firzen_ball.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 128, 70)));
	auto qigong = Sprite::createWithSpriteFrame(frame);
	qigong->setPosition(Vec2(player1->getPositionX() + 30, player1->getPositionY()));
	this->addChild(qigong, 2);
	auto animation3 = Animation::createWithSpriteFrames(player1Qigong, 0.5f);
	auto animate3 = Animate::create(animation3);
	qigong->runAction(Sequence::create(animate3, NULL));
}

void FightMode::update(float f) {
	if (player1IsMove) {
		this->player1Movement(player1Movekey);
	}
}

//人物移动函数
void FightMode::addKeyboardListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(FightMode::onKeyPressed1, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(FightMode::onKeyReleased1, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, player1);
}

void FightMode::onKeyPressed1(EventKeyboard::KeyCode code, Event* event) {
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		player1IsMove = true;
		player1Movekey = 'A';
		if (lastkey1 == 'D') {
			player1->setFlippedX(true);
		}
		lastkey1 = 'A';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
		player1IsMove = true;
		player1Movekey = 'D';
		if (lastkey1 == 'A') {
			player1->setFlippedX(false);
		}
		lastkey1 = 'D';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
		player1IsMove = true;
		player1Movekey = 'W';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
		player1IsMove = true;
		player1Movekey = 'S';
		break;
	}
}
void FightMode::onKeyReleased1(EventKeyboard::KeyCode code, Event *event) {
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
		player1IsMove = false;
		break;
	}
}

void FightMode::player1Movement(char key) {
	switch (key)
	{
	case 'A':
		if (player1->getPositionX() - 15 >= 30) {
			player1->setPosition(player1->getPositionX() - 15, player1->getPositionY());
		}
		else {
			player1->setPosition(30, player1->getPositionY());
		}
		break;
	case 'D':
		if (player1->getPositionX() + 15 <=  visibleSize.width - 30) {
			player1->setPosition(player1->getPositionX() + 15, player1->getPositionY());
		}
		else {
			player1->setPosition(visibleSize.width - 30, player1->getPositionY());
		}
		break;
	case 'W':
		if (player1->getPositionY() + 10 <= 280) {
			player1->setPosition(player1->getPositionX(), player1->getPositionY() + 10);
		}
		else {
			player1->setPosition(player1->getPositionX(), 280);
		}
		break;
	case 'S':
		if (player1->getPositionY() - 10 >= 80) {
			player1->setPosition(player1->getPositionX(), player1->getPositionY() - 10);
		}
		else {
			player1->setPosition(player1->getPositionX(), 70);
		}
		break;
	}
}