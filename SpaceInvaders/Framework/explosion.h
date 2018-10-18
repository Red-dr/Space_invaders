#ifndef __EXPOSION_H__
#define __EXPOSION_H__

//local inculeds

#include "entity.h"



class Explosion : public Entity
{

public: 

	Explosion();
	~Explosion();

	void Process(float deltatime);

	void Draw(BackBuffer& backBuffer);

	

	float GetSpritePositionX() const;

	float GetSpritePositionY() const;


private:



};
#endif // __EXPOSION_H__

