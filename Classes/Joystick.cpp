
#include "Joystick.h"
#include "math.h"
#include <iostream>
Joystick::Joystick(void) 
{ 
} 

Joystick::~Joystick(void) 
{ 
} 

void Joystick::updatePos(float dt) 
{ 
	jsSprite->setPosition(ccpAdd(jsSprite->getPosition(), ccpMult(ccpSub(currentPoint, jsSprite->getPosition()), 0.5))); 
} 

void Joystick::Active() 
{ 
	if(!active) 
	{ 
		active = true; 
		schedule(schedule_selector(Joystick::updatePos));   // 添加刷新函数 
		//CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);  // 添加触摸委托 
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,false);
	} 
} 

void Joystick::Inactive() 
{ 
	if(active) 
	{ 
		active = false; 
		this->unschedule(schedule_selector(Joystick::updatePos));    // 删除刷新函数 
		//CCTouchDispatcher::sharedDispatcher()->removeDelegate(this); // 删除委托 
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	} 
} 

bool Joystick::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) 
{ 
	if(!active) 
		return false; 

	//CCPoint touchPoint = pTouch->locationInView(pTouch->view()); 
	CCPoint touchPoint = pTouch->getLocation();
	//touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint); 
	if(ccpDistance(touchPoint, centerPoint) > radius) 
		return false; 

	currentPoint = touchPoint; 
	return true; 
} 

void Joystick::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) 
{ 
	//CCPoint touchPoint = pTouch->locationInView(pTouch->view()); 
	CCPoint touchPoint = pTouch->getLocation();
	touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint); 
	if(ccpDistance(touchPoint, centerPoint) > radius) 
	{ 
		currentPoint = ccpAdd(centerPoint, ccpMult(ccpNormalize(ccpSub(touchPoint, centerPoint)), radius)); 
	} 
	else
	{ 
		currentPoint = touchPoint; 
	} 
} 

void Joystick::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) 
{ 
	currentPoint = centerPoint; 
} 

CCPoint Joystick::getDirection() 
{ 
	if(centerPoint.x == currentPoint.x && centerPoint.y == currentPoint.y)
		return CCPointZero;
	return ccpNormalize(ccpSub(centerPoint, currentPoint));   
}   

float Joystick::getVelocity()   
{   
	return ccpDistance(centerPoint, currentPoint);   
}   

Joystick* Joystick:: JoystickWithCenter(CCPoint aPoint, float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg) 
{   
	Joystick *jstick=Joystick::create();   
	jstick->initWithCenter(aPoint,aRadius,aJsSprite,aJsBg);   
	return jstick;   
}   

Joystick* Joystick::initWithCenter(CCPoint aPoint, float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg) 
{   
	active = false;   
	radius = aRadius;   
	centerPoint = aPoint;   
	currentPoint = centerPoint;   
	jsSprite = aJsSprite;   
	jsBg = aJsBg;
	jsSprite->setPosition(centerPoint);   
	aJsBg->setPosition(centerPoint);   
	this->addChild(jsSprite,1);   
	this->addChild(aJsBg,0);   
	return this;   
}

void Joystick::setOpacity(float op)
{
	op = op*255;
	this->jsSprite->setOpacity(op);
	this->jsBg->setOpacity(op);
}


Direction Joystick::getDirectionByDir()
{
	CCPoint temp = getDirection();
	if(!(temp.x || temp.y)  || getVelocity() < 0.3)
		return STAND;

	if(fabsf(temp.x) > fabsf(temp.y))   // x方向
	{
		if(temp.x < 0) 
			return RIGHT;
		return LEFT;
	}
	else
	{
		if(temp.y < 0)
			return UP;
		return DOWN;
	}
	
}