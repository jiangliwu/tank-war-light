//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.ippelife.com
//////////////////////////////////////////////////////////////////////////
#include "StartScene.h"
#include "PlayScene.h"
#include "res.h"
using namespace cocos2d;


CCScene* StartScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		StartScene *layer = StartScene::create();
		CC_BREAK_IF(! layer);
		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool StartScene::init()
{
	bool bRet = false;
	do 
	{

		//背景
		

		//数据


		this->setKeypadEnabled(true);
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSprite * mainBg = CCSprite::create(menu_main);
		mainBg->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(mainBg);

		//菜单项
		CCSprite *menuBg = CCSprite::create(menu_menubg);
		menuBg->setPosition(ccp(630,240));
		this->addChild(menuBg);

		CCMenuItemImage *start = CCMenuItemImage::create(
			menu_start,
			menu_start_,
			this,
			menu_selector(StartScene::menuStartCallBack)
			);
		start->setPosition(ccp(630,400));

		CCMenuItemImage *load = CCMenuItemImage::create(
			menu_load,
			menu_load_,
			this,
			menu_selector(StartScene::menuLoadCallBack)
			);
		load->setPosition(ccp(630,340));

		CCMenuItemImage *options = CCMenuItemImage::create(
			menu_options,
			menu_options_,
			this,
			menu_selector(StartScene::menuOptionsCallBack)
			);
		options->setPosition(ccp(630,280));

		CCMenuItemImage *about = CCMenuItemImage::create(
			menu_about,
			menu_about_,
			this,
			menu_selector(StartScene::menuAboutCallBack)
			);
		about->setPosition(ccp(630,220));

		CCMenuItemImage *exit = CCMenuItemImage::create(
			menu_exit,
			menu_exit_,
			this,
			menu_selector(StartScene::menuExitCallBack)
			);
		exit->setPosition(ccp(630,80));
		CCMenu * menu  = CCMenu::create(start,load,options,about,exit,NULL);
		menu->setPosition(CCPointZero);
		this->addChild(menu);

		//动画
		CCPoint menuOrigin = menu->getPosition();
		menu->setPosition(ccp(0,menu->getPosition().y+winSize.height));
		menu->runAction( 
			CCEaseElasticOut::create(
			CCMoveTo::create(2, menuOrigin), 0.35f
			) 
			);
		CCPoint menuBgOrigin = menuBg->getPosition();
		menuBg->setPosition(ccp(menuBg->getPosition().x,menuBg->getPosition().y + winSize.height));

		menuBg->runAction( 
			CCEaseElasticOut::create(
			CCMoveTo::create(2, menuBgOrigin), 0.35f
			) 
			);


		CCParticleRain * m_emitter = CCParticleRain::create();
		m_emitter->retain();
		this->addChild(m_emitter, 10);

		CCPoint p = m_emitter->getPosition();
		m_emitter->setPosition( CCPointMake( p.x, p.y) );
		m_emitter->setLife(4);

		m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(snow) );
		bRet = true;
	} while (0);
	return bRet;
}
StartScene::StartScene(void)
{
}

StartScene::~StartScene(void)
{
}

void StartScene::menuStartCallBack(cocos2d::CCObject *pSender)
{
	CCDirector::sharedDirector()->replaceScene(PlayScene::scene());
}
void StartScene::menuLoadCallBack(cocos2d::CCObject *pSender)
{

}
void StartScene::menuOptionsCallBack(cocos2d::CCObject *pSender)
{

}
void StartScene::menuAboutCallBack(cocos2d::CCObject *pSender)
{

}
void StartScene::menuExitCallBack(cocos2d::CCObject *pSender)
{
	CCDirector::sharedDirector()->end();
}

void StartScene::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
}