#include "CooperateMode.h"

USING_NS_CC;

Scene* CooperateMode::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = CooperateMode::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CooperateMode::init()
{

    if (!Layer::init())
    {
        return false;
    }

    //add touch listener
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CooperateMode::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    return true;
}

bool CooperateMode::onTouchBegan(Touch *touch, Event *unused_event) {

    return true;
}

void CooperateMode::shootCallback(Ref* pSender)
{
    
}