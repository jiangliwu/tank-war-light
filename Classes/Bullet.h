//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.ippelife.com
//////////////////////////////////////////////////////////////////////////
#ifndef __BULLET__CORE__H__
#define __BULLET__CORE__H__
#include "type.h"
#include "cocos2d.h"
using namespace cocos2d;


class Bullet : public CCParticleSun
{
public:

	
	static Bullet * bullet(CCPoint start , Direction dir , BulletKind type);
	
	Behaviour getBehaviour();
	Direction getDirection();
	void setBehaviour (Behaviour be);
	
	BulletKind getBulletType();
	void explode();
	void send();
	CREATE_FUNC(Bullet);


protected:
private:
	bool  initB(CCPoint start , Direction dir , BulletKind type);
	
	void explodeCallBack(CCNode * node);
	Behaviour be;
	Direction dir;
	BulletKind type;
	CCPoint start;
	CCPoint end;
	float speed;
};
#endif