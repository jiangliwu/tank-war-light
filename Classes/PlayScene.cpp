//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.jiangliwu.me
//////////////////////////////////////////////////////////////////////////

#include "PlayScene.h"
#include "StartScene.h"
#include "Bullet.h"
#include "res.h"
using namespace cocos2d;
#include <iostream>
#include "Bullet.h"
#include "type.h"
#include "Tank.h"
//#include "core/Mediator.h"

CCScene* PlayScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);
		// 'layer' is an autorelease object
		PlayScene *layer = PlayScene::create();
		CC_BREAK_IF(! layer);
		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool PlayScene::init()
{
	bool bRet = false;
	do 
	{
		winSize = CCDirector::sharedDirector()->getWinSize();
		isPause = false;
		
		//this->registerWithTouchDispatcher();
		this->setKeypadEnabled(true);
		int lvl = CCUserDefault::sharedUserDefault()->getIntegerForKey("lvl");
		if(lvl == 0)
		{
			lvl = 1;
			CCUserDefault::sharedUserDefault()->setIntegerForKey("lvl",1);
		}
		char mapPath[20];
		char lvlinform[30];
		//map_tmx_lvl1
		sprintf(mapPath,"map/lvl%d.tmx",lvl);
		sprintf(lvlinform,"Lvl %d",lvl);
		isOver = -1;

		CCLabelTTF * lvlInform = CCLabelTTF::create(lvlinform, "Thonburi", 64);
		lvlInform->setPosition(ccp(-100,winSize.height/2));
		this->addChild(lvlInform,3);

		overinfor = CCLabelTTF::create("Game Over", "Thonburi", 64);
		overinfor->setPosition(ccp(winSize.width/2,winSize.height/2));
		overinfor->setVisible(false);
		this->addChild(overinfor,3);
		//��������ͣ�˵���Ϣs

		lvlInform->runAction(CCJumpTo::create(3,ccp(winSize.width+100,winSize.height/2),60,10));
		ccColor4B t;
		t.a = 200;
		t.r = 0;
		t.g = 0;
		t.b = 0;
		pauseLayer = CCLayerColor::create(t);
		this->addChild(pauseLayer,10);
		pauseLayer->setPosition(CCPointZero);
		CCSprite *pauseBg = CCSprite::create(pause_bg);
		pauseBg->setPosition(ccp(winSize.width/2 , winSize.height/2));
		pauseLayer->addChild(pauseBg);
		
		CCMenuItemImage *resume = CCMenuItemImage::create(
			pause_resume,
			pause_resume,
			this,
			menu_selector(PlayScene::menuResume_CallBack)
			);
		
		CCMenuItemImage *save = CCMenuItemImage::create(
			pause_save,
			pause_save,
			this,
			menu_selector(PlayScene::menuSaveCallBack)
			);


		CCMenuItemImage *returnMenu = CCMenuItemImage::create(
			pause_return,
			pause_return,
			this,
			menu_selector(PlayScene::menuReturnCallBack)
			);

		CCMenu * pauseMenu = CCMenu::create(resume,save,returnMenu,NULL);
		pauseMenu->alignItemsVerticallyWithPadding(30);
		pauseLayer->addChild(pauseMenu);
		pauseLayer->setVisible(false);



		//////////////////////////////////////////////////////////////////////////


		Mediator::getInstance()->release();   //�ͷ�
		Mediator::getInstance()->init();   //��ʼ��



		//���ص�ͼ
		map = TmxMap::initMap(mapPath);  //����
		this->addChild(map);
		
		
		//ҡ��
#ifdef IAMPHONE__
		CCSprite * bg = CCSprite::create(control_bg);
		CCSprite * cen = CCSprite::create(control_ce);
		control = Joystick::JoystickWithCenter(ccp(120,120),100.f,cen,bg);
		control->Active();
		this->addChild(control,1);
		control->setOpacity((float)0.3);

		
		//��ť
		fire = CCMenuItemImage::create(
			fire_normal,
			fire_press
			);
		fire->setOpacity(0.3*255);



		fire->setPosition(ccp(winSize.width - fire->getContentSize().width/2 - 10 , fire->getContentSize().height/2 + 10));
		CCMenu * menu = CCMenu::create(fire,NULL);
		menu->setPosition(CCPointZero);
		CCLayer * menu_layer = CCLayer::create();
		menu_layer->addChild(menu);


		//menu_layer->registerWithTouchDispatcher();
		//CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(menu_layer,0,false);
		//
		this->addChild(menu_layer);
#endif // IAMPHONE__
		this->setTouchEnabled(true);
		

		Tank * tanks = Tank::getATank(tank_player,UP,HERO);   //�����̹��
		this->addChild(tanks);
		tanks->getTankSprite()->setPosition(ccp(14.5*map->getTileSize().width ,16));

#ifdef IAMPHONE__
		Mediator::getInstance()->addJoyStick(control);  //���ҡ��
		Mediator::getInstance()->addFireButton(fire);    //��Ӱ�ť
#endif
		
		Mediator::getInstance()->addGameScene(this);    //��ӳ���
		Mediator::getInstance()->addHeroTank(tanks);    //���̹��
		Mediator::getInstance()->addTileMap(map);     //��ӵ�ͼ
		


		//schedule
		schedule(schedule_selector(PlayScene::gameFire),0.3f);    //�����������
		schedule(schedule_selector(PlayScene::gameLoop),0.01f);   //��Ϸ��ѭ��������
		schedule(schedule_selector(PlayScene::gameCrash),0.02f);   //��Ϸ��ײ������
		schedule(schedule_selector(PlayScene::gameAddEnemy),2.0f);   //ÿ��һ��ʱ����ӵ���
		schedule(schedule_selector(PlayScene::gameEnemyAction),0.05f);  //�з�̹�˵��˶�


		

		bRet = true;
	} while (0);
	return bRet;
}


