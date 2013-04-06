//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.ippelife.com
//////////////////////////////////////////////////////////////////////////
#include "Bullet.h"
#include "res.h"
#include <iostream>
#include "Common.h"
#include "SimpleAudioEngine.h"

Bullet * Bullet::bullet(CCPoint start , Direction dir , BulletKind type)
{
	Bullet * b = Bullet::create();
	if(b && b->initB(start,dir,type))
		return b;
	return NULL;
}

bool  Bullet::initB(CCPoint start , Direction dir , BulletKind type)
{	
	bool bRet = false;
	do 
	{
		this->type = type;
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		this->start = start;
		this->dir = dir;
		this->be = MOVE;
		this->setTexture(CCTextureCache::sharedTextureCache()->addImage(fire_pariticl));
		this->setScale(0.6f);
		this->setLife(0.05f);
		this->setLifeVar(0.1f);
		this->setPosition(start);

		end = start;

		if(this->type == ENEMY)  //敌人的子弹小一点
			this->setScale(0.4f);
		switch(dir)
		{
		case UP:
			end.y = winSize.height + 20;
			speed = Common::getDistance(start,end)/winSize.height;
			break;
		case DOWN:
			end.y = -20;
			speed = Common::getDistance(start,end)/winSize.height;
			break;
		case LEFT:
			end.x = -20;
			speed = Common::getDistance(start,end)/winSize.width;
			break;
		case RIGHT:
			end.x = winSize.width + 20;
			speed = Common::getDistance(start,end)/winSize.width;
			break;
		}
		//printf("Bullet Direction : %d\n",dir);
		send();
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(fire_music,true);
		bRet = true;
		
	} while (0);
	return bRet;
}

void Bullet::send()
{
	this->runAction(CCMoveTo::create(speed,end));
}
void Bullet::explode()
{
	this->stopAllActions();
	CCFiniteTimeAction * disap = CCScaleTo::create(0.005f,5.0f,5.0f);
	CCCallFuncN *action_call = CCCallFuncN::create(this,callfuncN_selector(Bullet::explodeCallBack));
	this->runAction(CCSequence::create(disap,action_call,NULL));
}

void Bullet::explodeCallBack(CCNode * node)
{
	node->setVisible(false);
	this->be = DEAD;
}


Direction Bullet::getDirection()
{
	return this->dir;
}

Behaviour Bullet::getBehaviour()
{
	return this->be;
}

void Bullet::setBehaviour(Behaviour be)
{
	this->be = be;
}

BulletKind Bullet::getBulletType()
{
	return this->type;
}