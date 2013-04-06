#include "Tank.h"
#include "res.h"
#include <string>
using std::string;

Tank * Tank::getATank(string path , Direction dir , TankKind kind)
{
	Tank * t = new Tank();
	if(t && t->initT(path,dir,kind))
		return t;
	return NULL;
}

bool Tank::initT(string path , Direction dir , TankKind kind)
{
	bool bRet = false;
	do 
	{
		this->kind = kind;
		//tankSprite = CCSprite::spriteWithFile(path.c_str(),size);
		this->behaviour = MOVE;
		this->speed = 4.0f;
		
		for (int i = 0 ; i <4 ; i++)   //初始化四个方向的坦克
		{
			sprites[i] = CCSprite::create(path.c_str(),CCRectMake(i*32,0,32,32));
			this->addChild(sprites[i]);
			sprites[i]->setPosition(ccp(-sprites[i]->getContentSize().width, - sprites[i]->getContentSize().height));
			sprites[i]->setVisible(false);
		}
		this->direction = dir;
		sprites[this->direction]->setVisible(true);
		tankSprite = sprites[this->direction];
		tankSprite->setPosition(CCPointZero);
		//this->addChild(tankSprite,0);
		
		if(this->kind == HERO)  //如果为英雄坦克， 添加辉光效果
		{
			effect = CCParticleGalaxy::create();
			effect->setTexture(CCTextureCache::sharedTextureCache()->addImage(fire_pariticl));
			effect->setScale(0.4f);
			effect->setLife(0.5f);
			effect->setPosition(this->tankSprite->getPosition());
			this->addChild(effect,1);

			//printf("Hero\n");
		}
		
		schedule(schedule_selector(Tank::setEffectPosition)); //更新辉光的位置
		//this->runAction(CCMoveTo::actionWithDuration(0.5,ccp(this->getPosition().x +200,this->getPosition().y)));
		bRet = true;
	} while (0);

	return bRet;
	
}

void Tank::setEffectPosition(float dt)
{
	if(this->kind ==  HERO)
	{
		effect->setPosition(this->tankSprite->getPosition());
		//printf("%f %f\n",this->tankSprite->getPosition().x , this->tankSprite->getPosition().y);
		//printf("set\n");
	}

	if(this->behaviour == DEAD)  //挂了 爆炸
	{
		this->tankSprite->setVisible(false);
	}

}

void Tank::up()
{
	this->tankSprite->runAction(CCMoveTo::create(0.05f,ccp(this->tankSprite->getPosition().x , this->tankSprite->getPosition().y + speed)));
}

void Tank::down()
{
	
	this->tankSprite->runAction(CCMoveTo::create(0.05f,ccp(this->tankSprite->getPosition().x , this->tankSprite->getPosition().y - speed)));
}
void Tank::left()
{
	this->tankSprite->runAction(CCMoveTo::create(0.05f,ccp(this->tankSprite->getPosition().x - speed, this->tankSprite->getPosition().y )));
}
void Tank::right()
{
	
	this->tankSprite->runAction(CCMoveTo::create(0.05f,ccp(this->tankSprite->getPosition().x + speed, this->tankSprite->getPosition().y )));
}

void Tank::move(int dir)
{
	switch (dir)
	{
	case UP:
		up();
		break;
	case DOWN:
		down();
		break;
	case LEFT:
		left();
		break;
	case RIGHT:
		right();
		break;
	}
}



void Tank::changDirectionGUI(Direction dir)
{
	this->direction = dir;
	tankSprite->setVisible(false);
	sprites[dir]->setPosition(tankSprite->getPosition());
	sprites[dir]->setVisible(true);
	tankSprite = sprites[dir];
}
Direction Tank::getDirection()
{
	return this->direction;
}

Behaviour Tank::getBehaviour()
{
	return this->behaviour;
}

CCSprite * Tank::getTankSprite()
{
	return this->tankSprite;
}

void Tank::setSpeed(float speed)
{
	this->speed = speed;
}

float Tank::getSpeed()
{
	return this->speed;
}

TankKind Tank::getKind()
{
	return this->kind;
}
void Tank::setBehaviour(Behaviour staus)
{
	this->behaviour = staus;
}


