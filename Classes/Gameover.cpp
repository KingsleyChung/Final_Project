#include "Gameover.h"
#include "FightMode.h"
#include "MenuSence.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
//传入一个CCrenderTexture 
//相当于一个正在运行的游戏的截图作为这个暂停对话框的背景 
//这样就看起来像是对话框在游戏界面之上，一般游戏当中都是这样子写的。
Scene* Gameover::createScene(RenderTexture* sqr)
{
    auto scene = Scene::create();
    auto layer = Gameover::create();
	scene->addChild(layer,2);//把游戏层放上面，我们还要在这上面放按钮


	//增加部分：使用Game界面中截图的sqr纹理图片创建Sprite
	//并将Sprite添加到GamePause场景层中
	//得到窗口的大小
	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	Sprite *back_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());  
	back_spr->setPosition(ccp(visibleSize.width/2,visibleSize.height/2)); //放置位置,这个相对于中心位置。
	back_spr->setFlipY(true);            //翻转，因为UI坐标和OpenGL坐标不同
	//back_spr->setColor(Color3B::GRAY); //图片颜色变灰色
	scene->addChild(back_spr);


	//添加游戏暂停背景小图，用来放按钮
	Sprite *back_small_spr = Sprite::create("GameOver.png");
	back_small_spr->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2)); //放置位置,这个相对于中心位置。
	scene->addChild(back_small_spr);

    return scene;
}

bool Gameover::init()
{

    if ( !CCLayer::init() )
    {
        return false;
    }
	//得到窗口的大小
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//原点坐标
	Point origin = Director::getInstance()->getVisibleOrigin();

	//重新开始游戏按钮
	MenuItemImage *pRestartItem = MenuItemImage::create(
		"Restart.png",
		"Restart.png",
		CC_CALLBACK_1(Gameover::menuRestart, this));

	pRestartItem->setPosition(Vec2(0 ,visibleSize.height/15 - 40));

	//回主界面
	MenuItemImage *pLoginItem = MenuItemImage::create(
		"Exit.png",
		"Exit.png",
		CC_CALLBACK_1(Gameover::menuLogin, this));

	pLoginItem->setPosition(Vec2(0 ,visibleSize.height/15 - 140));


	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create(pRestartItem,pLoginItem,NULL);
	pMenu->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	this->addChild(pMenu, 2);

    return true;
}

//重新开始游戏
void  Gameover::menuRestart(Object* pSender)
{
	Director::sharedDirector()->replaceScene(FightMode::createScene());
}

//回主界面
void  Gameover::menuLogin(Object* pSender)
{
	Director::sharedDirector()->replaceScene(MenuSence::createScene());
}