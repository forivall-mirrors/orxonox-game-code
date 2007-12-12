#include <OgreSceneManager.h>
#include <string>

#include "../orxonox.h"
#include "../../tinyxml/tinyxml.h"
#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"

#include "Ambient.h"

namespace orxonox
{
    CreateFactory(Ambient);

    Ambient::Ambient()
    {
        RegisterObject(Ambient);
    }

    Ambient::~Ambient()
    {
    }

    void Ambient::loadParams(TiXmlElement* xmlElem)
    {
    	Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();
    	
    	if (xmlElem->Attribute("colourvalue"))
    	{
	    	
	    	std::vector<std::string> colourvalues = tokenize(xmlElem->Attribute("colourvalue"),",");
	    	float r, g, b;
    	 	String2Number<float>(r, colourvalues[0]);
	    	String2Number<float>(g, colourvalues[1]);
	    	String2Number<float>(b, colourvalues[2]);
	    	 
	    	mgr->setAmbientLight(ColourValue(r,g,b));
	    	
	    	std::cout << "Loader: Set ambient light: "<<r<<" " << g << " " << b  << std::endl << std::endl;
    	}   	
   }
}
