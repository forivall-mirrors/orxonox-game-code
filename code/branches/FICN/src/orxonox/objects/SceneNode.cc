#include <OgreSceneManager.h>
#include <string>

#include "../Orxonox.h"
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
    	if (xmlElem->Attribute("name") && xmlElem->Attribute("pos"))
    	{
    		name_ = xmlElem->Attribute("name");

	    	std::vector<std::string> pos = tokenize(xmlElem->Attribute("pos"),",");
	    	//float x, y, z_;
    	 	String2Number<float>(x_, pos[0]);
	    	String2Number<float>(y_, pos[1]);
	    	String2Number<float>(z_, pos[2]);

        sx_ = 1; sy_ = 1; sz_ = 1;
        if (xmlElem->Attribute("scale"))
        {
	    	  pos = tokenize(xmlElem->Attribute("scale"),",");
    	 	  String2Number<float>(sx_, pos[0]);
	    	  String2Number<float>(sy_, pos[1]);
	    	  String2Number<float>(sz_, pos[2]);
        }
        yaw_ = 0.0;
        if (xmlElem->Attribute("yaw"))
        {
          String2Number<float>(yaw_,xmlElem->Attribute("yaw"));
        }
        pitch_ = 0.0;
        if (xmlElem->Attribute("pitch"))
        {
          String2Number<float>(pitch_,xmlElem->Attribute("pitch"));
        }
        roll_ = 0.0;
        if (xmlElem->Attribute("roll"))
        {
          String2Number<float>(roll_,xmlElem->Attribute("roll"));
        }

        if(xmlElem->Attribute("node"))
          nodeName_ = xmlElem->Attribute("node");
        else
          nodeName_ = "";

        create();

        COUT(4) << "Loader: loaded node "<< name_ <<" : "<< x_ <<" " << y_ << " " << z_ << std::endl << std::endl;
    	}
   }

   void SceneNode::registerAllVariables(){
     registerVar(&x_, sizeof(float), network::DATA);
     registerVar(&y_, sizeof(float), network::DATA);
     registerVar(&z_, sizeof(float), network::DATA);
     registerVar(&sx_, sizeof(float), network::DATA);
     registerVar(&sy_, sizeof(float), network::DATA);
     registerVar(&sz_, sizeof(float), network::DATA);
     registerVar(&yaw_, sizeof(float), network::DATA);
     registerVar(&pitch_, sizeof(float), network::DATA);
     registerVar(&roll_, sizeof(float), network::DATA);
     registerVar(&name_, sizeof(float), network::STRING);
     registerVar(&nodeName_, sizeof(float), network::STRING);
   }

   bool SceneNode::create(){
     Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();

     Ogre::SceneNode *parentNode;
     if (nodeName_.compare("")!=0)
       parentNode = mgr->getSceneNode(nodeName_);
     else
       parentNode = mgr->getRootSceneNode();

     Ogre::SceneNode* node = parentNode->createChildSceneNode(name_, Vector3(x_,y_,z_));
     node->scale(sx_,sy_,sz_);
     node->yaw(Ogre::Degree(yaw_));
     node->pitch(Ogre::Degree(pitch_));
     node->roll(Ogre::Degree(roll_));
     return true;
   }

}
