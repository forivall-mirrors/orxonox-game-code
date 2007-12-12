#include <string.h>
#include <iostream>

#include "LevelLoader.h"
//#include "xml/xmlParser.h"
#include "tinyxml/tinyxml.h"

using namespace std;

namespace loader
{

LevelLoader::LevelLoader(string file, string dir)
{
	// Load XML level file
	dir.append("/");
	dir.append(file);	
	
	TiXmlDocument doc(file);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{

	}
	else
	{
		std::string err = "Could not load level file ";
		err.append(file); 
		std::cout << err << std::endl;
	}	
	
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

}

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

void LevelLoader::showLoadingScreen()
{
	cout << "\n\n\nThis is Orxonox\nthe hottest 3D action shooter ever to exist\n\n\n";
	cout << "Level: " << name() << "\nDescription:" << description() << "\nImage:"<<image()<<"\n\n\n";
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

