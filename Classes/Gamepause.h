#ifndef __Gamepause__H__
#define __Gamepause__H__
#include "cocos2d.h"
USING_NS_CC;
class Gamepause : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(RenderTexture* sqr);

    virtual bool init();  
    //static cocos2d::CCScene* scene(CCRenderTexture* sqr);
    CREATE_FUNC(Gamepause);
	//������Ϸ
	void menuContinueCallback(Object* pSender);
	//���¿�ʼ��Ϸ
	void menuRestart(Object* pSender);
	//��������
	void menuLogin(Object* pSender);
private:

};

#endif // __Gamepause_H__
