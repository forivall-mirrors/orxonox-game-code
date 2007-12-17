#include <OgreSceneManager.h>
#include <string>

#include "../orxonox.h"
#include "../../tinyxml/tinyxml.h"
#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"

#include "BaseEntity.h"

namespace orxonox
{
    CreateFactory(BaseEntity);

    BaseEntity::BaseEntity()
    {
        RegisterObject(BaseEntity);
        valid=false;
    }

    BaseEntity::~BaseEntity()
    {
    }

    void BaseEntity::loadParams(TiXmlElement* xmlElem)
    {
    	if (xmlElem->Attribute("name") && xmlElem->Attribute("src"))
    	{
        // save params
        name_ = xmlElem->Attribute("name");
        mesh_ = xmlElem->Attribute("src");
        std::string node = xmlElem->Attribute("node");
        
        // register variables to be synchronised
        registerAllVariables();
        valid=true;
        create();

	    	std::cout << "Loader: Created entity "<< name_ <<" with source " << mesh_  << " at node " << node  << std::endl << std::endl;
    	}
   }
   
   void BaseEntity::registerAllVariables(){
     WorldEntity::registerAllVariables();
     registerVar(&name_, name_.length()+1, network::STRING);
     registerVar(&mesh_, mesh_.length()+1, network::STRING);
   }
   
   bool BaseEntity::create(){
     if(!valid)
       return false;
     Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();
     
     Ogre::Entity* entity = mgr->createEntity(name_, mesh_);
     
     Ogre::MovableObject *ent = (Ogre::MovableObject *)entity;
     getNode()->attachObject(ent); // big problem here: sigsegv
     return true;
   }
}
