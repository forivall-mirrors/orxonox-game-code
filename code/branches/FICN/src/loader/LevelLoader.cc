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
 *     Nicolas Perrenoud <nicolape@ee.ethz.ch>
 *   Co-authors:
 *      ...
 *
 */

// #include <string>
// #include <vector>
// #include <iostream>
#include <algorithm>
#include <iterator>

#include <OgreOverlayManager.h>

#include "LevelLoader.h"
// #include "tinyxml/tinyxml.h"
#include "orxonox/core/CoreIncludes.h"
#include "orxonox/core/Error.h"
#include "orxonox/objects/BaseObject.h"
#include "audio/AudioManager.h"
#include "orxonox/Orxonox.h"

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
			TiXmlElement* audioElem;
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

				COUT(0) << "\n\n\nThis is Orxonox\nthe hottest 3D action shooter ever to exist\n\n\n";
				COUT(0) << "Level: " << name() << "\nDescription:" << description() << "\nImage:"<<image()<<"\n\n\n";
				COUT(4) << "Backgroundcolor: " << loadingBackgroundColor_ << "\nBackgroundimage:" << loadingBackgroundImage_ << "\n\n\n";

			}

			// Load audio
			audio::AudioManager* auMan = orxonox::Orxonox::getSingleton()->getAudioManagerPointer();
			audioElem = rootElement->FirstChildElement("audio");

			if (audioElem)
			{
				audioElem = audioElem->FirstChildElement("ambient");
				if (audioElem)
				{
					tNode = 0;
          //FIXME something is worng, probably ==
					while( tNode = audioElem->IterateChildren( tNode ) )
					{
            if (tNode->Type() == TiXmlNode::ELEMENT)
            {

						  tElem = tNode->ToElement();
						  std::string elemVal = tElem->Value();
						  if (elemVal == "ogg")
						  {
				COUT(0) << "Adding sound "<< tElem->Attribute("src") << "\n\n\n";

				    	  auMan->ambientAdd(tElem->Attribute("src"));
						  }
            }
					}
					auMan->ambientStart();
				}
			}

			// Load world
			worldElem = rootElement->FirstChildElement("world");
			if (worldElem)
			{
				tNode = 0;
				while (tNode = worldElem->IterateChildren(tNode))
				{
          if (tNode->Type() == TiXmlNode::ELEMENT)
          {
					  tElem = tNode->ToElement();
					  orxonox::Identifier* id = ID(tElem->Value());
					  if (id)
					  {
                          orxonox::BaseObject* obj = id->fabricate();
                          obj->loadParams(tElem);
					  }
					  else
					  {
					      COUT(2) << "Warning: '"<< tElem->Value() <<"' is not a valid classname.\n";
					  }
          }
				}
			}

			if (loadElem)
			{
				 mLoadOverlay->hide();
			}


			COUT(0) << "Loading finished!\n\n\n\n\n";
		}
	}

	LevelLoader::~LevelLoader()
	{

	}



}

