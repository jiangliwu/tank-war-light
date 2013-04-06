#ifndef __COMMON_TOOLS_H__
#define __COMMON_TOOLS_H__
#include "cocos2d.h"
#include "type.h"
using namespace cocos2d;
class Common
{
public:
	static float getDistance(CCPoint a , CCPoint b);
	static bool isPointOk(CCPoint p);
	static Direction intToDirection(int dir);
	static long millisecondNow()  ;
	
protected:
private:
};



#endif