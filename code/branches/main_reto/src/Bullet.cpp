#include "Bullet.h"


Bullet::Bullet(SceneNode *mNode, Entity *mEntity, Vector3 mSpeed) : mNode(mNode), mEntity(mEntity), mSpeed(mSpeed)
{
	mNode->attachObject(mEntity);
}


Bullet::~Bullet()
{
}
