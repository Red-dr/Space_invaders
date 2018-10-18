#include "Bullet.h"

#include "sprite.h"

Bullet::Bullet()
	:Entity()
{
}


Bullet::~Bullet()
{
	delete m_pSprite;
	m_pSprite = 0;
}

void
Bullet::Process(float deltatime)
{
	Entity::Process(deltatime);
}
