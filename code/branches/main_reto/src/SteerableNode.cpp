#include "SteerableNode.h"


SteerableNode::SteerableNode(SceneManager *creator, const Ogre::String &name) : SceneNode(creator, name)
{

}


SteerableNode::SteerableNode(SceneManager *creator) : SceneNode(creator)
{

}


SteerableNode::~SteerableNode()
{
}


bool SteerableNode::initialise()
{
	return true;
}
