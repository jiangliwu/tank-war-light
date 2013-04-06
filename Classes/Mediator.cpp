//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.ippelife.com
//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/* 
核心算法类， 管理器
管理地图，场景，角色，摇杆之间的交互，所有动作由这个类负责
*/
/************************************************************************/
#include "Mediator.h"
#include "res.h"
#include "Common.h"
#include <iostream>
#include <queue>

Mediator::~Mediator()
{
	/*delete tileMap;
	delete heroTank;
	delete gameScene;
	delete jst;
	delete fire;
	bullets->release();
	tanks->release();
	enemysBullets->release();*/
}

void Mediator::addHeroTank(Tank *hero)
{
	this->heroTank = hero;
	tanks->addObject(heroTank);
}

void Mediator::addTileMap(TmxMap *tileMap)
{
	this->tileMap = tileMap;
}

void Mediator::addGameScene(CCLayer *gameScene)
{
	this->gameScene = gameScene;
}

void Mediator::addJoyStick(Joystick * jst)
{
	this->jst = jst;
}

void Mediator::addFireButton(CCMenuItemImage * fire)
{
	this->fire = fire;
}


bool Mediator::init()
{
	bool bRet =  false;

	do 
	{
		winSize = CCDirector::sharedDirector()->getWinSize();
		bullets = CCArray::create();
		tanks = CCArray::create();
		enemysBullets = CCArray::create();
		bullets->retain();
		tanks->retain();
		enemysBullets->retain();
		this->numberOfEnemys = 0;
		this->addedEnemys = 0;
		this->leaveEnemys = 0;
		
		isGameOver = false;
		isVictory = false;
		CCLOG("Mediatoer  init success!");
	} while (0);
	return bRet;
}


void Mediator::deleteBulletsAndTanks(float dt)
{
	
	CCArray *needToDelete = CCArray::create();
	CCArray *tankNeedToDelete = CCArray::create();
	for ( unsigned int  i = 0 ; i < bullets->count(); i++)
	{
		Bullet * t = (Bullet*)bullets->objectAtIndex(i);
		if(t->getBehaviour() == DEAD || !Common::isPointOk(t->getPosition()))
			needToDelete->addObject(t);
	}
	for (unsigned int i = 0 ; i < tanks->count() ; i++)   //统计坦克
	{
		Tank * t = (Tank*)tanks->objectAtIndex(i);
		if(t == heroTank)    //英雄坦克作为游戏结束标志， 不删除
			continue;
		if(t->getBehaviour() == DEAD)
			tankNeedToDelete->addObject(t);
	}

	//delete
	for (unsigned int i = 0 ; i < tankNeedToDelete->count() ; i++)
	{
		Tank * t = (Tank*)tankNeedToDelete->objectAtIndex(i);
		tanks->removeObject(t);
		CCNode * p = t->getParent();
		if(p)
		{
			p->removeChild((CCNode*)t,true);
			leaveEnemys--;
		}
	}
	for(unsigned int i = 0 ; i < needToDelete->count() ; i++)
	{
		Bullet * t = (Bullet *)needToDelete->objectAtIndex(i);
		bullets->removeObject(t);
		CCNode * p = t->getParent();
		if(p)
		{
			p->removeChild((CCNode*)t,true);
		}
	}
	needToDelete->release();
	tankNeedToDelete->release();
}


