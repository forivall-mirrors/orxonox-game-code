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

#include <vector>
#include <string>

#include <OgreSceneManager.h>

#include "tinyxml/tinyxml.h"
#include "misc/Tokenizer.h"
#include "misc/String2Number.h"
#include "misc/Math.h"
#include "../core/Debug.h"
#include "../core/CoreIncludes.h"
#include "../Orxonox.h"

#include "Ambient.h"

namespace orxonox
{
    CreateFactory(Ambient);

    Ambient::Ambient()
    {
        RegisterObject(Ambient);
    }

    Ambient::~Ambient()
    {
    }

    void Ambient::loadParams(TiXmlElement* xmlElem)
    {
    	Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();

    	if (xmlElem->Attribute("colourvalue"))
    	{

	    	std::vector<std::string> colourvalues = tokenize(xmlElem->Attribute("colourvalue"),",");
	    	float r, g, b;
    	 	String2Number<float>(r, colourvalues[0]);
	    	String2Number<float>(g, colourvalues[1]);
	    	String2Number<float>(b, colourvalues[2]);

	    	mgr->setAmbientLight(ColourValue(r,g,b));

	    	COUT(4) << "Loader: Set ambient light: "<<r<<" " << g << " " << b  << std::endl << std::endl;
    	}
   }
}
