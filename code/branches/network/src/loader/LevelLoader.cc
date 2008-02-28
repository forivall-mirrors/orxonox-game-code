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

#include "OrxonoxStableHeaders.h"

#include <OgreOverlay.h>
#include <OgreOverlayManager.h>

#include "core/Error.h"
#include "core/Debug.h"
#include "core/CoreIncludes.h"

//#include "audio/AudioManager.h"
#include "core/BaseObject.h"
#include "orxonox/Orxonox.h"

#include "LevelLoader.h"

namespace loader
{

  LevelLoader::LevelLoader(std::string file, std::string path)
  {
    valid_ = false;

    // Load XML level file
    path.append("/");
    path.append(file);

    // Open xml file
    doc_.LoadFile(path.c_str());

    // Check if file was loaded
    if (doc_.LoadFile())
    {
      TiXmlHandle hDoc(&doc_);
      TiXmlHandle hRoot(0);
      TiXmlElement* pElem;

      // Check for root element
      pElem = hDoc.FirstChildElement("orxonoxworld").Element();
      if (pElem)
      {
        // Set root element
        hRoot = TiXmlHandle(pElem);
        rootElement_ = hRoot.Element();

        // Set level description
        pElem = hRoot.FirstChild("description").Element();
        if (pElem)
        {
          description_ = pElem->GetText();
        }

        // Set level name
        name_ = rootElement_->Attribute("name");
        image_ = rootElement_->Attribute("image");

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
//      TiXmlElement* audioElem;
      TiXmlElement* worldElem;
      TiXmlElement* tElem;
      TiXmlNode* tNode;

      Ogre::OverlayManager& omgr = Ogre::OverlayManager::getSingleton();
      Ogre::Overlay* mLoadOverlay; // FIXME: may be uninitialized

      // Set loading screen
      loadElem = rootElement_->FirstChildElement("loading");
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

        COUT(0) << "This is Orxonox" << std::endl;
        COUT(0) << "the hottest 3D action shooter ever to exist" << std::endl;
        COUT(0) << "Level: " << name() << std::endl << "Description:" << description() << std::endl << "Image:" << image() << std::endl;
        COUT(4) << "Backgroundcolor: " << loadingBackgroundColor_ << std::endl << "Backgroundimage:" << loadingBackgroundImage_ << std::endl;

      }
/*
      // Load audio
      audio::AudioManager* auMan = orxonox::Orxonox::getSingleton()->getAudioManagerPointer();
      audioElem = rootElement_->FirstChildElement("audio");

      if (audioElem)
      {
        audioElem = audioElem->FirstChildElement("ambient");
        if (audioElem)
        {
          tNode = 0;
          //FIXME something is wrong, probably missing ==
          while( tNode = audioElem->IterateChildren( tNode ) )
          {
            if (tNode->Type() == TiXmlNode::ELEMENT)
            {

              tElem = tNode->ToElement();
              std::string elemVal = tElem->Value();
              if (elemVal == "ogg")
              {
                COUT(3) << "Adding sound "<< tElem->Attribute("src") << std::endl;

                auMan->ambientAdd(tElem->Attribute("src"));
              }
            }
          }
          auMan->ambientStart();
        }
      }
*/
      // Load world
      worldElem = rootElement_->FirstChildElement("world");
      if (worldElem)
      {
        tNode = 0;
        //FIXME something is wrong, probably missing ==
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
              COUT(2) << "Warning: '"<< tElem->Value() <<"' is not a valid classname." << std::endl;
            }
          }
        }
      }

      if (loadElem)
      {
        // FIXME: check for potential initialisation of mLoadOverlay
        mLoadOverlay->hide();
      }


      COUT(0) << "Loading finished!" << std::endl << std::endl;
    }
  }

  LevelLoader::~LevelLoader()
  {

  }

}