void Mediator::controlMove(float dt)   //这里控制移动
{
#ifdef IAMPHONE__
	//printf("Mediator Move \n ");
	CCPoint actual = heroTank->getTankSprite()->getPosition();
	//printf("%f %f\n",actual.x , actual.y);
	CCPoint tiledPointM,tiledPointL,tiledPointR;
	switch (jst->getDirectionByDir())   //获取方向,移动的时候并判断
	{
	case UP:
		if(isCanMove(heroTank,UP))
			heroTank->up();
		break;
	case DOWN:
		if(isCanMove(heroTank,DOWN))
			heroTank->down();
		break;
	case LEFT:
		if(isCanMove(heroTank,LEFT))
			heroTank->left();
		break;
	case RIGHT:
		if(isCanMove(heroTank,RIGHT))
			heroTank->right();
		break;
	}
	#endif // IAMPHONE__

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if(KEY_DOWN(VK_UP))
	{
		if(isCanMove(heroTank,UP))
			heroTank->move(UP);
		return;
	}
	if(KEY_DOWN(VK_DOWN))
	{
		if(isCanMove(heroTank,DOWN))
			heroTank->move(DOWN);
		return;
	}
	if(KEY_DOWN(VK_LEFT))
	{
		if(isCanMove(heroTank,LEFT))
			heroTank->move(LEFT);
		return;
	}
	if(KEY_DOWN(VK_RIGHT))
	{
		if(isCanMove(heroTank,RIGHT))
			heroTank->move(RIGHT);
		return;
	}
#endif
}

void Mediator::sendBullet(float dt)    //这里控制开火
{
#ifdef IAMPHONE__
	if(fire->isEnabled())
	{
		
		Bullet * b = Bullet::bullet(heroTank->getTankSprite()->getPosition(),heroTank->getDirection(),TANK);
		gameScene->addChild(b,1);
		//bullets->addObject(b);
	}
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if(KEY_DOWN(VK_SPACE))
	{
		Bullet * b = Bullet::bullet(heroTank->getTankSprite()->getPosition(),heroTank->getDirection(),TANK);
		gameScene->addChild(b,1);
		bullets->addObject(b);
	}
#endif
	
}



void Mediator::bulletProcess(float dt)
{

	CCArray *needToDelete = CCArray::create();
	for(unsigned int i = 0 ; i < bullets->count() ; i++)
	{
		Bullet * t = (Bullet*)bullets->objectAtIndex(i);
		CCPoint actual = t->getPosition();
		if( Common::isPointOk(actual))  //如果子弹在区域内
		{
			CCPoint tiledPosition = tileMap->convertCoordGLToTile(actual);
			if(!TmxMap::isInside(tiledPosition.x,tiledPosition.y))
				return;
			switch(tileMap->getTiedType(tiledPosition))
			{
			case 0:   //这里没有瓦片，那不就是可能存在其他的东西
				{
					//与其他坦克的碰撞
					for (unsigned int i = 0 ; i < tanks->count() ; i++)
					{
						
						Tank * tankTemp = (Tank*)tanks->objectAtIndex(i);
						if( (tankTemp->getKind() == HERO && t->getBulletType() == TANK )
							|| (tankTemp->getKind() == NORMAL && t->getBulletType() == ENEMY))
							continue;

						float x1 = tankTemp->getTankSprite()->getPosition().x - (tankTemp->getTankSprite()->getContentSize().width/2);   //左界
						float x2 = tankTemp->getTankSprite()->getPosition().x + (tankTemp->getTankSprite()->getContentSize().width/2);   //右界
						float y1 = tankTemp->getTankSprite()->getPosition().y - (tankTemp->getTankSprite()->getContentSize().height/2);  //下届
						float y2 = tankTemp->getTankSprite()->getPosition().y + (tankTemp->getTankSprite()->getContentSize().height/2);   //上界

						if( actual.x >= x1 && actual.x <= x2 && actual.y >= y1 && actual.y <= y2 )  //子弹与坦克碰撞了
						{
							t->setBehaviour(DEAD);
							t->explode();
							tankTemp->setBehaviour(DEAD);
							tankTemp->setVisible(false);
							tankTemp->release();
							return;     //这里处理完了碰撞就return
						}
					}
					
					//如果没有碰撞,那么检测子弹与子弹子间
					/* 
					//因为这段代码添加后在android手机造成卡顿
					CCRect origin = CCRectMake(
						t->getPosition().x - 5,
						t->getPosition().y - 5,
						10,
						10
						);
	
					for (int i =  0 ; i < bullets->count() ;i++)    //这里是子弹与墙壁的碰撞
					{
						
						Bullet *bTemp = bullets->getObjectAtIndex(i);
						if(t == bTemp)
							continue;
						if(t->getBulletType() == bTemp->getBulletType())
							continue;

						CCRect bRec = CCRectMake(
							bTemp->getPosition().x - 5,
							bTemp->getPosition().y - 5,
							10,
							10
							);
						if(CCRect::CCRectIntersectsRect(origin,bRec))
						{
							t->setBehaviour(DEAD);
							bTemp->setBehaviour(DEAD);
							t->explode();
							bTemp->explode();
							return;
						}
					}*/
				}
				
				break;
			case 1:   //不能破坏的
				t->setBehaviour(DEAD);
				t->explode();
				return;
				break;
			case 2:  //游戏要结束
				tileMap->getTiledAt(tiledPosition)->setScale(2.0);
				isGameOver = true;
				return;
				break;
			case 3:   //能破坏的
				t->setBehaviour(DEAD);
				t->explode();
				needToDelete->addObject(tileMap->getTiledAt(tileMap->convertCoordGLToTile(t->getPosition())));  //缓存到删除
				break;
			}
		}

	}

	for(unsigned int i = 0; i < needToDelete->count() ; i++)
	{
		//CCSprite * t = *ij;    //删除tiled的时候先要取得其父亲，然后再删除
		CCSprite * t = (CCSprite *)needToDelete->objectAtIndex(i);
		//t->release();
		CCNode * p = ((CCNode*)t)->getParent();  
		if(p)
			p->removeChild((CCNode*)t,true);
	}

	needToDelete->release();

}


