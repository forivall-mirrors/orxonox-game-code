/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
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

#include "tinyxml/tinyxml.h"
#include "util/SubString.h"
#include "util/Convert.h"
#include "util/Math.h"
#include "core/Debug.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"
#include "core/XMLPort.h"
#include "core/ConsoleCommand.h"

namespace orxonox
{
    SetConsoleCommand(Ambient, setAmbientLightTest, false).setDefaultValues(ColourValue(1, 1, 1, 1)).setAccessLevel(AccessLevel::Offline);

    CreateFactory(Ambient);

    Ambient* Ambient::instance_s;

    Ambient::Ambient()
    {
        RegisterObject(Ambient);
        Ambient::instance_s = this;
        registerAllVariables();
    }

    Ambient::~Ambient()
    {
    }

    bool Ambient::create(){
      GraphicsEngine::getSingleton().getSceneManager()->setAmbientLight(ambientLight_);
      return Synchronisable::create();
    }
    
    void Ambient::registerAllVariables(){
      registerVar(&ambientLight_, sizeof(ColourValue), network::DATA);
      
    }
    
    void Ambient::loadParams(TiXmlElement* xmlElem)
    {
    	if (xmlElem->Attribute("colourvalue"))
    	{
        SubString colourvalues(xmlElem->Attribute("colourvalue"), ',');

	    	float r, g, b;
        convertValue<std::string, float>(&r, colourvalues[0]);
        convertValue<std::string, float>(&g, colourvalues[1]);
        convertValue<std::string, float>(&b, colourvalues[2]);

	    	this->setAmbientLight(ColourValue(r, g, b));

	    	COUT(4) << "Loader: Set ambient light: "<<r<<" " << g << " " << b  << std::endl << std::endl;
    	}
   }

   void Ambient::setAmbientLight(const ColourValue& colour)
   {
    	GraphicsEngine::getSingleton().getSceneManager()->setAmbientLight(colour);
      ambientLight_=colour;	
   }

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void Ambient::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        BaseObject::XMLPort(xmlelement, mode);

        XMLPortParamLoadOnly(Ambient, "colourvalue", setAmbientLight, xmlelement, mode);
        create();
    }
}
