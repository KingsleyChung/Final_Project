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
	void FightMode::player1MoveAnimation(Ref* pSender);
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
	//�������
	Size visibleSize;
	void update(float f);

	//palyer1����
	Sprite * player1;
	Vector<SpriteFrame*> player1Idle;
	Vector<SpriteFrame*> player1AttackHand;
	Vector<SpriteFrame*> player1AttackLeg;
	Vector<SpriteFrame*> player1AttackQigong;
	Vector<SpriteFrame*> player1Move;
	Vector<SpriteFrame*> player1Dead;
	Vector<SpriteFrame*> player1Qigong;//������״�仯����
	Vector<SpriteFrame*> player1Defense;

	//palyer2����
	Sprite * player2;
	Vector<SpriteFrame*> player2Idle;
	Vector<SpriteFrame*> player2AttackHand;
	Vector<SpriteFrame*> player2AttackLeg;
	Vector<SpriteFrame*> player2AttackQigong;
	Vector<SpriteFrame*> player2Move;
	Vector<SpriteFrame*> player2Dead;
	Vector<SpriteFrame*> player2Qigong;//������״�仯����
	Vector<SpriteFrame*> player2Defense;

	//�����ƶ���ر���
	char player1Movekey;
	char lastkey1;
	void player1Movement(char key);
	char player2MoveKey;
	char lastkey2;
	void player2Movemwnt(char key);
	bool player1IsMove;
	bool player2IsMove;
	void onKeyPressed1(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased1(EventKeyboard::KeyCode code, Event *event);
	void addKeyboardListener();
	Size visibleSize;
	Vec2 origin;
};

#endif // __FIGHTMODE_H__