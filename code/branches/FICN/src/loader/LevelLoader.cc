#include <string.h>
#include <iostream>

#include "LevelLoader.h"
#include "tinyxml/tinyxml.h"
#include "orxonox/core/CoreIncludes.h"
#include "orxonox/core/Error.h"
#include "orxonox/objects/BaseObject.h"

using namespace std;

namespace loader
{

LevelLoader::LevelLoader(string file, string path)
{
	valid_ = false;
	
	// Load XML level file
	path.append("/");
	path.append(file);	
	
	// Open xml file
	doc.LoadFile(path);

	// Check if file was loaded
	if (doc.LoadFile())
	{
		TiXmlHandle hDoc(&doc);
		TiXmlHandle hRoot(0);		
		TiXmlElement* pElem;

		// Check for root element
		pElem = hDoc.FirstChildElement("orxonoxworld").Element();
		if (pElem)
		{
			// Set root element
			hRoot = TiXmlHandle(pElem);
			rootElement = hRoot.Element();

			// Set level description
			pElem = hRoot.FirstChild("description").Element();
			if (pElem)
			{
				description_ = pElem->GetText();	
			}
			
			// Set level name
			name_ = rootElement->Attribute("name");
			image_ = rootElement->Attribute("image");
			
			valid_ = true;
		}
		else
		{
			orxonox::Error("Level file has no valid root node");
		}	
	}
	else
	{
		orxonox::Error("Could not load level file ");
	}	
}

	void LevelLoader::loadLevel()
	{
		if (valid_)
		{
			TiXmlElement* loadElem;
			TiXmlElement* worldElem;
			TiXmlElement* tElem;
			TiXmlNode* tNode;
			
			
			// Set loading screen
			loadElem = rootElement->FirstChildElement("loading");
			if (loadElem)
			{
				// Set background
				tElem = loadElem->FirstChildElement("background");
				if (tElem)
				{
					loadingBackgroundColor_ = tElem->Attribute("color");
					loadingBackgroundImage_ = tElem->Attribute("image");
				}
				// Set bar
				tElem = loadElem->FirstChildElement("bar");
				if (tElem)
				{
					loadingBarImage_ = tElem->Attribute("image");;
					loadingBarTop_ = tElem->Attribute("top");
					loadingBarLeft_ = tElem->Attribute("left");
					loadingBarWidth_ = tElem->Attribute("width");
					loadingBarHeight_ = tElem->Attribute("height");
				}
				showLoadingScreen();
			}
			
			// Load audio
			// TODO
			
			// Load scripts
			// TODO
			
			// Load world
			worldElem = rootElement->FirstChildElement("world");
			if (worldElem)
			{	
				tNode = 0;
				while( tNode = worldElem->IterateChildren( tNode ) )
				{
					tElem = tNode->ToElement();
					orxonox::BaseObject* obj = ID(tElem->Value())->fabricate();
					obj->loadParams(tElem);
				}			
			}
			
			std::cout << "Loading finished!\n\n\n\n\n";						
		}
	}
	
	void LevelLoader::showLoadingScreen()
	{
		std::cout << "\n\n\nThis is Orxonox\nthe hottest 3D action shooter ever to exist\n\n\n";
		std::cout << "Level: " << name() << "\nDescription:" << description() << "\nImage:"<<image()<<"\n\n\n";
		std::cout << "Backgroundcolor: " << loadingBackgroundColor_ << "\nBackgroundimage:" << loadingBackgroundImage_ << "\n\n\n";
	}
	
	
	//orxonox::BaseObject* bla = orxonox::ID("classname")->fabricate();
	//bla->loadParams();
	
	
	/*
  	rootNode = XMLNode::openFileHelper(dir.c_str(),"orxonoxworld");
  	// TODO: Error handling

	// Assing general level infos to class variables
	
  	this->name_ = rootNode.getAttribute("name");
  	this->image_ = rootNode.getAttribute("image");
  	this->description_ = rootNode.getChildNode("description").getText();
  
  	loadingScreenNode = rootNode.getChildNode("loading");
  	if (!loadingScreenNode.isEmpty())
  	{
  		this->showLoadingScreen();
  	}

  	worldNode = rootNode.getChildNode("world");
  	if (!worldNode.isEmpty())
  	{
  		
  	}
  	*/

  
  /*
  // Assign sub-nodes
  if (rootNode.nChildNode("LightManager")==1)
  {
  	// Init Luightmanager...
  }*/
  
  /*
  
	worldNode = rootNode.getChildNode("WorldEntities");
	scriptNode = rootNode.getChildNode("ScriptManager");
	cameraNode = rootNode.getChildNode("CameraMan");
	lightNode = rootNode.getChildNode("LightManager");
*/



LevelLoader::~LevelLoader()
{


}


string LevelLoader::name()
{
	return this->name_;
}

string LevelLoader::description()
{
	return this->description_;
}

string LevelLoader::image()
{
	return this->image_;
}


/*


void LevelLoader::loadWorld(WorldManager* wm)
{
	if (!worldNode.getChildNode("lights").isEmpty())
	{
		
		
	}
}


void LevelLoader::loadLights(LightManager* lm)
{
	if (!lightNode.getChildNode("lights").isEmpty())
	{
		int nLights = lightNode.getChildNode("lights").nChildNode("light");
		for (int i=0; i<nLights;i++)
		{
			XMLNode t = lightNode.getChildNode("lights").getChildNode("light",i);
			const char* diffuse = t.getAttribute("diffuse-color");
			const char* coor = t.getAttribute("abs-coor");
			lm->addLight(diffuse,coor);	
		}
	}
	lm->setAmbient(lightNode.getChildNode("ambient").getAttribute("color"));	
}

void LevelLoader::loadCameras(CameraManager* cm)
{
	if (!cameraNode.getChildNode("cameras").isEmpty())
	{
		int nCameras = cameraNode.getChildNode("cameras").nChildNode("camera");
		for (int i=0; i<nCameras;i++)
		{
			XMLNode t = cameraNode.getChildNode("cameras").getChildNode("camera",i);
			
			
			cm->addCamera();
		}
	}
}


void LevelLoader::loadScripts(ScriptManager* sm)
{
	if (!scriptNode.getChildNode("scripts").isEmpty())
	{
		int nScripts = scriptNode.getChildNode("scripts").nChildNode("script");
		for (int i=0; i<nScripts;i++)
		{
			XMLNode t = scriptNode.getChildNode("scripts").getChildNode("script",i);
			sm->addScript(t.getAttribute("file"));
		}
	
	}
}
*/


}

