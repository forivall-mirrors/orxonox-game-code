#ifndef __OrxonoxShip__
#define __OrxonoxShip__

#include "Ogre.h"

using namespace Ogre;

class OrxonoxShip : public Entity
{
public:
	OrxonoxShip(const Ogre::String, Ogre::MeshPtr&);
	virtual ~OrxonoxShip();

	virtual bool initialise();

protected:
};


#endif