void Mediator::addEnemy(float dt)  //这里从地图的对象层中取出属性
{
	
	CCTMXObjectGroup * data = tileMap->objectGroupNamed("enemy");
	//if(data == NULL)
	//	printf("Object Group Load faile\n");
	CCDictionary * numbers = data->objectNamed("count");
	//if(numbers == NULL)
	//	printf("count load faile\n");
	this->numberOfEnemys = ((CCString*)numbers->objectForKey("count"))->intValue();
	//printf("%d    %d    %d\n",numberOfEnemys , addedEnemys , leaveEnemys);

	if(addedEnemys >= numberOfEnemys)
		return;

	//CCMutableArray<CCStringToStringDictionary *> * all = data->getObjects();
	CCArray* all = data->getObjects();
	CCDictionary * temp;
	for (unsigned int i = 0 ; i < all->count() ; i++)
	{
		//CCStringToStringDictionary * temp = all->getObjectAtIndex(i);  //获取到
		temp = (CCDictionary * ) all->objectAtIndex(i);
		//std::cout << temp->objectForKey("name")->toStdString() ;
		//if(temp->objectForKey("name")->toStdString() == std::string("enemy"))
		if( ((CCString*)temp->objectForKey("name"))->compare("enemy") == 0)
		{
			if(addedEnemys >= numberOfEnemys)
				break;
		//	printf("enemy posion : %d\n",temp->objectForKey("x")->toInt());

			float x = ((CCString*)temp->objectForKey("x"))->floatValue();     //获取坐标
			float y = ((CCString*)temp->objectForKey("y"))->floatValue();     
			Tank * enemyTemp = Tank::getATank(tank_player,DOWN,NORMAL);    //构造一个新坦克
			gameScene->addChild(enemyTemp,1);          //添加进来场景
			enemyTemp->getTankSprite()->setPosition(ccp(x,y));   //设置位置
			tanks->addObject(enemyTemp);    //添加到控制器的敌人数组中

			addedEnemys++;
			leaveEnemys++;
		}
	}
}

