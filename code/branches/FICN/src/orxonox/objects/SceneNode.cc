#include <OgreSceneManager.h>
#include <string>

#include "../orxonox.h"
#include "../../tinyxml/tinyxml.h"
#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"
#include "../core/Debug.h"

#include "SceneNode.h"

namespace orxonox
{
    CreateFactory(SceneNode);

    SceneNode::SceneNode()
    {
        RegisterObject(SceneNode);
    }

    SceneNode::~SceneNode()
    {
    }

    void SceneNode::loadParams(TiXmlElement* xmlElem)
    {
    	Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();

    	if (xmlElem->Attribute("name") && xmlElem->Attribute("pos"))
    	{
    		std::string name = xmlElem->Attribute("name");

	    	std::vector<std::string> pos = tokenize(xmlElem->Attribute("pos"),",");
	    	float x, y, z;
    	 	String2Number<float>(x, pos[0]);
	    	String2Number<float>(y, pos[1]);
	    	String2Number<float>(z, pos[2]);


    		mgr->getRootSceneNode()->createChildSceneNode(name, Vector3(x,y,z));

	    	COUT(4) << "Loader: Created node "<< name <<" : "<<x<<" " << y << " " << z  << std::endl << std::endl;
    	}
   }
}
