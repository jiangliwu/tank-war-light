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
class Mediator:public Singleton<Mediator>   //单例模式
{
public:

	~Mediator();
	void addHeroTank(Tank *hero);
	void addTileMap(TmxMap *tileMap);
	void addGameScene(CCLayer *gameScene);
	void addJoyStick(Joystick * jst);
	void addFireButton(CCMenuItemImage * fire);
	

	bool init();
	void sendBullet(float dt);     //发射子弹监听，也就是摇杆监控
	void controlMove(float dt);     //控制移动，与墙壁的碰撞
	void bulletProcess(float dt);    //子弹与墙壁的碰撞,子弹与坦克的碰撞
	void addEnemy(float dt);     //更新添加敌人
	void deleteBulletsAndTanks(float dt);  //删除无用的子弹
	void enemyMoves(float dt);    //敌人的移动代码
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
	int numberOfEnemys;     //应该有的敌人个数 
  	int addedEnemys;     //已经添加敌人的个数
	int leaveEnemys;	//战场人还剩下的人数
	bool isGameOver;    //游戏是否结束
};


#endif