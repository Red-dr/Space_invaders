#ifndef __BULLET_H__
#define __BULLET_H__

#include "entity.h"

class Bullet : public Entity

{
public:
	Bullet();
	~Bullet();

	void Process(float deltatime);

private:

};


#endif // !__BULLET_H__


