#include <string>

#include "Model.h"
#include "../core/CoreIncludes.h"
#include "../Orxonox.h"
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
            meshSrc_ = xmlElem->Attribute("mesh");
        }
        create();
    }
    
    bool Model::create(){
      if(meshSrc_.compare("")!=0){
        this->mesh_.setMesh(meshSrc_);
        this->attachObject(this->mesh_.getEntity());
        COUT(4) << "Loader: Created model" << std::endl;
      }
      registerAllVariables();
      return true;
    }
    
    void Model::registerAllVariables(){
      registerVar(&meshSrc_, meshSrc_.length()+1, network::STRING);
    }
}
