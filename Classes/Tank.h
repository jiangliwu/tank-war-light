//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.jiangliwu.me
//////////////////////////////////////////////////////////////////////////
#ifndef __CORE_TANK_H__
#define __CORE_TANK_H__
#include "type.h"
#include "cocos2d.h"
#include <string>
using namespace cocos2d;
using std::string;

class Tank : public CCNode
{
public:
	
	static Tank * getATank(string path ,Direction dir,TankKind kind);

	void up();
	void down();
	void left();
	void right();
	void move(int dir);
	Direction getDirection();   //����
	Behaviour getBehaviour(); 
	CCSprite * getTankSprite();
	void setSpeed(float speed);
	float getSpeed();    //ȡ���ٶ�
	void changDirectionGUI(Direction dir);   //���������ʱ��ı�̹�˵����
	TankKind getKind();
	void setBehaviour(Behaviour staus);

protected:
	
private:
	CCSprite * sprites[4];
	void setEffectPosition(float dt);
	bool  initT(string path ,Direction dir,TankKind kind);
	Direction direction;   //����
	Behaviour behaviour;    //�ж�
	float life;
	CCSprite * tankSprite;
	CCParticleGalaxy *effect; 
	TankKind kind;
	float speed;
};
#endif