void Mediator::enemyMoves(float dt)
{
	//printf("use enemy move \n");
	int map[mapHeight][mapWidth];  //这里得到地图数据，只适合mapHeight*mapWidth
	int dir[4][2] = {0,-1,0,1,-1,0,1,0};  //四个方向 ,上,下，左,右
	CCPoint heroTilePosition = tileMap->convertCoordGLToTile(heroTank->getTankSprite()->getPosition());
	
	

	for (unsigned int i = 0 ; i < tanks->count() ; i++)			//这里得到所有敌方坦克，遍历运行AI代码
	{
		Tank * t = (Tank*)tanks->objectAtIndex(i);
		//对于每个坦克都应该重新扫描一下地图,以前前面的坦克破坏了地图对后面的造成影响
		for (int i = 0 ; i < mapHeight ; i++)
			for (int j = 0 ; j < mapWidth ; j++)
				map[i][j] = tileMap->getTiedType(ccp(j,i));
			
		// 因为周期为0.05s ,故不能采用传统的搜索算法，
		// 只需每次找到最有决策即可 ,并且尽可能的减小子弹的数目。
		// 1.每次扫描其前面是否存在敌方坦克，如果存在并且墙无阻碍，果断开枪。
		//    同样的扫描正前方，是否存在心脏，如果存在， 并且全部是可破坏的， 那么开枪。

		// 2. 如果被墙挡住，如果是土墙， 试探性的开枪，再试探是否能继续走，如果不能换方向。
		//
		//
		//敌方坦克AI算法部分。
		//2012年3月6日 22:44:24
		// 
		/*更新时间 2013年3月31日21:18:37
		1.前面能攻击的，一律攻击 ， 比如说敌方坦克， 土墙

		*/
		if(t->getKind() != HERO)
		{
			//1.先检测
			
			CCPoint tTilePosition = tileMap->convertCoordGLToTile(t->getTankSprite()->getPosition());
			int y = tTilePosition.y;
			int x = tTilePosition.x;
			
			while(true)  //探测前面的坦克
			{
				x += dir[t->getDirection()][0];
				y += dir[t->getDirection()][1];  //原方向向前。
				if( !TmxMap::isInside(x,y) || map[x][y] == 1)  //碰到墙壁
					break;
				int value = tileMap->getTiedType(ccp(x,y));
				if( ( (int)heroTilePosition.x == x && (int)heroTilePosition.y == y) || value == 2 || value == 3)  //碰到坦克了
				{
					if(bullets->count() < 10 && value != 0)
					{
						Bullet * b = Bullet::bullet(t->getTankSprite()->getPosition(),t->getDirection(),ENEMY);
						bullets->addObject(b);
						gameScene->addChild(b);
					}
				}
			}
			
			//如果前面的情况都没有
			y = tTilePosition.y;
			x = tTilePosition.x;
 			//{
				if(isCanMove(t,t->getDirection()))  //如果仍旧能走
				{
					x += dir[t->getDirection()][0];
					y += dir[t->getDirection()][1];  //原方向向前。
					if(TmxMap::isInside(x,y) && map[x][y] == 3)
					{
						if(bullets->count() < 10)
						{
							Bullet * b = Bullet::bullet(t->getTankSprite()->getPosition(),t->getDirection(),ENEMY);
							bullets->addObject(b);
							gameScene->addChild(b);
						}
					}
					t->move(t->getDirection());
					
				}
				else
				{
					for(int i = 1 ; i < 4 ; i++)
					{
						int j =rand()%4;
						if(isCanMove(t,Common::intToDirection((t->getDirection()+j)%4)))
							t->move(Common::intToDirection((t->getDirection()+j)%4));
					}
				}
			
		}
	}
}

