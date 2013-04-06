//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.ippelife.com
//////////////////////////////////////////////////////////////////////////
#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "cocos2d.h"
#include "TmxMap.h"
#include "SimpleAudioEngine.h"
#include "Mediator.h"
class PlayScene : public cocos2d::CCLayer
{
	TmxMap * map;   //µØÍ¼
	Joystick * control;  //Ò¡¸Ë
	CCMenuItemImage * fire;
	cocos2d::CCLayerColor * pauseLayer;
	bool isPause;
	CCLabelTTF  * overinfor;
	int isOver;
public:
	cocos2d::CCSize winSize;
	virtual bool init();  
	static cocos2d::CCScene* scene();
	CREATE_FUNC(PlayScene);
	virtual void keyBackClicked();
	virtual void keyMenuClicked();

	void test(float dt);

	void gameLoop(float dt);
	void gameFire(float dt);
	void gameCrash(float dt);
	void gameAddEnemy(float dt);
	void gameEnemyAction(float dt);


	void menuResume_CallBack(CCObject * pSender);
	void menuSaveCallBack(CCObject *pSender);
	void menuReturnCallBack(CCObject * pSender);
	void lvlCallBack(CCNode * node);
};

#endif  // __HELLOWORLD_SCENE_H__