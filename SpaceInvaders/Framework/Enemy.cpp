#include "Enemy.h"

#include "sprite.h"

Enemy::Enemy()
	:Entity()
{
}


Enemy::~Enemy()
{
	delete m_pSprite;
	m_pSprite = 0;
}

void Enemy::Process(float deltatime)
{
	Entity::Process(deltatime);
}
