#include "FightMode.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

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
	//变量初始量
	x_num = 28;
	y_num = 23;
	player1ADIsMove = false;
	player1WSIsMove = false;
	lastkey1 = 'D';
	player2ADIsMove = false;
	player2WSIsMove = false;
	lastkey2 = 'A';

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	player1IsDefend = false;
	player2IsDefend = false;

	auto bg = Sprite::create("FightBackground.JPG");
	bg->setPosition(origin.x, origin.y);
	bg->setAnchorPoint(Vec2(0, 0));
	float bgx = bg->getTextureRect().getMaxX();
	float bgy = bg->getTextureRect().getMaxY();
	bg->setScaleX(visibleSize.height / bgy);
	bg->setScaleY(visibleSize.height / bgy);
	this->addChild(bg, 0);

	initAnimation();
	//preloadMusic();
	//playBgm();
	addKeyboardListener();
	addCustomListener();
	schedule(schedule_selector(FightMode::update), 0.1f, kRepeatForever, 0);
	schedule(schedule_selector(FightMode::update_numHit), 0.1f, kRepeatForever, 0);
	schedule(schedule_selector(FightMode::update_maxHit), 1.0f, kRepeatForever, 0);
	schedule(schedule_selector(FightMode::update_powerHit), 0.01f, kRepeatForever, 0);

	return true;
}

void FightMode::preloadMusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/play1/hand.wav");
	audio->preloadBackgroundMusic("music/play1/leg.wav");
	audio->preloadBackgroundMusic("music/play1/qigong.wav");
	audio->preloadBackgroundMusic("music/play2/hand.wav");
	audio->preloadBackgroundMusic("music/play2/leg.wav");
	audio->preloadBackgroundMusic("music/play2/qigong.wav");
	audio->preloadBackgroundMusic("music/background.wav");
	audio->preloadBackgroundMusic("music/BeingAttack.wav");
}

//播放背景音乐
void FightMode::playBgm() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("music/background.wav", true);
}

