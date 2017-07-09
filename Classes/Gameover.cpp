#include "Gameover.h"
#include "FightMode.h"
#include "MenuSence.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
//����һ��CCrenderTexture 
//�൱��һ���������е���Ϸ�Ľ�ͼ��Ϊ�����ͣ�Ի���ı��� 
//�����Ϳ��������ǶԻ�������Ϸ����֮�ϣ�һ����Ϸ���ж���������д�ġ�
Scene* Gameover::createScene(RenderTexture* sqr)
{
    auto scene = Scene::create();
    auto layer = Gameover::create();
	scene->addChild(layer,2);//����Ϸ������棬���ǻ�Ҫ��������Ű�ť


	//���Ӳ��֣�ʹ��Game�����н�ͼ��sqr����ͼƬ����Sprite
	//����Sprite��ӵ�GamePause��������
	//�õ����ڵĴ�С
	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	Sprite *back_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());  
	back_spr->setPosition(ccp(visibleSize.width/2,visibleSize.height/2)); //����λ��,������������λ�á�
	back_spr->setFlipY(true);            //��ת����ΪUI�����OpenGL���겻ͬ
	//back_spr->setColor(Color3B::GRAY); //ͼƬ��ɫ���ɫ
	scene->addChild(back_spr);


	//�����Ϸ��ͣ����Сͼ�������Ű�ť
	Sprite *back_small_spr = Sprite::create("GameOver.png");
	back_small_spr->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2)); //����λ��,������������λ�á�
	scene->addChild(back_small_spr);

    return scene;
}

bool Gameover::init()
{

    if ( !CCLayer::init() )
    {
        return false;
    }
	//�õ����ڵĴ�С
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//ԭ������
	Point origin = Director::getInstance()->getVisibleOrigin();

	//���¿�ʼ��Ϸ��ť
	MenuItemImage *pRestartItem = MenuItemImage::create(
		"Restart.png",
		"Restart.png",
		CC_CALLBACK_1(Gameover::menuRestart, this));

	pRestartItem->setPosition(Vec2(0 ,visibleSize.height/15 - 40));

	//��������
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

//���¿�ʼ��Ϸ
void  Gameover::menuRestart(Object* pSender)
{
	Director::sharedDirector()->replaceScene(FightMode::createScene());
}

//��������
void  Gameover::menuLogin(Object* pSender)
{
	Director::sharedDirector()->replaceScene(MenuSence::createScene());
}