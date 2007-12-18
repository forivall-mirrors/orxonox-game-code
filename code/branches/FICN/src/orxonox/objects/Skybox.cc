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

#include <OgreSceneManager.h>
#include <string>

#include "../Orxonox.h"
#include "../../tinyxml/tinyxml.h"
#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"
#include "../core/Debug.h"

#include "Skybox.h"

namespace orxonox
{
    CreateFactory(Skybox);

    Skybox::Skybox()
    {
        RegisterObject(Skybox);
    }

    Skybox::~Skybox()
    {
    }

    void Skybox::loadParams(TiXmlElement* xmlElem)
    {
    	Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();

    	if (xmlElem->Attribute("src"))
    	{
    		std::string skyboxSrc = xmlElem->Attribute("src");
	    	mgr->setSkyBox(true, skyboxSrc);

	    	COUT(4) << "Loader: Set skybox: "<< skyboxSrc << std::endl << std::endl;
    	}
   }
}