//初始化player1和player2的所有动画
void FightMode::initAnimation() {
	//创建一张玩家的贴图
	auto texture1 = Director::getInstance()->getTextureCache()->addImage("player1/firzen_0.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame1 = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(81 * 6, 0, 81, 81)));
	//使用第一帧创建精灵
	player1 = Sprite::createWithSpriteFrame(frame1);
	player1->setPosition(Vec2(visibleSize.width / 4 + origin.x, origin.y + player1->getContentSize().height + 42));
	//设置缩放比例
	Size player1Size = player1->getContentSize();
	float scaleX = visibleSize.width * 0.126 / player1Size.width;
	player1->setScale(scaleX, scaleX);
	this->addChild(player1, 2);

	// player1静态动画
	player1Idle.reserve(1);
	player1Idle.pushBack(frame1);

	//创建一张玩家的贴图
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("player2/woody_0.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame2 = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(81 * 6, 0, 81, 81)));
	//使用第一帧创建精灵
	player2 = Sprite::createWithSpriteFrame(frame2);
	player2->setPosition(Vec2(3 * visibleSize.width / 4 + origin.x, origin.y + player2->getContentSize().height + 30));
	player2->setFlippedX(true);

	//设置缩放比例
	Size player2Size = player2->getContentSize();
	float scale2X = visibleSize.width * 0.126 / player2Size.width;
	player2->setScale(scale2X, scale2X);
	this->addChild(player2, 2);

	//hp mp条
	Sprite* HPMP1 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(7, 14, 1414, 275)));
	Sprite* HP1 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(1480, 57, 24, 41)));
	Sprite* MP1 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(1480, 184, 24, 41)));
	Sprite* HPMP2 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(7, 293, 1414, 275)));
	Sprite* HP2 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(1480, 57, 24, 41)));
	Sprite* MP2 = Sprite::create("HPMP.png", CC_RECT_PIXELS_TO_POINTS(Rect(1480, 184, 24, 41)));

	//使用hp mp条设置progressBar
	HPpt1 = ProgressTimer::create(HP1);
	HPpt1->setScale(0.25);
	HPpt1->setScaleX(11.7);
	HPpt1->setAnchorPoint(Vec2(0, 0));
	HPpt1->setType(ProgressTimerType::BAR);
	HPpt1->setBarChangeRate(Point(1, 0));
	HPpt1->setMidpoint(Point(0, 1));
	HPpt1->setPercentage(100);
	HPpt1->setPosition(Vec2(origin.x + HPpt1->getContentSize().width + 237 * 0.23 + 1, origin.y + visibleSize.height - HPMP1->getContentSize().height * 0.134));
	addChild(HPpt1, 1);
	MPpt1 = ProgressTimer::create(MP1);
	MPpt1->setScale(0.25);
	MPpt1->setScaleX(11.7);
	MPpt1->setAnchorPoint(Vec2(0, 0));
	MPpt1->setType(ProgressTimerType::BAR);
	MPpt1->setBarChangeRate(Point(1, 0));
	MPpt1->setMidpoint(Point(0, 1));
	MPpt1->setPercentage(100);
	MPpt1->setPosition(Vec2(origin.x + HPpt1->getContentSize().width + 237 * 0.23 + 1, origin.y + visibleSize.height - HPMP1->getContentSize().height * 0.24));
	addChild(MPpt1, 1);

	HPMP1->setAnchorPoint(Vec2(0, 0));
	HPMP1->setPosition(Vec2(origin.x + HPpt1->getContentSize().width * 0.5, origin.y + visibleSize.height - HPMP1->getContentSize().height * 0.3));
	HPMP1->setScale(0.25);
	addChild(HPMP1, 0);


	HPpt2 = ProgressTimer::create(HP2);
	HPpt2->setScale(0.25);
	HPpt2->setScaleX(11.7);
	HPpt2->setAnchorPoint(Vec2(0, 0));
	HPpt2->setType(ProgressTimerType::BAR);
	HPpt2->setBarChangeRate(Point(1, 0));
	HPpt2->setMidpoint(Point(1, 0));
	HPpt2->setPercentage(100);
	HPpt2->setPosition(Vec2(visibleSize.width - (HPpt2->getContentSize().width * 14.56), origin.y + visibleSize.height - HPMP2->getContentSize().height * 0.134));
	addChild(HPpt2, 1);
	MPpt2 = ProgressTimer::create(MP2);
	MPpt2->setScale(0.25);
	MPpt2->setScaleX(11.7);
	MPpt2->setAnchorPoint(Vec2(0, 0));
	MPpt2->setType(ProgressTimerType::BAR);
	MPpt2->setBarChangeRate(Point(0, 1));
	MPpt2->setMidpoint(Point(0, 1));
	MPpt2->setPercentage(100);
	MPpt2->setPosition(Vec2(visibleSize.width - (MPpt2->getContentSize().width * 14.56), origin.y + visibleSize.height - HPMP2->getContentSize().height * 0.24));
	addChild(MPpt2, 1);

	HPMP2->setAnchorPoint(Vec2(0, 0));
	HPMP2->setPosition(Vec2(visibleSize.width - (origin.x + HPpt1->getContentSize().width + HPMP2->getContentSize().width * 0.24) + 4, origin.y + visibleSize.height - HPMP2->getContentSize().height * 0.3));
	HPMP2->setScale(0.25);
	addChild(HPMP2, 0);

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
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i + 320, 0, 80, 81)));
		player1Move.pushBack(frame);
	}
	//player1死亡动画
	player1Dead.reserve(5);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 243, 80, 81)));
		player1Dead.pushBack(frame);
	}
	//player1被袭击动画
	player1BeingAttacked.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i + 240 , 400, 80, 81)));
		player1BeingAttacked.pushBack(frame);
	}
	//player1发放气功动画
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("player1/firzen_1.png");
	player1AttackQigong.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 162, 79, 81)));
		player1AttackQigong.pushBack(frame);
	}
	//player1气功形状变化动画
	auto texture4 = Director::getInstance()->getTextureCache()->addImage("player1/firzen_chasei.png");
	player1Qigong.reserve(8);
	for (int i = 0; i < 8; i++) {
		int j = 0, k = i;
		if (i >= 4) {
			j = 1;
			k = i - 4;
		}
		auto frame = SpriteFrame::createWithTexture(texture4, CC_RECT_PIXELS_TO_POINTS(Rect(78 * k, j * 80, 78, 70)));
		player1Qigong.pushBack(frame);
	}
	//player1防御动画
	player1Defense.reserve(5);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i + 79 * 3, 0, 79, 81)));
		player1Defense.pushBack(frame);
	}


	//player2手攻击动画
	auto texture5 = Director::getInstance()->getTextureCache()->addImage("player2/woody_0.png");
	player2AttackHand.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture5, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 81, 80, 81)));
		player2AttackHand.pushBack(frame);
	}
	//player2脚攻击动画
	auto texturePlayer2 = Director::getInstance()->getTextureCache()->addImage("player2/woody_1.png");
	player2AttackLeg.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texturePlayer2, CC_RECT_PIXELS_TO_POINTS(Rect(560 + 80 * i, 243, 80, 81)));
		player2AttackLeg.pushBack(frame);
	}
	//player2移动动画
	player2Move.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture5, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i + 320, 0, 80, 81)));
		player2Move.pushBack(frame);
	}
	//player2死亡动画
	player2Dead.reserve(5);
	for (int i = 0; i < 5; i++) {
		auto frame = SpriteFrame::createWithTexture(texture5, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 243, 80, 81)));
		player2Dead.pushBack(frame);
	}
	//player2被袭击动画
	player2BeingAttacked.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texturePlayer2, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i , 400, 80, 81)));
		player2BeingAttacked.pushBack(frame);
	}
	//player2发放气功动画
	auto texture6 = Director::getInstance()->getTextureCache()->addImage("player2/woody_2.png");
	player2AttackQigong.reserve(10);
	for (int i = 0; i < 10; i++) {
		auto frame = SpriteFrame::createWithTexture(texture6, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 0, 80, 81)));
		player2AttackQigong.pushBack(frame);
	}
	//player2气功形状变化动画
	auto texture7 = Director::getInstance()->getTextureCache()->addImage("player2/woody_ball.png");
	player2Qigong.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture7, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 162, 80, 81)));
		player2Qigong.pushBack(frame);
	}
	//player2防御动画
	player2Defense.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(texture5, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i + 400, 405, 80, 81)));
		player2Defense.pushBack(frame);
	}
}

