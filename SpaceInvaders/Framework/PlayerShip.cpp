#include "PlayerShip.h"
#include "entity.h"

#include <iostream>



PlayerShip::PlayerShip()
	:Entity()
{
	std::cout << " player ship created " << std::endl;
}


PlayerShip::~PlayerShip()
{
		
}

void
PlayerShip::Process(float deltatime)	
{
Entity::Process(deltatime);
}
