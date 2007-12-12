#include <OgreSceneManager.h>
#include <string>

#include "../orxonox.h"
#include "../../tinyxml/tinyxml.h"
#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"

#include "Skybox.h"

namespace orxonox
{
    CreateFactory(Skybox);

    Skybox::Skybox()
    {
        RegisterObject(Skybox);
    }

    Skybox::~Skybox()
    {
    }

    void Skybox::loadParams(TiXmlElement* xmlElem)
    {
    	Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();
    	
    	if (xmlElem->Attribute("src"))
    	{
    		std::string skyboxSrc = xmlElem->Attribute("src");
	    	mgr->setSkyBox(true, skyboxSrc);
	    	
	    	std::cout << "Loader: Set skybox: "<< skyboxSrc << std::endl << std::endl;
    	}   	
   }
}
