#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

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


}

