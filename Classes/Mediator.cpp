//////////////////////////////////////////////////////////////////////////
//	Copyright (c) 2011-2012 www.ipplelife.com
//	Copyright (c) 2011-2012 vainner
//	http://www.ippelife.com
//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/* 
�����㷨�࣬ ������
�����ͼ����������ɫ��ҡ��֮��Ľ��������ж���������ฺ��
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
	for (unsigned int i = 0 ; i < tanks->count() ; i++)   //ͳ��̹��
	{
		Tank * t = (Tank*)tanks->objectAtIndex(i);
		if(t == heroTank)    //Ӣ��̹����Ϊ��Ϸ������־�� ��ɾ��
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


void Mediator::controlMove(float dt)   //��������ƶ�
{
#ifdef IAMPHONE__
	//printf("Mediator Move \n ");
	CCPoint actual = heroTank->getTankSprite()->getPosition();
	//printf("%f %f\n",actual.x , actual.y);
	CCPoint tiledPointM,tiledPointL,tiledPointR;
	switch (jst->getDirectionByDir())   //��ȡ����,�ƶ���ʱ���ж�
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

void Mediator::sendBullet(float dt)    //������ƿ���
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
		if( Common::isPointOk(actual))  //����ӵ���������
		{
			CCPoint tiledPosition = tileMap->convertCoordGLToTile(actual);
			if(!TmxMap::isInside(tiledPosition.x,tiledPosition.y))
				return;
			switch(tileMap->getTiedType(tiledPosition))
			{
			case 0:   //����û����Ƭ���ǲ����ǿ��ܴ��������Ķ���
				{
					//������̹�˵���ײ
					for (unsigned int i = 0 ; i < tanks->count() ; i++)
					{
						
						Tank * tankTemp = (Tank*)tanks->objectAtIndex(i);
						if( (tankTemp->getKind() == HERO && t->getBulletType() == TANK )
							|| (tankTemp->getKind() == NORMAL && t->getBulletType() == ENEMY))
							continue;

						float x1 = tankTemp->getTankSprite()->getPosition().x - (tankTemp->getTankSprite()->getContentSize().width/2);   //���
						float x2 = tankTemp->getTankSprite()->getPosition().x + (tankTemp->getTankSprite()->getContentSize().width/2);   //�ҽ�
						float y1 = tankTemp->getTankSprite()->getPosition().y - (tankTemp->getTankSprite()->getContentSize().height/2);  //�½�
						float y2 = tankTemp->getTankSprite()->getPosition().y + (tankTemp->getTankSprite()->getContentSize().height/2);   //�Ͻ�

						if( actual.x >= x1 && actual.x <= x2 && actual.y >= y1 && actual.y <= y2 )  //�ӵ���̹����ײ��
						{
							t->setBehaviour(DEAD);
							t->explode();
							tankTemp->setBehaviour(DEAD);
							tankTemp->setVisible(false);
							tankTemp->release();
							return;     //���ﴦ��������ײ��return
						}
					}
					
					//���û����ײ,��ô����ӵ����ӵ��Ӽ�
					/* 
					//��Ϊ��δ�����Ӻ���android�ֻ���ɿ���
					CCRect origin = CCRectMake(
						t->getPosition().x - 5,
						t->getPosition().y - 5,
						10,
						10
						);
	
					for (int i =  0 ; i < bullets->count() ;i++)    //�������ӵ���ǽ�ڵ���ײ
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
			case 1:   //�����ƻ���
				t->setBehaviour(DEAD);
				t->explode();
				return;
				break;
			case 2:  //��ϷҪ����
				tileMap->getTiledAt(tiledPosition)->setScale(2.0);
				isGameOver = true;
				return;
				break;
			case 3:   //���ƻ���
				t->setBehaviour(DEAD);
				t->explode();
				needToDelete->addObject(tileMap->getTiledAt(tileMap->convertCoordGLToTile(t->getPosition())));  //���浽ɾ��
				break;
			}
		}

	}

	for(unsigned int i = 0; i < needToDelete->count() ; i++)
	{
		//CCSprite * t = *ij;    //ɾ��tiled��ʱ����Ҫȡ���丸�ף�Ȼ����ɾ��
		CCSprite * t = (CCSprite *)needToDelete->objectAtIndex(i);
		//t->release();
		CCNode * p = ((CCNode*)t)->getParent();  
		if(p)
			p->removeChild((CCNode*)t,true);
	}

	needToDelete->release();

}


void Mediator::addEnemy(float dt)  //����ӵ�ͼ�Ķ������ȡ������
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
		//CCStringToStringDictionary * temp = all->getObjectAtIndex(i);  //��ȡ��
		temp = (CCDictionary * ) all->objectAtIndex(i);
		//std::cout << temp->objectForKey("name")->toStdString() ;
		//if(temp->objectForKey("name")->toStdString() == std::string("enemy"))
		if( ((CCString*)temp->objectForKey("name"))->compare("enemy") == 0)
		{
			if(addedEnemys >= numberOfEnemys)
				break;
		//	printf("enemy posion : %d\n",temp->objectForKey("x")->toInt());

			float x = ((CCString*)temp->objectForKey("x"))->floatValue();     //��ȡ����
			float y = ((CCString*)temp->objectForKey("y"))->floatValue();     
			Tank * enemyTemp = Tank::getATank(tank_player,DOWN,NORMAL);    //����һ����̹��
			gameScene->addChild(enemyTemp,1);          //��ӽ�������
			enemyTemp->getTankSprite()->setPosition(ccp(x,y));   //����λ��
			tanks->addObject(enemyTemp);    //��ӵ��������ĵ���������

			addedEnemys++;
			leaveEnemys++;
		}
	}
}

void Mediator::enemyMoves(float dt)
{
	//printf("use enemy move \n");
	int map[mapHeight][mapWidth];  //����õ���ͼ���ݣ�ֻ�ʺ�mapHeight*mapWidth
	int dir[4][2] = {0,-1,0,1,-1,0,1,0};  //�ĸ����� ,��,�£���,��
	CCPoint heroTilePosition = tileMap->convertCoordGLToTile(heroTank->getTankSprite()->getPosition());
	
	

	for (unsigned int i = 0 ; i < tanks->count() ; i++)			//����õ����ез�̹�ˣ���������AI����
	{
		Tank * t = (Tank*)tanks->objectAtIndex(i);
		//����ÿ��̹�˶�Ӧ������ɨ��һ�µ�ͼ,��ǰǰ���̹���ƻ��˵�ͼ�Ժ�������Ӱ��
		for (int i = 0 ; i < mapHeight ; i++)
			for (int j = 0 ; j < mapWidth ; j++)
				map[i][j] = tileMap->getTiedType(ccp(j,i));
			
		// ��Ϊ����Ϊ0.05s ,�ʲ��ܲ��ô�ͳ�������㷨��
		// ֻ��ÿ���ҵ����о��߼��� ,���Ҿ����ܵļ�С�ӵ�����Ŀ��
		// 1.ÿ��ɨ����ǰ���Ƿ���ڵз�̹�ˣ�������ڲ���ǽ���谭�����Ͽ�ǹ��
		//    ͬ����ɨ����ǰ�����Ƿ�������࣬������ڣ� ����ȫ���ǿ��ƻ��ģ� ��ô��ǹ��

		// 2. �����ǽ��ס���������ǽ�� ��̽�ԵĿ�ǹ������̽�Ƿ��ܼ����ߣ�������ܻ�����
		//
		//
		//�з�̹��AI�㷨���֡�
		//2012��3��6�� 22:44:24
		// 
		/*����ʱ�� 2013��3��31��21:18:37
		1.ǰ���ܹ����ģ�һ�ɹ��� �� ����˵�з�̹�ˣ� ��ǽ

		*/
		if(t->getKind() != HERO)
		{
			//1.�ȼ��
			
			CCPoint tTilePosition = tileMap->convertCoordGLToTile(t->getTankSprite()->getPosition());
			int y = tTilePosition.y;
			int x = tTilePosition.x;
			
			while(true)  //̽��ǰ���̹��
			{
				x += dir[t->getDirection()][0];
				y += dir[t->getDirection()][1];  //ԭ������ǰ��
				if( !TmxMap::isInside(x,y) || map[x][y] == 1)  //����ǽ��
					break;
				int value = tileMap->getTiedType(ccp(x,y));
				if( ( (int)heroTilePosition.x == x && (int)heroTilePosition.y == y) || value == 2 || value == 3)  //����̹����
				{
					if(bullets->count() < 10 && value != 0)
					{
						Bullet * b = Bullet::bullet(t->getTankSprite()->getPosition(),t->getDirection(),ENEMY);
						bullets->addObject(b);
						gameScene->addChild(b);
					}
				}
			}
			
			//���ǰ��������û��
			y = tTilePosition.y;
			x = tTilePosition.x;
 			//{
				if(isCanMove(t,t->getDirection()))  //����Ծ�����
				{
					x += dir[t->getDirection()][0];
					y += dir[t->getDirection()][1];  //ԭ������ǰ��
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
	��������ײֻ����3����
	��һ�� �ƶ�����y ��
	�ڶ� x +- ���ʿ�ȵ�һ��
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

	//����ѡ��
	if(!(TmxMap::isInside(tiledPointM.x,tiledPointM.y) && TmxMap::isInside(tiledPointL.x,tiledPointL.y)
		&& TmxMap::isInside(tiledPointR.x,tiledPointR.y)))
		return false;

	if(tileMap->isEmpty(tiledPointM) && tileMap->isEmpty(tiledPointL) && tileMap->isEmpty(tiledPointR))  //�ж��Ƿ����ͼ����ײ
	{
		//�ж���̹���Ƿ�����ײ
		for (unsigned int i = 0 ; i < tanks->count() ; i++)
		{
			Tank * t = (Tank*)tanks->objectAtIndex(i);  //ȡ������̹��
			if(t == tank)  //������Լ����Ͳ��ж���
				continue;
			else
			{
				//����Ŀ��������� ,ע�����ﲻ�ܽ�������ѡȡ������ײ��ֻ��ʹ�÷����ԵĲ�������ײ
				float x1 = t->getTankSprite()->getPosition().x - (t->getTankSprite()->getContentSize().width/2);   //���
				float x2 = t->getTankSprite()->getPosition().x + (t->getTankSprite()->getContentSize().width/2);   //�ҽ�
				float y1 = t->getTankSprite()->getPosition().y - (t->getTankSprite()->getContentSize().height/2);  //�½�
				float y2 = t->getTankSprite()->getPosition().y + (t->getTankSprite()->getContentSize().height/2);   //�Ͻ�

				if(  y1 <= actual.y  && y2 >= actual.y && x1 <= actual.x && x2 >= actual.x)  //ֻ�ж���ǰ��
					return false;
				if(  y1 <= actualR.y  && y2 >= actualR.y && x1 <= actualR.x && x2 >= actualR.x)  //ֻ�ж���ǰ��
					return false;
				if(  y1 <= actualL.y  && y2 >= actualL.y && x1 <= actualL.x && x2 >= actualL.x)  //ֻ�ж���ǰ��
					return false;
			}
		}
		return true;
	}
	return false;
}

bool Mediator::gameOver(float dt)
{
	//1.Ӣ��̹�˹���
	if(heroTank->getBehaviour() == DEAD)
	{
		return true;
	}

	//2.�з�ȫ����
	if(leaveEnemys == 0 && addedEnemys != 0 )  
	{
		isVictory = true;
		return true;
	}

	//3. ���౬��
	if(isGameOver)
	{
		return true;
	}
	return false;
}