bool Mediator::isCanMove(Tank * tank , Direction dir)
{
	/*
	这里检测碰撞只需检测3个点
	第一个 移动到的y 点
	第二 x +- 材质宽度的一半
	*/

	CCPoint actual = tank->getTankSprite()->getPosition();
	CCPoint actualL,actualR;
	CCPoint tiledPointM,tiledPointL,tiledPointR;
	tank->changDirectionGUI(dir);
	

	switch(dir)
	{
	case UP:
		actual.y += tank->getTankSprite()->getContentSize().height/2 + heroTank->getSpeed();
		actualR = actual;
		actualL = actual;
		actualR.x += COLLIDE_FIX;
		actualL.x -= COLLIDE_FIX;
		break;
	case DOWN:
		
		actual.y	-= tank->getTankSprite()->getContentSize().height/2 + heroTank->getSpeed();
		actualR = actual;
		actualL = actual;
		actualR.x	+= COLLIDE_FIX;
		actualL.x	-= COLLIDE_FIX;
		break;
	case LEFT:
		actual.x -= tank->getTankSprite()->getContentSize().width/2 + heroTank->getSpeed();
		actualR = actual;
		actualL = actual;
		actualL.y += COLLIDE_FIX;
		actualR.y -= COLLIDE_FIX;
		break;
	case RIGHT:
		actual.x += tank->getTankSprite()->getContentSize().width/2 + heroTank->getSpeed();
		actualR = actual;
		actualL = actual;
		actualL.y += COLLIDE_FIX;
		actualR.y -= COLLIDE_FIX;
		break;
	}

	if(!Common::isPointOk(actual))
		return false;

	tiledPointM = tileMap->convertCoordGLToTile(actual);
	tiledPointL = tileMap->convertCoordGLToTile(actualL);
	tiledPointR = tileMap->convertCoordGLToTile(actualR);

	//建立选区
	if(!(TmxMap::isInside(tiledPointM.x,tiledPointM.y) && TmxMap::isInside(tiledPointL.x,tiledPointL.y)
		&& TmxMap::isInside(tiledPointR.x,tiledPointR.y)))
		return false;

	if(tileMap->isEmpty(tiledPointM) && tileMap->isEmpty(tiledPointL) && tileMap->isEmpty(tiledPointR))  //判断是否与地图有碰撞
	{
		//判断与坦克是否有碰撞
		for (unsigned int i = 0 ; i < tanks->count() ; i++)
		{
			Tank * t = (Tank*)tanks->objectAtIndex(i);  //取得其他坦克
			if(t == tank)  //如果是自己，就不判断了
				continue;
			else
			{
				//建立目标矩形区域 ,注意这里不能建立矩形选取进行碰撞，只能使用方向性的测试性碰撞
				float x1 = t->getTankSprite()->getPosition().x - (t->getTankSprite()->getContentSize().width/2);   //左界
				float x2 = t->getTankSprite()->getPosition().x + (t->getTankSprite()->getContentSize().width/2);   //右界
				float y1 = t->getTankSprite()->getPosition().y - (t->getTankSprite()->getContentSize().height/2);  //下届
				float y2 = t->getTankSprite()->getPosition().y + (t->getTankSprite()->getContentSize().height/2);   //上界

				if(  y1 <= actual.y  && y2 >= actual.y && x1 <= actual.x && x2 >= actual.x)  //只判断正前方
					return false;
				if(  y1 <= actualR.y  && y2 >= actualR.y && x1 <= actualR.x && x2 >= actualR.x)  //只判断正前方
					return false;
				if(  y1 <= actualL.y  && y2 >= actualL.y && x1 <= actualL.x && x2 >= actualL.x)  //只判断正前方
					return false;
			}
		}
		return true;
	}
	return false;
}

bool Mediator::gameOver(float dt)
{
	//1.英雄坦克挂了
	if(heroTank->getBehaviour() == DEAD)
	{
		return true;
	}

	//2.敌方全挂了
	if(leaveEnemys == 0 && addedEnemys != 0 )  
	{
		isVictory = true;
		return true;
	}

	//3. 心脏爆了
	if(isGameOver)
	{
		return true;
	}
	return false;
}