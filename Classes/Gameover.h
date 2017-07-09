#ifndef __GAMEOVER__H__
#define __GAMEOVER__H__
#include "cocos2d.h"
USING_NS_CC;
class Gameover : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(RenderTexture* sqr);

    virtual bool init();
    CREATE_FUNC(Gameover);
	//重新开始游戏
	void menuRestart(Object* pSender);
	//回主界面
	void menuLogin(Object* pSender);
private:

};

#endif // __GAMEOVER__H__
