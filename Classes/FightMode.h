#ifndef __FIGHTMODE_H__
#define __FIGHTMODE_H__

#include "cocos2d.h"
USING_NS_CC;

class FightMode : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	virtual void shootCallback(Ref* pSender);

	CREATE_FUNC(FightMode);

	void player1AttackByHand(Ref* pSender);
	void FightMode::player1AttackByLeg(Ref* pSender);
	void FightMode::player1MoveAnimation(Ref* pSender);
	void FightMode::player1WouldDead(Ref* pSender);
	void FightMode::player1WouldDefense(Ref* pSender);
	void FightMode::player1AttackByQigong(Ref* pSender);


private:
	Sprite * player1;
	Vector<SpriteFrame*> player1Idle;
	Vector<SpriteFrame*> player1AttackHand;
	Vector<SpriteFrame*> player1AttackLeg;
	Vector<SpriteFrame*> player1AttackQigong;
	Vector<SpriteFrame*> player1Move;
	Vector<SpriteFrame*> player1Dead;
	Vector<SpriteFrame*> player1Qigong;//������״�仯����
	Vector<SpriteFrame*> player1Defense;

	Sprite * player2;
	Vector<SpriteFrame*> player2Idle;
	Vector<SpriteFrame*> player2AttackHand;
	Vector<SpriteFrame*> player2AttackLeg;
	Vector<SpriteFrame*> player2AttackQigong;
	Vector<SpriteFrame*> player2Move;
	Vector<SpriteFrame*> player2Dead;
	Vector<SpriteFrame*> player2Qigong;//������״�仯����
	Vector<SpriteFrame*> player2Defense;

};

#endif // __FIGHTMODE_H__