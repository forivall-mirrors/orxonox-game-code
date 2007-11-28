#include <string.h>
#include <iostream>

#include "LevelLoader.h"
#include "xml/xmlParser.h"

using namespace std;

namespace loader
{

LevelLoader::LevelLoader(string file, string dir)
{
	// Load XML level file
	dir.append("/");
	dir.append(file);	
  	rootNode = XMLNode::openFileHelper(dir.c_str(),"WorldDataFile");
  	// TODO: Error handling

	// Assing general level infos to class variables
  	this->name_ = rootNode.getChildNode("name").getText();
  	this->description_ = rootNode.getChildNode("description").getText();
  	this->image_ = rootNode.getChildNode("image").getText();
  
  	this->loadingScreen();
  
  // Assign sub-nodes
  if (rootNode.nChildNode("LightManager")==1)
  {
  	// Init Luightmanager...
  }
  
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

void LevelLoader::loadingScreen()
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

