#include <OgreSceneManager.h>
#include <string>

#include "../orxonox.h"
#include "../../tinyxml/tinyxml.h"
#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"
#include "../core/Debug.h"

#include "Entity.h"

namespace orxonox
{
    CreateFactory(Entity);

    Entity::Entity()
    {
        RegisterObject(Entity);
    }

    Entity::~Entity()
    {
    }

    void Entity::loadParams(TiXmlElement* xmlElem)
    {
    	Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();

    	if (xmlElem->Attribute("name") && xmlElem->Attribute("src") && xmlElem->Attribute("node"))
    	{
    		std::string name = xmlElem->Attribute("name");
    		std::string src = xmlElem->Attribute("src");
    		std::string node = xmlElem->Attribute("node");

			Ogre::Entity* entity = mgr->createEntity(name, src);

		    Ogre::SceneNode* sceneNode = (Ogre::SceneNode*)mgr->getRootSceneNode()->getChild(node);
		    sceneNode->attachObject((Ogre::MovableObject*)entity);

	    	COUT(4) << "Loader: Created entity "<< name <<" with source " << src  << " at node " << node  << std::endl << std::endl;
    	}
   }

}
