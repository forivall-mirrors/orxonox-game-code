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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "WorldEntity.h"

#include <string>
#include <sstream>

#include "util/tinyxml/tinyxml.h"
#include "util/SubString.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(WorldEntity);

    unsigned int WorldEntity::worldEntityCounter_s = 0;

    WorldEntity::WorldEntity()
    {
        RegisterObject(WorldEntity);


        this->bStatic_ = true;
        this->velocity_ = Vector3(0, 0, 0);
        this->acceleration_ = Vector3(0, 0, 0);
        this->rotationAxis_ = Vector3(0, 1, 0);
        this->rotationRate_ = 0;
        this->momentum_ = 0;

        if (GraphicsEngine::getSingleton().getSceneManager())
        {
          std::ostringstream name;
          name << (WorldEntity::worldEntityCounter_s++);
          this->setName("WorldEntity" + name.str());
          this->node_ = GraphicsEngine::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode(this->getName());

          registerAllVariables();
        }
        else
        {
          this->node_ = 0;
        }
    }
    

    WorldEntity::~WorldEntity()
    {
    }

    void WorldEntity::tick(float dt)
    {
        if (!this->bStatic_)
        {
            this->velocity_ += (dt * this->acceleration_);
            this->translate(dt * this->velocity_, Ogre::Node::TS_LOCAL);

            this->rotationRate_ += (dt * this->momentum_);
            this->rotate(this->rotationAxis_, dt * this->rotationRate_);
        }
    }

    void WorldEntity::loadParams(TiXmlElement* xmlElem)
    {

        BaseObject::loadParams(xmlElem);
        create();
/*
        if (xmlElem->Attribute("position"))
        {
            std::vector<std::string> pos = tokenize(xmlElem->Attribute("position"),",");
            float x, y, z;
            String2Number<float>(x, pos[0]);
            String2Number<float>(y, pos[1]);
            String2Number<float>(z, pos[2]);
            this->setPosition(x, y, z);
        }

        if (xmlElem->Attribute("direction"))
        {
            std::vector<std::string> pos = tokenize(xmlElem->Attribute("direction"),",");
            float x, y, z;
            String2Number<float>(x, pos[0]);
            String2Number<float>(y, pos[1]);
            String2Number<float>(z, pos[2]);
            this->setDirection(x, y, z);
        }

        if (xmlElem->Attribute("yaw") || xmlElem->Attribute("pitch") || xmlElem->Attribute("roll"))
        {
            float yaw = 0.0, pitch = 0.0, roll = 0.0;
            if (xmlElem->Attribute("yaw"))
                String2Number<float>(yaw,xmlElem->Attribute("yaw"));

            if (xmlElem->Attribute("pitch"))
                String2Number<float>(pitch,xmlElem->Attribute("pitch"));

            if (xmlElem->Attribute("roll"))
                String2Number<float>(roll,xmlElem->Attribute("roll"));

            this->yaw(Degree(yaw));
            this->pitch(Degree(pitch));
            this->roll(Degree(roll));
        }

        if (xmlElem->Attribute("scale"))
        {
            std::string scaleStr = xmlElem->Attribute("scale");
            float scale;
            String2Number<float>(scale, scaleStr);
            this->setScale(scale);
        }

        if (xmlElem->Attribute("rotationAxis"))
        {
            std::vector<std::string> pos = tokenize(xmlElem->Attribute("rotationAxis"),",");
            float x, y, z;
            String2Number<float>(x, pos[0]);
            String2Number<float>(y, pos[1]);
            String2Number<float>(z, pos[2]);
            this->setRotationAxis(x, y, z);
        }

        if (xmlElem->Attribute("rotationRate"))
        {
            float rotationRate;
            String2Number<float>(rotationRate, xmlElem->Attribute("rotationRate"));
            this->setRotationRate(Degree(rotationRate));
            if (rotationRate != 0)
                this->setStatic(false);
        }

        create();
*/
    }
    

    void WorldEntity::setYawPitchRoll(const Degree& yaw, const Degree& pitch, const Degree& roll)
    {
        this->yaw(yaw);
        this->pitch(pitch);
        this->roll(roll);
    }

    /**
        @brief XML loading and saving.
        @param xmlelement The XML-element
        @param loading Loading (true) or saving (false)
        @return The XML-element
    */
    void WorldEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        BaseObject::XMLPort(xmlelement, mode);

        XMLPortParam(WorldEntity, "position", setPositionLoader2, getPosition, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "direction", setDirectionLoader, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "yawpitchroll", setYawPitchRoll, xmlelement, mode);
        XMLPortParam(WorldEntity, "scale", setTotalScale, getScale, xmlelement, mode);
        XMLPortParam(WorldEntity, "rotationAxis", setRotationAxisLoader, getRotationAxis, xmlelement, mode);
        XMLPortParam(WorldEntity, "rotationRate", setRotationRate, getRotationRate, xmlelement, mode);

        XMLPortObject(WorldEntity, WorldEntity, "attached", attachWorldEntity, getAttachedWorldEntity, xmlelement, mode, false, true);
        
        //create();
    }


    void WorldEntity::registerAllVariables()
    {
      // register coordinates
      registerVar( (void*) &(this->getPosition().x), sizeof(this->getPosition().x), network::DATA);
      registerVar( (void*) &(this->getPosition().y), sizeof(this->getPosition().y), network::DATA);
      registerVar( (void*) &(this->getPosition().z), sizeof(this->getPosition().z), network::DATA);
      // register orientation
      registerVar( (void*) &(this->getOrientation().w), sizeof(this->getOrientation().w), network::DATA);
      registerVar( (void*) &(this->getOrientation().x), sizeof(this->getOrientation().x), network::DATA);
      registerVar( (void*) &(this->getOrientation().y), sizeof(this->getOrientation().y), network::DATA);
      registerVar( (void*) &(this->getOrientation().z), sizeof(this->getOrientation().z), network::DATA);
      // register velocity_
      registerVar( (void*) &(this->getVelocity().x), sizeof(this->getVelocity().x), network::DATA);
      registerVar( (void*) &(this->getVelocity().y), sizeof(this->getVelocity().y), network::DATA);
      registerVar( (void*) &(this->getVelocity().z), sizeof(this->getVelocity().z), network::DATA);
      // register rotationAxis/rate
      registerVar( (void*) &(this->getRotationRate()), sizeof(this->getRotationRate()), network::DATA);
      registerVar( (void*) &(this->getRotationAxis().x), sizeof(this->getRotationAxis().x), network::DATA);
      registerVar( (void*) &(this->getRotationAxis().y), sizeof(this->getRotationAxis().y), network::DATA);
      registerVar( (void*) &(this->getRotationAxis().z), sizeof(this->getRotationAxis().z), network::DATA);
      // register scale of node
      registerVar( (void*) &(this->getScale().x), sizeof(this->getScale().x), network::DATA);
      registerVar( (void*) &(this->getScale().y), sizeof(this->getScale().y), network::DATA);
      registerVar( (void*) &(this->getScale().z), sizeof(this->getScale().z), network::DATA);
      //register staticity
      registerVar( (void*) &(this->bStatic_), sizeof(this->bStatic_), network::DATA);
    }

    void WorldEntity::attachWorldEntity(WorldEntity* entity)
    {
        this->attachedWorldEntities_.push_back(entity);
    }

    const WorldEntity* WorldEntity::getAttachedWorldEntity(unsigned int index) const
    {
        if (index < this->attachedWorldEntities_.size())
            return this->attachedWorldEntities_[index];
        else
            return 0;
    }
}
