#ifndef __TYPE_CORE_H__
#define __TYPE_CORE_H__

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	STAND,
}  Direction;

//char* getDirection(Direction direction);

typedef enum{
	MOVE,
	DEAD
} Behaviour;

//char* getBehaviour(Behaviour behaviour);

typedef enum
{
	TANK,
	ENEMY,
} BulletKind;

typedef enum {
	HERO,
	NORMAL,
}  TankKind;
#endif