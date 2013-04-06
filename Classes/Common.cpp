#include "Common.h"
#include <math.h>

float Common::getDistance(CCPoint a , CCPoint b)
{
	return sqrt((a.x - b.x)* (a.x - b.x) + (a.y -b.y)* (a.y - b.y));
}

bool Common::isPointOk(CCPoint p)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if(p.x >= 0 && p.y >= 0 && p.x <= winSize.width && p.y <= winSize.height)
		return true;
	return false;
}

Direction Common::intToDirection(int dir)
{
	switch(dir)
	{
	case 0:
		return UP;
	case 1:
		return DOWN;
	case 2:
		return LEFT;
	case 3:
		return RIGHT;
	}

	return UP;
}

long Common::millisecondNow()
{
	
	struct cocos2d::cc_timeval now; 
	cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL); 
	return (now.tv_sec * 1000 + now.tv_usec / 1000); 
	
}