//player1运行气功动画并产生气功精灵移动
void FightMode::player1AttackByQigong() {
	auto callBack = CallFunc::create(CC_CALLBACK_0(FightMode::player1QiGong, this));
	if (true) {
		auto animation1 = Animation::createWithSpriteFrames(player1AttackQigong, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		Vector<SpriteFrame*> afterSendAnimation;
		afterSendAnimation.pushBack(player1AttackQigong.at(2));
		auto animation3 = Animation::createWithSpriteFrames(afterSendAnimation, 0.5f);
		auto animate3 = Animate::create(animation3);
		player1->runAction(Sequence::create(animate1, callBack, animate3, animate2, NULL));
	}
}

void FightMode::player1QiGong() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("player1/firzen_chasei.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 78, 70)));
	qigong1 = Sprite::createWithSpriteFrame(frame);
	qigong1->setPosition(Vec2(player1->getPositionX() + 36, player1->getPositionY()));
	if (lastkey1 == 'A')
		qigong1->setPosition(Vec2(player1->getPositionX() - 36, player1->getPositionY()));
	else
		qigong1->setPosition(Vec2(player1->getPositionX() + 36, player1->getPositionY()));
	//设置缩放比例
	Size qigongSize = qigong1->getContentSize();
	float scaleX = visibleSize.width * 0.126 / qigongSize.width;
	qigong1->setScale(scaleX, scaleX);
	this->addChild(qigong1, 2);
	auto animation3 = Animation::createWithSpriteFrames(player1Qigong, 0.23f);
	auto animate3 = Animate::create(animation3);
	auto moveBy = MoveBy::create(1.66, Vec2(visibleSize.width, 0));
	if (lastkey1 == 'A') {
		moveBy = MoveBy::create(1.66, Vec2(-visibleSize.width, 0));
	}
	else {
		moveBy = MoveBy::create(1.66, Vec2(visibleSize.width, 0));
	}
	auto spawn = Spawn::createWithTwoActions(animate3, moveBy);
	auto fadeout = FadeOut::create(0.01f);
	qigong1->runAction(Sequence::create(spawn, fadeout, NULL));
	player1QiGongNumber.push_back(qigong1);
}

//player2运行气功动画并产生气功精灵移动
void FightMode::player2AttackByQigong() {
	auto callBack = CallFunc::create(CC_CALLBACK_0(FightMode::player2QiGong, this));
	if (true) {
		auto animation1 = Animation::createWithSpriteFrames(player2AttackQigong, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		Vector<SpriteFrame*> afterSendAnimation;
		afterSendAnimation.pushBack(player2AttackQigong.at(8));
		auto animation3 = Animation::createWithSpriteFrames(afterSendAnimation, 0.3f);
		auto animate3 = Animate::create(animation3);
		player2->runAction(Sequence::create(animate1, callBack, animate3, animate2, NULL));
	}
}

void FightMode::player2QiGong() {
	auto texture = Director::getInstance()->getTextureCache()->addImage("player2/woody_ball.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 162, 80, 81)));
	qigong2 = Sprite::createWithSpriteFrame(frame);
	qigong2->setPosition(Vec2(player2->getPositionX() + 36, player2->getPositionY()));
	if (lastkey2 == 'A') {
		qigong2->setPosition(Vec2(player2->getPositionX() - 36, player2->getPositionY()));
		qigong2->setFlippedX(true);
	}
	else {
		qigong2->setPosition(Vec2(player2->getPositionX() + 36, player2->getPositionY()));
		qigong2->setFlippedX(false);
	}
	//设置缩放比例
	Size qigongSize = qigong2->getContentSize();
	float scaleX = visibleSize.width * 0.126 / qigongSize.width;
	qigong2->setScale(scaleX, scaleX);
	this->addChild(qigong2, 2);
	auto animation3 = Animation::createWithSpriteFrames(player2Qigong, 0.3f);
	auto animate3 = Animate::create(animation3);
	auto moveBy = MoveBy::create(1.3, Vec2(visibleSize.width, 0));
	if (lastkey2 == 'A') {
		moveBy = MoveBy::create(1.3, Vec2(-visibleSize.width, 0));
	}
	else {
		moveBy = MoveBy::create(1.5, Vec2(visibleSize.width, 0));
	}
	auto spawn = Spawn::createWithTwoActions(animate3, moveBy);
	auto fadeout = FadeOut::create(0.01f);
	qigong2->runAction(Sequence::create(spawn, fadeout, NULL));
	player2QiGongNumber.push_back(qigong2);
}

void FightMode::update(float f) {
	//weimumu
	if (player1ADIsMove || player1WSIsMove) {
		this->player1Movement(player1ADMovekey, player1WSMovekey);
	}
	if (player2ADIsMove || player2WSIsMove) {
		this->player2Movement(player2ADMovekey, player2WSMovekey);
	}


	//zzh

	//人物defence
	if (player2_defence) {
		if (player2IsDefend == false && player2->getNumberOfRunningActions() == 0) {
			auto animation1 = Animation::createWithSpriteFrames(player2Defense, 0.1f);
			auto animate1 = Animate::create(animation1);
			player2->runAction(Sequence::create(animate1, NULL));
			player2IsDefend = true;
		}
	}
	else {
		if (player2->getNumberOfRunningActions() == 0) {
			auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
			auto animate2 = Animate::create(animation2);
			player2->runAction(Sequence::create(animate2, NULL));
			player2IsDefend = false;
		}
	}

	if (player1_defence) {
		if (player1IsDefend == false && player1->getNumberOfRunningActions() == 0) {
			auto animation1 = Animation::createWithSpriteFrames(player1Defense, 0.1f);
			auto animate1 = Animate::create(animation1);
			player1->runAction(Sequence::create(animate1, NULL));
			player1IsDefend = true;
		}
	}
	else {
		if (player1->getNumberOfRunningActions() == 0) {
			auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
			auto animate2 = Animate::create(animation2);
			player1->runAction(Sequence::create(animate2, NULL));
			player1IsDefend = false;
		}
	}

	////Mp补充
	//auto mp1 = Mp1->getPercentage();
	//mp1 += 1;
	//auto mp1Action = ProgressTo::create(0.1, mp1);
	//Mp1->runAction(mp1Action);

	//auto mp2 = Mp2->getPercentage();
	//mp2 += 1;
	//auto mp2Action = ProgressTo::create(0.1, mp2);
	//Mp2->runAction(mp2Action);

	auto mp1 = MPpt1->getPercentage();
	mp1 += 1;
	auto mp1Action = ProgressTo::create(0.1, mp1);
	MPpt2->runAction(mp1Action);

	auto mp2 = MPpt2->getPercentage();
	mp2 += 1;
	auto mp2Action = ProgressTo::create(0.1, mp2);
	MPpt2->runAction(mp2Action);
}

//人物移动函数
void FightMode::addKeyboardListener() {
	auto keyboardListener1 = EventListenerKeyboard::create();
	keyboardListener1->onKeyPressed = CC_CALLBACK_2(FightMode::onKeyPressed1, this);
	keyboardListener1->onKeyReleased = CC_CALLBACK_2(FightMode::onKeyReleased1, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener1, player1);

	auto keyboardListener2 = EventListenerKeyboard::create();
	keyboardListener2->onKeyPressed = CC_CALLBACK_2(FightMode::onKeyPressed2, this);
	keyboardListener2->onKeyReleased = CC_CALLBACK_2(FightMode::onKeyReleased2, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener2, player2);
}
void FightMode::onKeyPressed1(EventKeyboard::KeyCode code, Event* event) {
	switch (code)
	{
	//weimumu
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		player1ADIsMove = true;
		player1ADMovekey = 'A';
		if (lastkey1 == 'D') {
			player1->setFlippedX(true);
		}
		lastkey1 = 'A';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
		player1ADIsMove = true;
		player1ADMovekey = 'D';
		if (lastkey1 == 'A') {
			player1->setFlippedX(false);
		}
		lastkey1 = 'D';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
		player1WSIsMove = true;
		player1WSMovekey = 'W';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
		player1WSIsMove = true;
		player1WSMovekey = 'S';
		break;



	//zzh
	case EventKeyboard::KeyCode::KEY_J:
		player1_numHit++;
		player1_attack_1 = true;
		break;
	case EventKeyboard::KeyCode::KEY_K:
		player1_numHit++;
		player1_attack_2 = true;
		break;
	case EventKeyboard::KeyCode::KEY_L:
		player1_defence = true;
		break;
	case EventKeyboard::KeyCode::KEY_I:
		player1_power = true;
		break;
	}
}
void FightMode::onKeyReleased1(EventKeyboard::KeyCode code, Event *event) {
	switch (code)
	{
	//weimumu
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
		player1ADIsMove = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:
		player1WSIsMove = false;
		break;


	//zzh
	case EventKeyboard::KeyCode::KEY_J:
		player1_attack_1 = false;
		break;
	case EventKeyboard::KeyCode::KEY_K:
		player1_attack_2 = false;
		break;
	case EventKeyboard::KeyCode::KEY_L:
		player1_defence = false;
		break;
	case EventKeyboard::KeyCode::KEY_I:
		player1_power = false;
		break;
	}
}
void FightMode::player1Movement(char ADkey, char WSkey) {
	float x_move = 0;
	float y_move = 0;
	if (player1ADIsMove) {
		switch (ADkey)
		{
		case 'A':
			if (player1->getPositionX() - x_num >= 30) {
				x_move = 0 - x_num;
			}
			else {
				x_move = 30 - player1->getPositionX();
			}
			break;
		case 'D':
			if (player1->getPositionX() + x_num <= visibleSize.width - 30) {
				x_move = x_num;
			}
			else {
				x_move = visibleSize.width - 30 - player1->getPositionX();
			}
			break;
		}
	}
	if (player1WSIsMove) {
		switch (WSkey) {
		case 'W':
			if (player1->getPositionY() + y_num <= 250) {
				y_move = y_num;
			}
			else {
				y_move = 250 - player1->getPositionY();
			}
			break;
		case 'S':
			if (player1->getPositionY() - y_num >= 80) {
				y_move = 0 - y_num;
			}
			else {
				y_move = 80 - player1->getPositionY();
			}
			break;
		}
	}
	auto move = MoveBy::create(0.1f, Vec2(x_move, y_move));
	auto animation1 = Animation::createWithSpriteFrames(player1Move, 0.10f);
	auto animate1 = Animate::create(animation1);
	auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.10f);
	auto animate2 = Animate::create(animation2);
	player1->runAction(Sequence::create(Spawn::create(animate1, move, NULL), animate2, NULL));
}


void FightMode::onKeyPressed2(EventKeyboard::KeyCode code, Event* event) {
	switch (code)
	{
	//weimumu
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		player2ADIsMove = true;
		player2ADMovekey = 'A';
		if (lastkey2 == 'D') {
			player2->setFlippedX(true);
		}
		lastkey2 = 'A';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player2ADIsMove = true;
		player2ADMovekey = 'D';
		if (lastkey2 == 'A') {
			player2->setFlippedX(false);
		}
		lastkey2 = 'D';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		player2WSIsMove = true;
		player2WSMovekey = 'W';
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player2WSIsMove = true;
		player2WSMovekey = 'S';
		break;



	//zzh
	case EventKeyboard::KeyCode::KEY_1:
		player2_numHit++;
		player2_attack_1 = true;
		break;
	case EventKeyboard::KeyCode::KEY_2:
		player2_numHit++;
		player2_attack_2 = true;
		break;
	case EventKeyboard::KeyCode::KEY_3:
		player2_defence = true;
		break;
	case EventKeyboard::KeyCode::KEY_5:
		player2_power = true;
		break;
	}
}
void FightMode::onKeyReleased2(EventKeyboard::KeyCode code, Event *event) {
	switch (code)
	{
	//weimumu
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		player2ADIsMove = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		player2WSIsMove = false;
		break;




	//zzh
	case EventKeyboard::KeyCode::KEY_1:
		player2_attack_1 = false;
		break;
	case EventKeyboard::KeyCode::KEY_2:
		player2_attack_2 = false;
		break;
	case EventKeyboard::KeyCode::KEY_3:
		player2_defence = false;
		break;
	case EventKeyboard::KeyCode::KEY_5:
		player2_power = false;
		break;
	}
}
void FightMode::player2Movement(char ADkey, char WSkey) {
	float x_move = 0;
	float y_move = 0;
	if (player2ADIsMove) {
		switch (ADkey)
		{
		case 'A':
			if (player2->getPositionX() - x_num >= 30) {
				x_move = 0 - x_num;
			}
			else {
				x_move = 30 - player2->getPositionX();
			}
			break;
		case 'D':
			if (player2->getPositionX() + x_num <= visibleSize.width - 30) {
				x_move = x_num;
			}
			else {
				x_move = visibleSize.width - 30 - player2->getPositionX();
			}
			break;
		}
	}
	if (player2WSIsMove) {
		switch (WSkey) {
		case 'W':
			if (player2->getPositionY() + y_num <= 250) {
				y_move = y_num;
			}
			else {
				y_move = 250 - player2->getPositionY();
			}
			break;
		case 'S':
			if (player2->getPositionY() - y_num >= 80) {
				y_move = 0 - y_num;
			}
			else {
				y_move = 80 - player2->getPositionY();
			}
			break;
		}
	}
	auto move = MoveBy::create(0.1f, Vec2(x_move, y_move));
	auto animation1 = Animation::createWithSpriteFrames(player2Move, 0.1f);
	auto animate1 = Animate::create(animation1);
	auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
	auto animate2 = Animate::create(animation2);
	player2->runAction(Sequence::create(Spawn::create(animate1, move, NULL), animate2, NULL));
}



int FightMode::attack(Sprite* player1, Sprite* player2, int player1_numHit, bool player1_attack_1, bool player1_attack_2, bool player2_defence, bool player1_power, ProgressTimer* Hp2, ProgressTimer* Mp1) {
	int flag = 0;
	if (player1_numHit > 0) {
		//人物1用拳+人物2未防御人物1用脚/人物1用大招
		if (player1_attack_1 && !player2_defence)
			flag = 3;
		else if (player1_attack_2)
			flag = 2;
		else if (player1_attack_1)
			flag = 1;
		//判断是否击中
		Rect player1Rect = player1->getBoundingBox();
		Rect player1attackRect = Rect(
			player1Rect.getMinX(),
			player1Rect.getMinY(),
			player1Rect.getMaxX() - player1Rect.getMinX(),
			player1Rect.getMaxY() - player1Rect.getMinY()
		);

		if (player1attackRect.containsPoint(player2->getPosition())) {
			auto hp = Hp2->getPercentage();
			if (flag == 3)
				hp -= 5;
			else if (flag == 2)
				hp -= 3;
			auto hpAction = ProgressTo::create(0.1, hp);
			Hp2->runAction(hpAction);
			flag += 2;
		}
	}
	return flag;
}

int FightMode::power_attack(Sprite* player1, Sprite* player2, Sprite* qigong1, Sprite* qigong2, bool player1_power, bool player2_defence, ProgressTimer* Hp2, ProgressTimer* Mp1) {
	int flag = 0;

	if (player1_power) {
		flag = 1;

	}
 
	return flag;
}

//0.1秒内只能攻击一次，发气功一次
void FightMode::update_numHit(float f) {
	int flag1 = attack(player1, player2, player1_numHit, player1_attack_1, player1_attack_2, player2_defence, player1_power, HPpt2, MPpt1);
	int flag2 = attack(player2, player1, player2_numHit, player2_attack_1, player2_attack_2, player1_defence, player2_power, HPpt1, MPpt2);
	//执行人物1的动画
	if (player1->getNumberOfRunningActions() == 1) {
		if (flag1 % 2 == 1) {
			//拳打
			auto animation1 = Animation::createWithSpriteFrames(player1AttackHand, 0.1f);
			auto animate1 = Animate::create(animation1);
			auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
			auto animate2 = Animate::create(animation2);
			player1->runAction(Sequence::create(animate1, animate2, NULL));
		}
		else if (flag1 % 2 == 0 && flag1) {
			//脚踢
			auto animation1 = Animation::createWithSpriteFrames(player1AttackLeg, 0.1f);
			auto animate1 = Animate::create(animation1);
			auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
			auto animate2 = Animate::create(animation2);
			player1->runAction(Sequence::create(animate1, animate2, NULL));
		}
		if (flag1 > 3) {
			player2_maxHit++;
			//对手弹飞
			auto animation1 = Animation::createWithSpriteFrames(player2BeingAttacked, 0.1f);
			auto animate1 = Animate::create(animation1);
			auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
			auto animate2 = Animate::create(animation2);
			auto action = Sequence::create(animate1, animate2, NULL);
			player2->runAction(action);
			action->setTag(02);
		}
	}
		

	//执行人物2的动画
	if (player2->getNumberOfRunningActions() == 1) {
		if (flag2 % 2 == 1) {
			//拳打
			auto animation1 = Animation::createWithSpriteFrames(player2AttackHand, 0.1f);
			auto animate1 = Animate::create(animation1);
			auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
			auto animate2 = Animate::create(animation2);
			player2->runAction(Sequence::create(animate1, animate2, NULL));
		}
		else if (flag2 % 2 == 0 && flag2) {
			//脚踢
			auto animation1 = Animation::createWithSpriteFrames(player2AttackLeg, 0.1f);
			auto animate1 = Animate::create(animation1);
			auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
			auto animate2 = Animate::create(animation2);
			player2->runAction(Sequence::create(animate1, animate2, NULL));
		}
		if (flag2 > 3) {
			player1_maxHit++;
			//对手弹飞
			auto animation1 = Animation::createWithSpriteFrames(player1BeingAttacked, 0.1f);
			auto animate1 = Animate::create(animation1);
			auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
			auto animate2 = Animate::create(animation2);
			auto action = Sequence::create(animate1, animate2, NULL);
			player1->runAction(action);
			action->setTag(01);
		}
	}

	player1_numHit = 0;
	player2_numHit = 0;

	if (player1_power) {
		player1AttackByQigong();
	}
	if (player2_power) {
		player2AttackByQigong();
	}
}


//监听气功与玩家的位置
void FightMode::update_powerHit(float f) {
	// 分发自定义事件
	EventCustom e("meet");
	_eventDispatcher->dispatchEvent(&e);
	//power_attack(player1, player2, qigong1, qigong2, player1_power, player2_defence, Hp2, Mp1);

	std::list<Sprite*>::iterator i = player1QiGongNumber.begin();
	std::list<Sprite*>::iterator j = player2QiGongNumber.begin();
	int count1 = player1QiGongNumber.size();
	int count2 = player2QiGongNumber.size();

	while (count1--)
	{
		if ((*i)->getPositionX() > visibleSize.width || (*i)->getPositionX() < 0)
			i = player1QiGongNumber.erase(i);
		else
			i++;
	}

	while (count2--)
	{
		if ((*j)->getPositionX() > visibleSize.width || (*j)->getPositionX() < 0)
			j = player2QiGongNumber.erase(j);
		else
			j++;
	}
}

//1.0秒内被击中三次就弹飞
void FightMode::update_maxHit(float f) {
	if (player2_maxHit > 3) {
		//执行死亡动画（人物弹飞）
		player2_dead();
		player2_maxHit = 0;
	}

	if (player1_maxHit > 3) {
		//执行死亡动画（人物弹飞）
		player1_dead();
		player1_maxHit = 0;
	}
}

void FightMode::player1_dead() {
	auto pos = player2->getPosition();
	int dis = 0;
	if (lastkey2 == 'A') {
		if (pos.x - 200 >= 0)
			dis = -200;
		//else
			//dis = -pos.x;
	}
	else {
		if (pos.x + 200 <= visibleSize.width)
			dis = 200;
		//else
			//dis = visibleSize.width - pos.x;
	}
	auto moveAnimation = MoveTo::create(0.5, Vec2(origin.x + pos.x + dis, origin.y + pos.y));
	auto animation1 = Animation::createWithSpriteFrames(player1Dead, 0.2f);
	auto animate1 = Animate::create(animation1);
	auto animation2 = Animation::createWithSpriteFrames(player1Idle, 1.0f);
	auto animate2 = Animate::create(animation2);
	auto seq = Sequence::create(Spawn::create(animate1, moveAnimation), nullptr);
	auto action = Sequence::create(seq, animate2, NULL);
	player1->runAction(action);
	action->setTag(03);
}

void FightMode::player2_dead() {
	auto pos = player2->getPosition();
	int dis = 0;
	if (lastkey1 == 'A') {
		if (pos.x - 200 >= 0)
			dis = -200;
		//else
			//dis = -pos.x;
	}
	else {
		if (pos.x + 200 <= visibleSize.width)
			dis = 200;
		//else
			//dis = visibleSize.width - pos.x;
	}
	auto moveAnimation = MoveTo::create(0.5, Vec2(origin.x + pos.x + dis, origin.y + pos.y));
	auto animation1 = Animation::createWithSpriteFrames(player2Dead, 0.2f);
	auto animate1 = Animate::create(animation1);
	auto animation2 = Animation::createWithSpriteFrames(player2Idle, 1.0f);
	auto animate2 = Animate::create(animation2);
	auto seq = Sequence::create(Spawn::create(animate1, moveAnimation), nullptr);
	auto action = Sequence::create(seq, animate2, NULL);
	player2->runAction(action);
	action->setTag(04);
}

// 添加自定义监听器
void FightMode::addCustomListener() {
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto meetListener = EventListenerCustom::create("meet", CC_CALLBACK_1(FightMode::meet, this));
	dispatcher->addEventListenerWithFixedPriority(meetListener, 1);
}

void FightMode::meet(EventCustom * event) {
	//判断player1是否击中player2
	int temp = 0;
	std::list<Sprite*>::iterator i = player1QiGongNumber.begin();
	for (; i != player1QiGongNumber .end();) {
		if ((*i)->getPositionY() - player2->getPositionY() <= 70 && (*i)->getPositionY() - player2->getPositionY() >= -70) {
			if ((*i)->getPositionX() - player2->getPositionX() <= 16 && (*i)->getPositionX() - player2->getPositionX() >= -16) {
				player2->runAction(Sequence::create(Animate::create(Animation::createWithSpriteFrames(player2BeingAttacked, 0.05f, 1)), nullptr));
				auto fadeout = FadeOut::create(0.01f);
				(*i)->runAction(fadeout);
				i = player1QiGongNumber.erase(i);
			}
			else {
				i++;
			}
		}
		else {
			i++;
		}
		temp++;
	}

	//判断player2是否击中player1
	int temp2 = 0;
	std::list<Sprite*>::iterator j = player2QiGongNumber.begin();
	for (; j != player2QiGongNumber.end();) {
		if ((*j)->getPositionY() - player1->getPositionY() <= 70 && (*j)->getPositionY() - player1->getPositionY() >= -70) {
			if ((*j)->getPositionX() - player1->getPositionX() <= 16 && (*j)->getPositionX() - player1->getPositionX() >= -16) {
				player1->runAction(Sequence::create(Animate::create(Animation::createWithSpriteFrames(player1BeingAttacked, 0.05f, 1)), nullptr));
				auto fadeout = FadeOut::create(0.01f);
				(*j)->runAction(fadeout);
				j = player2QiGongNumber.erase(j);
			}
			else {
				j++;
			}
		}
		else {
			j++;
		}
		temp2++;
	}
}

void FightMode::pause() {
	//auto labelReturnGame = Label::createWithSystemFont("Return Game", "Marker Felt", 80);
	//auto labelReStart = Label::createWithSystemFont("ReStart", "Marker Felt", 80);
	//auto labelReturnMenu = Label::createWithSystemFont("Return Menu", "Marker Felt", 80);
	//auto menuItem1 = MenuItemLabel::create(labelReturnGame, CC_CALLBACK_1(FightMode::returnGameCallback, this));
	//auto menuItem2 = MenuItemLabel::create(labelReStart, CC_CALLBACK_1(FightMode::reStartCallback, this));
	//auto menuItem3 = MenuItemLabel::create(labelReturnMenu, CC_CALLBACK_1(FightMode::returnMenuCallback, this));
	//menuItem1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 3 / 5);
	//menuItem2->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 / 5);
	//menuItem3->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 1 / 5);
	//auto menu = Menu::create(menuItem1,menuItem2, menuItem3, NULL);
	//menu->setPosition((Vec2::ZERO));
	//this->addChild(menu, 3);
	//CCDirector::getInstance()->pause();
	//CCActionManage
}

void FightMode::returnGameCallback(Ref* pSender) {

}
void FightMode::reStartCallback(Ref* pSender) {

}
void FightMode::returnMenuCallback(Ref* pSender) {

}
