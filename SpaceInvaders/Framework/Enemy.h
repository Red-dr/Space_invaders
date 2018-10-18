#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	void Process(float deltatime);
	


private:
	
	
};

#endif // __ENEMY_H__

