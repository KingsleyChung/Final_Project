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

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(FightMode::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("FightBackground.JPG");
	bg->setPosition(origin.x, origin.y);
	bg->setAnchorPoint(Vec2(0, 0));
	float bgx = bg->getTextureRect().getMaxX();
	float bgy = bg->getTextureRect().getMaxY();
	bg->setScaleX(visibleSize.height / bgy);
	bg->setScaleY(visibleSize.height / bgy);
	this->addChild(bg, 0);

	/*auto label = Label::create("Shoot", "fonts/arial.ttf", 76);
	auto menuItemLabel = MenuItemLabel::create(label, CC_CALLBACK_1(CooperateMode::shootCallback, this));
	menuItemLabel->setPosition(Vec2(visibleSize.width + origin.x - 180, origin.y + 475));
	auto menu = Menu::create(menuItemLabel, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	enermyLayer = Layer::create();
	enermyLayer->ignoreAnchorPointForPosition(true);
	enermyLayer->setPosition(Vec2(origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(enermyLayer);*/

	return true;
}

bool FightMode::onTouchBegan(Touch *touch, Event *unused_event) {

	return true;
}

void FightMode::shootCallback(Ref* pSender)
{

}