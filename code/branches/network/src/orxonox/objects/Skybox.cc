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

#include <string>

#include <OgreSceneManager.h>

#include "util/tinyxml/tinyxml.h"
//#include "util/Tokenizer.h"
//#include "util/String2Number.h"
#include "../Orxonox.h"
#include "../core/CoreIncludes.h"
#include "../core/Debug.h"
#include "core/XMLPort.h"

#include "Skybox.h"

namespace orxonox
{
    CreateFactory(Skybox);

    Skybox::Skybox()
    {
        RegisterObject(Skybox);
        registerAllVariables();
    }

    Skybox::~Skybox()
    {
    }

    void Skybox::loadParams(TiXmlElement* xmlElem)
    {
    	if (xmlElem->Attribute("src"))
    	{
    		skyboxSrc_ = xmlElem->Attribute("src");
        this->create();

	    	COUT(4) << "Loader: Set skybox: "<< skyboxSrc_ << std::endl << std::endl;
    	}
   }

   void Skybox::setSkybox(const std::string& skyboxname)
   {
    	Orxonox::getSingleton()->getSceneManager()->setSkyBox(true, skyboxname);
   }

   void Skybox::setSkyboxSrc(std::string src){
     skyboxSrc_ = src;
   }
   
    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void Skybox::XMLPort(Element& xmlelement, bool loading)
    {
        BaseObject::XMLPort(xmlelement, loading);

        XMLPortParamLoadOnly(Skybox, "src", setSkyboxSrc, xmlelement, loading);
        create();
    }
    
    bool Skybox::create(){
      this->setSkybox(skyboxSrc_);
      return true;
    }
    
    void Skybox::registerAllVariables(){
      registerVar(&skyboxSrc_, skyboxSrc_.length()+1 ,network::STRING);
    }
    
}
