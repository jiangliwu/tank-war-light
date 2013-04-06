//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.ippelife.com
//////////////////////////////////////////////////////////////////////////

#ifndef __TMXMAP_TOOLS_H__
#define __TMXMAP_TOOLS_H__
#include "cocos2d.h"

class TmxMap : public cocos2d::CCTMXTiledMap
{
public:
	static TmxMap * initMap(const char * path);
	cocos2d::CCPoint convertCoordGLToTile(cocos2d::CCPoint p);
	int getTiedType(cocos2d::CCPoint p);
	bool isEmpty(cocos2d::CCPoint p);
	cocos2d::CCSprite * getTiledAt(cocos2d::CCPoint p);
	static bool isInside(int x, int y);
protected:
private:
};

#endif