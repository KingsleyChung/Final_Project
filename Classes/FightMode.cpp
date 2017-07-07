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
	x_num = 35;
	y_num = 30;
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
	addKeyboardListener();
	schedule(schedule_selector(FightMode::update), 0.04f, kRepeatForever, 0);

	return true;
}
//初始化player1和player2的所有动画
void FightMode::initAnimation() {
	//创建一张玩家1的贴图
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

	//创建一张玩家2的贴图
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


	//hp条
	Sprite* sp00 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));
	Sprite* sp11 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp1 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));
	Sprite* sp22 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp2 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));
	Sprite* sp33 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp3 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	Hp1 = ProgressTimer::create(sp0);
	Hp1->setScaleX(90);
	Hp1->setAnchorPoint(Vec2(0, 0));
	Hp1->setType(ProgressTimerType::BAR);
	Hp1->setBarChangeRate(Point(1, 0));
	Hp1->setMidpoint(Point(0, 1));
	Hp1->setPercentage(100);
	Hp1->setPosition(Vec2(origin.x + 14 * Hp1->getContentSize().width,
		origin.y + visibleSize.height - 2 * Hp1->getContentSize().height));
	addChild(Hp1, 1);
	sp00->setAnchorPoint(Vec2(0, 0));
	sp00->setPosition(Vec2(origin.x + Hp1->getContentSize().width,
		origin.y + visibleSize.height - sp00->getContentSize().height));
	addChild(sp00, 0);

	Hp2 = ProgressTimer::create(sp1);
	Hp2->setScaleX(90);
	Hp2->setAnchorPoint(Vec2(0, 0));
	Hp2->setType(ProgressTimerType::BAR);
	Hp2->setBarChangeRate(Point(1, 0));
	Hp2->setMidpoint(Point(0, 1));
	Hp2->setPercentage(100);
	Hp2->setPosition(Vec2(origin.x + 14 * Hp2->getContentSize().width + 1000,
		origin.y + visibleSize.height - 2 * Hp2->getContentSize().height));
	addChild(Hp2, 1);
	sp11->setAnchorPoint(Vec2(0, 0));
	sp11->setPosition(Vec2(origin.x + Hp2->getContentSize().width + 1000,
		origin.y + visibleSize.height - sp11->getContentSize().height));
	addChild(sp11, 0);

	Mp1 = ProgressTimer::create(sp2);
	Mp1->setScaleX(90);
	Mp1->setAnchorPoint(Vec2(0, 0));
	Mp1->setType(ProgressTimerType::BAR);
	Mp1->setBarChangeRate(Point(1, 0));
	Mp1->setMidpoint(Point(0, 1));
	Mp1->setPercentage(100);
	Mp1->setPosition(Vec2(origin.x + 14 * Mp1->getContentSize().width,
		origin.y + visibleSize.height - 2 * Mp1->getContentSize().height - 100));
	addChild(Mp1, 1);
	sp22->setAnchorPoint(Vec2(0, 0));
	sp22->setPosition(Vec2(origin.x + Mp1->getContentSize().width,
		origin.y + visibleSize.height - sp22->getContentSize().height - 100));
	addChild(sp22, 0);

	Mp2 = ProgressTimer::create(sp3);
	Mp2->setScaleX(90);
	Mp2->setAnchorPoint(Vec2(0, 0));
	Mp2->setType(ProgressTimerType::BAR);
	Mp2->setBarChangeRate(Point(1, 0));
	Mp2->setMidpoint(Point(0, 1));
	Mp2->setPercentage(100);
	Mp2->setPosition(Vec2(origin.x + 14 * Mp2->getContentSize().width + 1000,
		origin.y + visibleSize.height - 2 * Mp2->getContentSize().height - 100));
	addChild(Mp2, 1);
	sp33->setAnchorPoint(Vec2(0, 0));
	sp33->setPosition(Vec2(origin.x + Mp2->getContentSize().width + 1000,
		origin.y + visibleSize.height - sp33->getContentSize().height - 100));
	addChild(sp33, 0);

	//
	addKeyboardListener();
	schedule(schedule_selector(FightMode::update), 0.04f, kRepeatForever, 0);
	schedule(schedule_selector(FightMode::update_numHit), 0.1f, kRepeatForever, 0);
	schedule(schedule_selector(FightMode::update_maxHit), 1.0f, kRepeatForever, 0);


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
	auto textureleg = Director::getInstance()->getTextureCache()->addImage("player2/woody_1.png");
	player2AttackLeg.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(textureleg, CC_RECT_PIXELS_TO_POINTS(Rect(560 + 80 * i, 243, 80, 81)));
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

