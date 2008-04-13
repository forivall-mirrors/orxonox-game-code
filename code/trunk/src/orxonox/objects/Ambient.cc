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

#include "OrxonoxStableHeaders.h"
#include "Ambient.h"

#include <vector>
#include <string>

#include <OgreSceneManager.h>

#include "util/tinyxml/tinyxml.h"
#include "util/Tokenizer.h"
#include "util/String2Number.h"
#include "util/Math.h"
#include "core/Debug.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"
#include "core/XMLPort.h"

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
    	if (xmlElem->Attribute("colourvalue"))
    	{

	    	std::vector<std::string> colourvalues = tokenize(xmlElem->Attribute("colourvalue"),",");
	    	float r, g, b;
    	 	String2Number<float>(r, colourvalues[0]);
	    	String2Number<float>(g, colourvalues[1]);
	    	String2Number<float>(b, colourvalues[2]);

	    	this->setAmbientLight(ColourValue(r, g, b));

	    	COUT(4) << "Loader: Set ambient light: "<<r<<" " << g << " " << b  << std::endl << std::endl;
    	}
   }

   void Ambient::setAmbientLight(const ColourValue& colour)
   {
    	GraphicsEngine::getSingleton().getSceneManager()->setAmbientLight(colour);
   }

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void Ambient::XMLPort(Element& xmlelement, bool loading)
    {
        BaseObject::XMLPort(xmlelement, loading);

        XMLPortParamLoadOnly(Ambient, "colourvalue", setAmbientLight, xmlelement, loading);
    }
}
