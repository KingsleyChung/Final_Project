#ifndef __FIGHTMODE_H__
#define __FIGHTMODE_H__

#include "cocos2d.h"
USING_NS_CC;

class FightMode : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void initAnimation();
	CREATE_FUNC(FightMode);

	void player1AttackByHand(Ref* pSender);
	void FightMode::player1AttackByLeg(Ref* pSender);
	void FightMode::player1MoveAnimation();
	void FightMode::player1WouldDead(Ref* pSender);
	void FightMode::player1WouldDefense(Ref* pSender);
	void FightMode::player1AttackByQigong(Ref* pSender);

	void player2AttackByHand(Ref* pSender);
	void FightMode::player2AttackByLeg(Ref* pSender);
	void FightMode::player2MoveAnimation(Ref* pSender);
	void FightMode::player2WouldDead(Ref* pSender);
	void FightMode::player2WouldDefense(Ref* pSender);
	void FightMode::player2AttackByQigong(Ref* pSender);

private:
	//画面更新
	Size visibleSize;
	void update(float f);

	//palyer1动画
	Sprite * player1;
	Vector<SpriteFrame*> player1Idle;
	Vector<SpriteFrame*> player1AttackHand;
	Vector<SpriteFrame*> player1AttackLeg;
	Vector<SpriteFrame*> player1AttackQigong;
	Vector<SpriteFrame*> player1Move;
	Vector<SpriteFrame*> player1Dead;
	Vector<SpriteFrame*> player1Qigong;//气功形状变化动画
	Vector<SpriteFrame*> player1Defense;

	//palyer2动画
	Sprite * player2;
	Vector<SpriteFrame*> player2Idle;
	Vector<SpriteFrame*> player2AttackHand;
	Vector<SpriteFrame*> player2AttackLeg;
	Vector<SpriteFrame*> player2AttackQigong;
	Vector<SpriteFrame*> player2Move;
	Vector<SpriteFrame*> player2Dead;
	Vector<SpriteFrame*> player2Qigong;//气功形状变化动画
	Vector<SpriteFrame*> player2Defense;

	//人物移动相关变量
	char player1ADMovekey;
	char player1WSMovekey;
	char lastkey1;
	bool player1ADIsMove;
	bool player1WSIsMove;
	void player1Movement(char ADkey, char WSkey);
	void onKeyPressed1(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased1(EventKeyboard::KeyCode code, Event *event);

	char player2ADMovekey;
	char player2WSMovekey;
	char lastkey2;
	bool player2ADIsMove;
	bool player2WSIsMove;
	void player2Movement(char ADkey, char WSkey);
	void onKeyPressed2(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased2(EventKeyboard::KeyCode code, Event *event);

	void addKeyboardListener();
	Vec2 origin;
};

#endif // __FIGHTMODE_H__