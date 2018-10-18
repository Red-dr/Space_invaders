#include "explosion.h"
#include "animatedsprite.h"

Explosion::Explosion()
	:Entity()
{

}


Explosion::~Explosion()
{
	delete m_pAnimatedSprite;
	m_pSprite = 0;
}

void
Explosion::Process(float deltatime)
{
	Entity::Process(deltatime);
}
void
Explosion::Draw(BackBuffer& backBuffer)
{
	m_pAnimatedSprite->SetX(static_cast<int>(m_x));
	m_pAnimatedSprite->SetY(static_cast<int>(m_y));
	m_pAnimatedSprite->Draw(backBuffer);
}

float 
Explosion::GetSpritePositionX() const
{
	float x;
	x = m_x + (m_pAnimatedSprite->GetFrameWidth() / 4.5f);
	return x;
}

float
Explosion::GetSpritePositionY() const
{
	float y;
	y = m_y + (m_pAnimatedSprite->GetHeight() / 3.5f);
	return y;
}