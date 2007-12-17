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

        float sx = 1, sy = 1, sz = 1;
        if (xmlElem->Attribute("scale"))
        {
	    	  pos = tokenize(xmlElem->Attribute("scale"),",");
    	 	  String2Number<float>(sx, pos[0]);
	    	  String2Number<float>(sy, pos[1]);
	    	  String2Number<float>(sz, pos[2]);
        }

        float yaw = 0.0;
        if (xmlElem->Attribute("yaw"))
        {
          String2Number<float>(yaw,xmlElem->Attribute("yaw"));
        }
        float pitch = 0.0;
        if (xmlElem->Attribute("pitch"))
        {
          String2Number<float>(pitch,xmlElem->Attribute("pitch"));
        }
        float roll = 0.0;
        if (xmlElem->Attribute("roll"))
        {
          String2Number<float>(roll,xmlElem->Attribute("roll"));
        }

        Ogre::SceneNode *parentNode;
        if (xmlElem->Attribute("node"))
          parentNode = mgr->getSceneNode(xmlElem->Attribute("node"));
        else
          parentNode = mgr->getRootSceneNode();

        Ogre::SceneNode* node = parentNode->createChildSceneNode(name, Vector3(x,y,z));
        node->scale(sx,sy,sz);
        node->yaw(Ogre::Degree(yaw));
        node->pitch(Ogre::Degree(pitch));
        node->roll(Ogre::Degree(roll));

	    	COUT(4) << "Loader: Created node "<< name <<" : "<<x<<" " << y << " " << z  << std::endl << std::endl;
    	}
   }
   
   void SceneNode::registerAllVariables(){
     
   }
   
   bool SceneNode::create(){
     return true;
   }
   
}
