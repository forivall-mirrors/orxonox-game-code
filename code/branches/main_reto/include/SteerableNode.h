#ifndef __SteerableNode__
#define __SteerableNode__

#include "Ogre.h"

using namespace Ogre;

class SteerableNode : public SceneNode
{
public:
	SteerableNode(SceneManager*, const Ogre::String&);
	SteerableNode(SceneManager*);
	virtual ~SteerableNode();

	virtual bool initialise();

protected:
	SceneNode *mNode;
};


#endif
