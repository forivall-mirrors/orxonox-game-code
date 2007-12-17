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
std::cout << "### START PARSING MODEL" << std::endl;
        WorldEntity::loadParams(xmlElem);


std::cout << "Model: 1\n";
    	if (xmlElem->Attribute("mesh"))
    	{
std::cout << "Model: 2_1\n";
    		std::string src = xmlElem->Attribute("mesh");
std::cout << "Model: 2_2\n";
std::cout << "Model: mesh before creation: " << &this->mesh_ << std::endl;
std::cout << "Model: entity before creation: " << this->mesh_.getEntity() << std::endl;
    		this->mesh_ = Mesh(src);
std::cout << "Model: 2_3\n";
std::cout << "Model: entity after creation: " << this->mesh_.getEntity() << std::endl;
std::cout << "Model: mesh after creation: " << &this->mesh_ << std::endl;
std::cout << "Model: node: " << this->getNode() << std::endl;

//Entity *ent2 = Orxonox::getSingleton()->getSceneManager()->createEntity( "Robot2", "ogrehead.mesh" );
//SceneNode *node2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode2", Vector3( 50, 0, 0 ) );
//node2->attachObject( ent2 );
//this->attachObject( ent2 );
//node2->attachObject( this->mesh_.getEntity() );
this->attachObject( this->mesh_.getEntity() );

//		    this->attachObject(this->mesh_.getEntity());
std::cout << "Model: 2_4\n";
        }
std::cout << "Model: 3\n";
std::cout << "### FINISHED PARSING MODEL" << std::endl;

        COUT(4) << "Loader: Created model" << std::endl;
    }
}
