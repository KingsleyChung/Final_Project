#ifndef __GAMEPAUSE__H__
#define __GAMEPAUSE__H__
#include "cocos2d.h"
USING_NS_CC;
class Gamepause : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(RenderTexture* sqr);

    virtual bool init();
    CREATE_FUNC(Gamepause);
	//继续游戏
	void menuContinueCallback(Object* pSender);
	//重新开始游戏
	void menuRestart(Object* pSender);
	//回主界面
	void menuLogin(Object* pSender);
private:

};

#endif // __GAMEPAUSE__H__
