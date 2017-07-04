#ifndef __COOPERATEMODE_H__
#define __COOPERATEMODE_H__

#include "cocos2d.h"
USING_NS_CC;

class CooperateMode : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    virtual bool onTouchBegan(Touch *touch, Event *unused_event);

    virtual void shootCallback(Ref* pSender);

    CREATE_FUNC(CooperateMode);

private:
    Sprite* player1;

    Sprite* player2;

    Layer* enermyLayer;

};

#endif // __GAME_SENCE_H__