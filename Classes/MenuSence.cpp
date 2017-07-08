#include "MenuSence.h"
#include "CooperateMode.h"
#include "FightMode.h"
USING_NS_CC;

Scene* MenuSence::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MenuSence::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuSence::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = CCSprite::create("MenuBackground.png");
    background->setPosition(origin.x, origin.y);
    background->setAnchorPoint(Vec2(0, 0));
    float bgx = background->getTextureRect().getMaxX();
    float bgy = background->getTextureRect().getMaxY();
    background->setScaleX(visibleSize.width / bgx);
    background->setScaleY(visibleSize.height / bgy);
    this->addChild(background, 0);

	MenuItemImage *pFightItem = MenuItemImage::create(
		"Fight.png",
		"Fight.png",
		CC_CALLBACK_1(MenuSence::enterFightModeCallback, this));
	Menu *FightMenu = Menu::create(pFightItem, NULL);
	FightMenu->setPosition(Vec2( visibleSize.width * 0.3 , visibleSize.height / 15));

	MenuItemImage *pExitItem = MenuItemImage::create(
		"Exit.png",
		"Exit.png",
		CC_CALLBACK_1(MenuSence::menuCloseCallback, this));
	Menu *ExitMenu = Menu::create(pExitItem, NULL);
	ExitMenu->setPosition(Vec2(visibleSize.width * 0.6, visibleSize.height / 15));

	this->addChild(FightMenu, 1);
	this->addChild(ExitMenu, 1);
    return true;
}

void MenuSence::enterFightModeCallback(Ref* pSender) {
	auto fightMode = FightMode::createScene();
	Director::getInstance()->replaceScene(TransitionSlideInT::create(1, fightMode));
}

void MenuSence::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}