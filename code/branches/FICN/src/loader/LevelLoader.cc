/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

#include <OgreOverlayManager.h>

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

			Ogre::OverlayManager& omgr = Ogre::OverlayManager::getSingleton();
 			Ogre::Overlay* mLoadOverlay;

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
				
				
		    mLoadOverlay = (Ogre::Overlay*)omgr.getByName("Orxonox/LoadingScreenSample");
		    mLoadOverlay->show(); 				

				std::cout << "\n\n\nThis is Orxonox\nthe hottest 3D action shooter ever to exist\n\n\n";
				std::cout << "Level: " << name() << "\nDescription:" << description() << "\nImage:"<<image()<<"\n\n\n";
				std::cout << "Backgroundcolor: " << loadingBackgroundColor_ << "\nBackgroundimage:" << loadingBackgroundImage_ << "\n\n\n";
							
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
			
			if (loadElem)
			{
				 mLoadOverlay->hide();
			}
			std::cout << "Loading finished!\n\n\n\n\n";
		}
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

