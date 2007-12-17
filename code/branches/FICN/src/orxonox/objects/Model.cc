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

    	if (xmlElem->Attribute("mesh"))
    	{
    		std::string src = xmlElem->Attribute("mesh");
    		this->mesh_.setMesh(src);
		    this->attachObject(this->mesh_.getEntity());
        }

        COUT(4) << "Loader: Created model" << std::endl;
    }
}
