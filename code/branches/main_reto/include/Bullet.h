#ifndef __Bullet__
#define __Bullet__

#include "Ogre.h"

using namespace Ogre;

class Bullet
{
public:
	SceneNode *mNode;
	Entity *mEntity;
	Vector3 mSpeed;

	Bullet(SceneNode*, Entity*, Vector3);
	~Bullet();

protected:

};


#endif
