#ifndef __PLAYERSHIP_H__
#define __PLAYERSHIP_H__

#include "entity.h"

class PlayerShip : public Entity

{
public:
	PlayerShip();
	~PlayerShip();
	
	void Process(float deltatime);

private:

	
	

};

#endif // !__PLAYERSHIP_H__


