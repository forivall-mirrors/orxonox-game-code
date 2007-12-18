#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>

#include <string>

#include "../Orxonox.h"
#include "../GraphicsEngine.h"
#include "../../tinyxml/tinyxml.h"
#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"
#include "../core/Debug.h"

#include "SpaceshipSteeringObject.h"

#include "../SpaceshipSteering.h"

namespace orxonox
{
    CreateFactory(SpaceshipSteeringObject);

    SpaceshipSteeringObject::SpaceshipSteeringObject()
    {
        RegisterObject(SpaceshipSteeringObject);
    }

    SpaceshipSteeringObject::~SpaceshipSteeringObject()
    {
    }

    void SpaceshipSteeringObject::loadParams(TiXmlElement* xmlElem)
    {
    	SpaceshipSteering* steering = orxonox::Orxonox::getSingleton()->getSteeringPointer();

    	if (xmlElem->Attribute("node") && xmlElem->Attribute("forward") && xmlElem->Attribute("rotateupdown") && xmlElem->Attribute("rotaterightleft") && xmlElem->Attribute("looprightleft"))
    	{
    		std::string nodeStr = xmlElem->Attribute("node");
    		std::string forwardStr = xmlElem->Attribute("forward");
    		std::string rotateupdownStr = xmlElem->Attribute("rotateupdown");
    		std::string rotaterightleftStr = xmlElem->Attribute("rotaterightleft");
    		std::string looprightleftStr = xmlElem->Attribute("looprightleft");

	    	float forward, rotateupdown, rotaterightleft, looprightleft;
    	 	String2Number<float>(forward, forwardStr);
    	 	String2Number<float>(rotateupdown, rotateupdownStr);
    	 	String2Number<float>(rotaterightleft, rotaterightleftStr);
    	 	String2Number<float>(looprightleft, looprightleftStr);

    		steering = new SpaceshipSteering(forward, rotateupdown, rotaterightleft, looprightleft);

	    	Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();
		    Ogre::SceneNode* sceneNode = (Ogre::SceneNode*)mgr->getRootSceneNode()->getChild(nodeStr);

    		steering->addNode(sceneNode);




	    	COUT(4) << "Loader: Initialized spaceship steering at node " << nodeStr << " values "<< forward << " " << rotateupdown << " "<< rotaterightleft << " "<< looprightleft << " "<< std::endl << std::endl;
    	}
   }
}
