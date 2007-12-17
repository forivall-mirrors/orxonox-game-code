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
        WorldEntity::loadParams(xmlElem);

        std::cout << "10\n";
    	if (xmlElem->Attribute("mesh"))
    	{
        std::cout << "11_1\n";
    		std::string src = xmlElem->Attribute("mesh");
        std::cout << "11_2\n";
        std::cout << this->mesh_.getEntity() << std::endl;
    		this->mesh_ = Mesh(src);
        std::cout << "11_3\n";
        std::cout << this->mesh_.getEntity() << std::endl;
		    this->attachObject(this->mesh_.getEntity());
        std::cout << "11_4\n";
    	}
        std::cout << "12\n";

        COUT(4) << "Loader: Created model" << std::endl;
    }
}