void PlayScene::gameCrash(float dt)
{
	
}

void PlayScene::gameLoop(float dt)
{
	
	
	Mediator::getInstance()->bulletProcess(dt);   //�����ӵ���ײ��Ϊ
	
	Mediator::getInstance()->deleteBulletsAndTanks(dt);   //ɾ����Ҫ���ӵ��ͱ�ը��̹��
	
	if(Mediator::getInstance()->gameOver(dt))
	{
		if(Mediator::getInstance()->isVictory)
		{
			char str[100];
			if(CCUserDefault::sharedUserDefault()->getIntegerForKey("lvl") == 50)
			{
				sprintf(str,"all lvl passed , congratulations !");
				printf("all lvl passed , congratulations !\n");
				overinfor->setString(str);
				overinfor->setVisible(true);
				CCDirector::sharedDirector()->pause();
				pauseLayer->setVisible(true);
				isPause = true;
			}
			else
			{
			
			CCUserDefault::sharedUserDefault()->setIntegerForKey("lvl",CCUserDefault::sharedUserDefault()->getIntegerForKey("lvl") + 1);
			
			CCDirector::sharedDirector()->replaceScene(CCTransitionRotoZoom::create(2,PlayScene::scene()));

			}
		}
		else
		{
			char str[30];
			sprintf(str,"Lvl fail ,now restart !");
			printf("Lvl fail ,now restart !\n");
			overinfor->setString(str);
			overinfor->setVisible(true);
			CCDirector::sharedDirector()->replaceScene(CCTransitionRotoZoom::create(2,PlayScene::scene()));

		}
	}
	
}

void PlayScene::gameFire(float dt)
{
	Mediator::getInstance()->sendBullet(dt);
	
}
void PlayScene::gameAddEnemy(float dt)
{
	Mediator::getInstance()->addEnemy(dt);	
}

void PlayScene::gameEnemyAction(float dt)
{
	Mediator::getInstance()->controlMove(dt);
	Mediator::getInstance()->enemyMoves(dt);
}

void PlayScene::keyBackClicked()
{
	if(!isPause)   //���������ͣ��,��ô��ͣ
	{
		CCDirector::sharedDirector()->pause();
		pauseLayer->setVisible(true);
		isPause = true;
	}
	else  //����ͣ�ģ� �ͻָ�
	{
		pauseLayer->setVisible(false);
		CCDirector::sharedDirector()->resume();
		isPause = false;
	}
}


void PlayScene::keyMenuClicked()
{

}
void PlayScene::menuResume_CallBack(CCObject * pSender)
{
	if(isPause)  //������ͣ�Ļ�
	{
		pauseLayer->setVisible(false);
		CCDirector::sharedDirector()->resume();
		isPause = false;
	}
}
void PlayScene::menuSaveCallBack(CCObject *pSender)
{

}
void PlayScene::menuReturnCallBack(CCObject * pSender)
{
	CCDirector::sharedDirector()->resume();
	CCDirector::sharedDirector()->replaceScene(StartScene::scene());
}

void PlayScene::lvlCallBack(CCNode * node)
{
	CCDirector::sharedDirector()->replaceScene(StartScene::scene());
}
