//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.ippelife.com
//////////////////////////////////////////////////////////////////////////

#include "TmxMap.h"
#include <iostream>
#include "res.h"
using namespace cocos2d;


//װ�ص�ͼ
TmxMap * TmxMap::initMap(const char * path)
{
	TmxMap * map = new TmxMap;
	if(path != NULL)
	{
		if(map->initWithTMXFile(path))
		{
			map->autorelease();
			return map;
		}
		CC_SAFE_DELETE(map);
		return NULL;
	}
	return NULL;
}


//��GL����ϵת��Ϊ tiled����
CCPoint TmxMap::convertCoordGLToTile(cocos2d::CCPoint p)
{
	int x = (int)p.x / this->getTileSize().width;
	
	int y = (int)(((this->getMapSize().height) * this->getTileSize().height) - p.y) / this->getTileSize().height;
	//std::cout << p.x << " " << p.y << " " <<  "---- >"  << x << " " <<y << endl;
	return CCPointMake((float)x,(float)y);
}

//�ж��Ƿ����ƻ�
int TmxMap::getTiedType(cocos2d::CCPoint p)   //��tiled��
{
	/*

	�ж�һ��tiled���ͣ�
	�յ�Ϊ0
	�����ƻ���Ϊ1
	����Ϊ2
	���ƻ���Ϊ3
	*/
	int cannot[14] = {41,42,43,44,45,25,26,27,33,34,35,36,37,32};
	int heart = 31;
	if(!isInside(p.x,p.y))
		return 0;
	int value = this->layerNamed("content")->tileGIDAt(p);
	for (int i = 0 ; i < 14 ; i++)
	{
		if(value == cannot[i])
			return 1;
	}
	if(value == 0)
		return 0;

	if(value == heart)
		return 2;

	return 3;

}

//�ж��Ƿ�Ϊ��
bool TmxMap::isEmpty(cocos2d::CCPoint p)
{
	if(!this->layerNamed("content")->tileGIDAt(p))
		return true;
	return false;
}

CCSprite * TmxMap::getTiledAt(cocos2d::CCPoint p)
{

	return this->layerNamed("content")->tileAt(p);
}


bool TmxMap::isInside(int x, int y)
{
	return !( x < 0 || x >= mapWidth || y < 0 || y >= mapHeight);
}