//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.ippelife.com
//////////////////////////////////////////////////////////////////////////
#ifndef __MEDIATOR_CORE_H__
#define __MEDIATOR_CORE_H__
#define COLLIDE_FIX 12
#include "Singleton.h"
#include "Joystick.h"
#include "TmxMap.h"
#include "Tank.h"
#include "Bullet.h"
#include "Common.h"
#include "cocos2d.h"
#include <map>
using namespace cocos2d;
using std::map;

//
class Mediator:public Singleton<Mediator>   //����ģʽ
{
public:

	~Mediator();
	void addHeroTank(Tank *hero);
	void addTileMap(TmxMap *tileMap);
	void addGameScene(CCLayer *gameScene);
	void addJoyStick(Joystick * jst);
	void addFireButton(CCMenuItemImage * fire);
	

	bool init();
	void sendBullet(float dt);     //�����ӵ�������Ҳ����ҡ�˼��
	void controlMove(float dt);     //�����ƶ�����ǽ�ڵ���ײ
	void bulletProcess(float dt);    //�ӵ���ǽ�ڵ���ײ,�ӵ���̹�˵���ײ
	void addEnemy(float dt);     //������ӵ���
	void deleteBulletsAndTanks(float dt);  //ɾ�����õ��ӵ�
	void enemyMoves(float dt);    //���˵��ƶ�����
	bool gameOver(float dt);
	bool isCanMove   (Tank * tank,Direction dir);
	bool isVictory;

private:
	TmxMap *tileMap;
	Tank *heroTank;
	CCLayer *gameScene;
	Joystick * jst;
	CCMenuItemImage * fire;
	CCSize winSize;
	CCArray *bullets;
	CCArray *tanks;
	CCArray *enemysBullets;
	int numberOfEnemys;     //Ӧ���еĵ��˸��� 
  	int addedEnemys;     //�Ѿ���ӵ��˵ĸ���
	int leaveEnemys;	//ս���˻�ʣ�µ�����
	bool isGameOver;    //��Ϸ�Ƿ����
};


#endif