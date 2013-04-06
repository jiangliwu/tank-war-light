#pragma once
#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

class StartScene : public cocos2d::CCLayer
{
public:
	virtual bool init();  

	static cocos2d::CCScene* scene();
	CREATE_FUNC(StartScene);
	StartScene(void);
	~StartScene(void);

	void menuStartCallBack(cocos2d::CCObject *pSender);
	void menuLoadCallBack(cocos2d::CCObject *pSender);
	void menuOptionsCallBack(cocos2d::CCObject *pSender);
	void menuAboutCallBack(cocos2d::CCObject *pSender);
	void menuExitCallBack(cocos2d::CCObject *pSender);
	virtual void keyBackClicked();

};


#endif  // __HELLOWORLD_SCENE_H__