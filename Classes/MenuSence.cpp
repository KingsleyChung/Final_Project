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

    auto background = CCSprite::create("MenuBackground.jpg");
    background->setPosition(origin.x, origin.y);
    background->setAnchorPoint(Vec2(0, 0));
    float bgx = background->getTextureRect().getMaxX();
    float bgy = background->getTextureRect().getMaxY();
    background->setScaleX(visibleSize.width / bgx);
    background->setScaleY(visibleSize.height / bgy);
    this->addChild(background, 0);

    auto cooperateLabel = Label::create("Cooperate", "fonts/arial.ttf", 76);
    auto cooperateMenuItemLabel = MenuItemLabel::create(cooperateLabel, CC_CALLBACK_1(MenuSence::enterCooperateModeCallback, this));
    auto cooperateMenu = Menu::create(cooperateMenuItemLabel, NULL);
    cooperateMenu->setPosition(visibleSize.width/2, visibleSize.height/2);
    this->addChild(cooperateMenu, 1);

    return true;
}

void MenuSence::enterCooperateModeCallback(Ref* pSender) {
    /*auto cooperateMode = CooperateMode::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInT::create(1, cooperateMode));*/
	auto fightMode = FightMode::createScene();
	Director::getInstance()->replaceScene(TransitionSlideInT::create(1, fightMode));
}