void FightMode::player1MoveAnimation() {
	if (player1->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player1Move, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
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

//player1运行气功动画并产生气功精灵移动
void FightMode::player1AttackByQigong(Ref* pSender) {
	auto callBack = CallFunc::create(CC_CALLBACK_0(FightMode::player1QiGong, this));
	if (player1->getNumberOfRunningActions() == 0) {
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
	auto animation3 = Animation::createWithSpriteFrames(player1Qigong, 0.15f);
	auto animate3 = Animate::create(animation3);
	auto moveBy = MoveBy::create(0.8, Vec2(600, 0));
	if (lastkey1 == 'A')
		moveBy = MoveBy::create(0.8, Vec2(-600, 0));
	else
		moveBy = MoveBy::create(0.8, Vec2(600, 0));
	auto spawn = Spawn::createWithTwoActions(animate3, moveBy);
	auto fadeout = FadeOut::create(0.01f);
	qigong1->runAction(Sequence::create(spawn, fadeout, NULL));
}

void FightMode::player2AttackByHand(Ref* pSender) {
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2AttackHand, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player2AttackByLeg(Ref* pSender) {
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2AttackLeg, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player2MoveAnimation(Ref* pSender) {
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2Move, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player2WouldDead(Ref* pSender) {
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2Dead, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

void FightMode::player2WouldDefense(Ref* pSender) {
	if (player2->getNumberOfRunningActions() == 0) {
		auto animation1 = Animation::createWithSpriteFrames(player2Defense, 0.5f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.5f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
	}
}

//player2运行气功动画并产生气功精灵移动
void FightMode::player2AttackByQigong(Ref* pSender) {
	auto callBack = CallFunc::create(CC_CALLBACK_0(FightMode::player2QiGong, this));
	if (player2->getNumberOfRunningActions() == 0) {
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
	auto moveBy = MoveBy::create(1.2, Vec2(600, 0));
	if (lastkey2 == 'A')
		moveBy = MoveBy::create(1.2, Vec2(-600, 0));
	else
		moveBy = MoveBy::create(1.2, Vec2(600, 0));
	auto spawn = Spawn::createWithTwoActions(animate3, moveBy);
	auto fadeout = FadeOut::create(0.01f);
	qigong2->runAction(Sequence::create(spawn, fadeout, NULL));
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
	/*if (player2_defence) {
		if (player2IsDefend == false && player2->getNumberOfRunningActions() == 0) {
			auto animation1 = Animation::createWithSpriteFrames(player2Defense, 0.1f);
			auto animate1 = Animate::create(animation1);
			player2->runAction(Sequence::create(animate1, NULL));
			player2IsDefend = true;
		}
	}
	else {
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate2, NULL));
		player2IsDefend = false;
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
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate2, NULL));
		player1IsDefend = false;
	}*/

	//Mp补充
	auto mp1 = Mp1->getPercentage();
	mp1 += 10;
	auto hpAction = ProgressTo::create(0.1, mp1);
	Mp1->runAction(hpAction);

	auto mp2 = Mp2->getPercentage();
	mp2 += 10;
	auto hpAction = ProgressTo::create(0.1, mp2);
	Mp2->runAction(hpAction);
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
			if (player1->getPositionY() + y_num <= 280) {
				y_move = y_num;
			}
			else {
				y_move = 280 - player1->getPositionY();
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
			if (player2->getPositionY() + y_num <= 280) {
				y_move = y_num;
			}
			else {
				y_move = 280 - player2->getPositionY();
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
		//人物1用拳+人物2未防御/人物1用脚/人物1用大招

		//判断是否击中
		Rect player1Rect = player1->getBoundingBox();
		Rect player1attackRect = Rect(
			player1Rect.getMinX(),
			player1Rect.getMinY(),
			player1Rect.getMaxX() - player1Rect.getMinX() + 10,
			player1Rect.getMaxY() - player1Rect.getMinY()
		);
		if (player1attackRect.containsPoint(player2->getPosition())) {
			if (player1_attack_1 && !player2_defence)
				flag = 1;
			else if (player1_attack_2)
				flag = 2;
			else if (player1_power)
				flag = 3;

			auto hp = Hp2->getPercentage();
			auto mp = Mp1->getPercentage();
			if (flag == 1)
				hp -= 3;
			else if (flag == 2)
				hp -= 1;
			else if (flag == 3) {
				hp -= 10;
				mp -= 50;
			}
			auto hpAction = ProgressTo::create(0.1, hp);
			Hp2->runAction(hpAction);
			auto mpAction = ProgressTo::create(0.1, mp);
			Mp1->runAction(mpAction);
		}
	}
	return flag;
}

//0.1秒内只能攻击一次
void FightMode::update_numHit(float f) {
	int flag1 = attack(player1, player2, player1_numHit, player1_attack_1, player1_attack_2, player2_defence, player1_power, Hp2, Mp1);
	int flag2 = attack(player2, player1, player2_numHit, player2_attack_1, player2_attack_2, player1_defence, player2_power, Hp1, Mp2);
	//执行人物1的动画
	if (flag1 == 1) {
		//拳打
		auto animation1 = Animation::createWithSpriteFrames(player1AttackHand, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
		player2_maxHit++;
	}
	else if (flag1 == 2) {
		//脚踢
		auto animation1 = Animation::createWithSpriteFrames(player1AttackLeg, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player1Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		player1->runAction(Sequence::create(animate1, animate2, NULL));
		player2_maxHit++;
	}
	else if (flag1 == 3) {
		//大招


		//执行死亡动画（人物弹飞）
		player2_dead();
	}

	//执行人物2的动画
	if (flag2 == 1) {
		//拳打
		auto animation1 = Animation::createWithSpriteFrames(player2AttackHand, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
		player1_maxHit++;
	}
	else if (flag2 == 2) {
		//脚踢
		auto animation1 = Animation::createWithSpriteFrames(player2AttackLeg, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(player2Idle, 0.1f);
		auto animate2 = Animate::create(animation2);
		player2->runAction(Sequence::create(animate1, animate2, NULL));
		player1_maxHit++;
	}
	else if (flag2 == 3) {
		//大招


		//执行死亡动画（人物弹飞）
		player1_dead();
	}

	player1_numHit = 0;
	player2_numHit = 0;
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
	auto moveAnimation = MoveTo::create(0.5, Vec2(origin.x + pos.x + 200, origin.y + pos.y));
	auto animation1 = Animation::createWithSpriteFrames(player1Dead, 0.2f);
	auto animate1 = Animate::create(animation1);
	auto animation2 = Animation::createWithSpriteFrames(player1Idle, 1.0f);
	auto animate2 = Animate::create(animation2);
	auto seq = Sequence::create(Spawn::create(animate1, moveAnimation), nullptr);
	player1->runAction(Sequence::create(seq, animate2, NULL));
}

void FightMode::player2_dead() {
	auto pos = player2->getPosition();
	auto moveAnimation = MoveTo::create(0.5, Vec2(origin.x + pos.x + 200, origin.y + pos.y));
	auto animation1 = Animation::createWithSpriteFrames(player2Dead, 0.2f);
	auto animate1 = Animate::create(animation1);
	auto animation2 = Animation::createWithSpriteFrames(player2Idle, 1.0f);
	auto animate2 = Animate::create(animation2);
	auto seq = Sequence::create(Spawn::create(animate1, moveAnimation), nullptr);
	player2->runAction(Sequence::create(seq, animate2, NULL));
}