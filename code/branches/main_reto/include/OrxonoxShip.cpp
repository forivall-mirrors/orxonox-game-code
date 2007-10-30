#include "OrxonoxShip.h"


OrxonoxShip::OrxonoxShip(const Ogre::String name, Ogre::MeshPtr &mesh)
	: Entity(name, mesh)
{
}


OrxonoxShip::~OrxonoxShip()
{
}


bool OrxonoxShip::initialise()
{
	return true;
}
