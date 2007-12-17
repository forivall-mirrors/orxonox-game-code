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
#include <sstream>

#include "WorldEntity.h"
#include "../core/CoreIncludes.h"
#include "../orxonox.h"
#include "../../tinyxml/tinyxml.h"
#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"

namespace orxonox
{
    CreateFactory(WorldEntity);

    unsigned int WorldEntity::worldEntityCounter_s = 0;

    WorldEntity::WorldEntity()
    {
        RegisterObject(WorldEntity);

        std::cout << "10_1\n";
        if (Orxonox::getSingleton()->getSceneManager())
        {
        std::cout << "10_2\n";
            std::ostringstream name;
            name << (WorldEntity::worldEntityCounter_s++);
            this->setName("WorldEntity" + name.str());
            //node_ = Orxonox::getSingleton()->getSceneManager()->getRootSceneNode()->createChildSceneNode(this->getName());
            std::cout << "blubbbi: " << this->getName() << " .. " << this->node_ << std::endl;
        }
        std::cout << "10_3\n";

        this->bStatic_ = true;
        this->velocity_ = Vector3(0, 0, 0);
        this->acceleration_ = Vector3(0, 0, 0);
        this->rotationAxis_ = Vector3(0, 1, 0);
        this->rotationRate_ = 0;
        this->momentum_ = 0;
    }

    WorldEntity::~WorldEntity()
    {
    }

    void WorldEntity::tick(float dt)
    {
        if (!this->bStatic_)
        {
            this->velocity_ += (dt * this->acceleration_);
            this->translate(dt * this->velocity_);

            this->rotationRate_ += (dt * this->momentum_);
            this->rotate(this->rotationAxis_, dt * this->rotationRate_);
        }
    }

    void WorldEntity::loadParams(TiXmlElement* xmlElem)
    {
        BaseObject::loadParams(xmlElem);

        std::cout << "1\n";
    	if (xmlElem->Attribute("name"))
    	{
        std::cout << "2\n";
    		this->setName(xmlElem->Attribute("mesh"));
    	}
        std::cout << "3\n";
    	if (xmlElem->Attribute("position"))
    	{
        std::cout << "4\n";
	    	std::vector<std::string> pos = tokenize(xmlElem->Attribute("position"),",");
	    	float x, y, z;
    	 	String2Number<float>(x, pos[0]);
	    	String2Number<float>(y, pos[1]);
	    	String2Number<float>(z, pos[2]);
	    	this->setPosition(x, y, z);
    	}
        std::cout << "5\n";
    	if (xmlElem->Attribute("direction"))
    	{
        std::cout << "6\n";
	    	std::vector<std::string> pos = tokenize(xmlElem->Attribute("direction"),",");
	    	float x, y, z;
    	 	String2Number<float>(x, pos[0]);
	    	String2Number<float>(y, pos[1]);
	    	String2Number<float>(z, pos[2]);
	    	this->setDirection(x, y, z);
    	}
        std::cout << "7\n";
    	if (xmlElem->Attribute("scale"))
    	{
        std::cout << "8\n";
		    std::string scaleStr = xmlElem->Attribute("scale");
		    float scale;
		    String2Number<float>(scale, scaleStr);
		    this->setScale(scale);
    	}
        std::cout << "9\n";
    }

    void WorldEntity::registerAllVariables()
    {
      // to be implemented !
    }
}
