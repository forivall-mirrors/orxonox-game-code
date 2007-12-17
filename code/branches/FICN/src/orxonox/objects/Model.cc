#include <string>

#include "Model.h"
#include "../core/CoreIncludes.h"
#include "../orxonox.h"
#include "../../tinyxml/tinyxml.h"
#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"

namespace orxonox
{
    CreateFactory(Model);

    Model::Model()
    {
        RegisterObject(Model);
    }

    Model::~Model()
    {
    }

    void Model::loadParams(TiXmlElement* xmlElem)
    {
        std::cout << "1\n";
    	if (xmlElem->Attribute("position"))
    	{
        std::cout << "2\n";
	    	std::vector<std::string> pos = tokenize(xmlElem->Attribute("position"),",");
	    	float x, y, z;
    	 	String2Number<float>(x, pos[0]);
	    	String2Number<float>(y, pos[1]);
	    	String2Number<float>(z, pos[2]);
	    	this->setPosition(x, y, z);
    	}

        std::cout << "3\n";
    	if (xmlElem->Attribute("direction"))
    	{
        std::cout << "4\n";
	    	std::vector<std::string> pos = tokenize(xmlElem->Attribute("direction"),",");
	    	float x, y, z;
    	 	String2Number<float>(x, pos[0]);
	    	String2Number<float>(y, pos[1]);
	    	String2Number<float>(z, pos[2]);
	    	this->setDirection(x, y, z);
    	}

        std::cout << "5\n";
    	if (xmlElem->Attribute("mesh"))
    	{
        std::cout << "6_1\n";
    		std::string src = xmlElem->Attribute("mesh");
        std::cout << "6_2\n";
        std::cout << this->mesh_.getEntity() << std::endl;
    		this->mesh_ = Mesh(src);
        std::cout << "6_3\n";
        std::cout << this->mesh_.getEntity() << std::endl;
		    this->attachObject(this->mesh_.getEntity());
        std::cout << "6_4\n";
    	}

        std::cout << "7\n";
    	if (xmlElem->Attribute("scale"))
    	{
        std::cout << "8\n";
		    std::string scaleStr = xmlElem->Attribute("scale");
		    float scale;
		    String2Number<float>(scale, scaleStr);
		    this->setScale(scale);
    	}
        std::cout << "9\n";

        COUT(4) << "Loader: Created model" << std::endl;
    }
}
