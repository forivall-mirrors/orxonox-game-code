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

#include "tinyxml/tinyxml.h"
#include "util/SubString.h"
#include "core/CoreIncludes.h"
#include "GraphicsEngine.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(WorldEntity);

    unsigned int WorldEntity::worldEntityCounter_s = 0;

    WorldEntity::WorldEntity() :
      velocity_    (0, 0, 0),
      acceleration_(0, 0, 0),
      rotationAxis_(0, 1, 0),
      rotationRate_(0),
      momentum_    (0),
      node_        (0),
      bStatic_     (true)
    {
        RegisterObject(WorldEntity);

        if (GraphicsEngine::getSingleton().getSceneManager())
        {
          std::ostringstream name;
          name << (WorldEntity::worldEntityCounter_s++);
          this->setName("WorldEntity" + name.str());
          this->node_ = GraphicsEngine::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode(this->getName());

          registerAllVariables();
        }
    }
    

    WorldEntity::~WorldEntity()
    {
      // just to make sure we clean out all scene nodes
      if(this->getNode())
        this->getNode()->removeAndDestroyAllChildren();
    }

    void WorldEntity::tick(float dt)
    {
        if (!this->bStatic_)
        {
//             COUT(4) << "acceleration: " << this->acceleration_ << " velocity: " << this->velocity_ << std::endl;
            this->velocity_ += (dt * this->acceleration_);
            this->translate(dt * this->velocity_, Ogre::Node::TS_LOCAL);

            this->rotationRate_ += (dt * this->momentum_);
            this->rotate(this->rotationAxis_, dt * this->rotationRate_);
            //COUT(3) << "rotationrate: " << this->rotationRate_.valueDegrees() << " momentum: " << this->momentum_.valueDegrees() << std::endl;
        }
    }

    void WorldEntity::loadParams(TiXmlElement* xmlElem)
    {

        BaseObject::loadParams(xmlElem);
        create();
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
        
        WorldEntity::create();
    }


    void WorldEntity::registerAllVariables()
    {
      // register coordinates
      registerVar( (void*) &(this->getPosition().x), sizeof(this->getPosition().x), network::DATA, 0x3);
      registerVar( (void*) &(this->getPosition().y), sizeof(this->getPosition().y), network::DATA, 0x3);
      registerVar( (void*) &(this->getPosition().z), sizeof(this->getPosition().z), network::DATA, 0x3);
      // register orientation
      registerVar( (void*) &(this->getOrientation().w), sizeof(this->getOrientation().w), network::DATA, 0x3);
      registerVar( (void*) &(this->getOrientation().x), sizeof(this->getOrientation().x), network::DATA, 0x3);
      registerVar( (void*) &(this->getOrientation().y), sizeof(this->getOrientation().y), network::DATA, 0x3);
      registerVar( (void*) &(this->getOrientation().z), sizeof(this->getOrientation().z), network::DATA, 0x3);
      // register velocity_
      registerVar( (void*) &(this->getVelocity().x), sizeof(this->getVelocity().x), network::DATA, 0x3);
      registerVar( (void*) &(this->getVelocity().y), sizeof(this->getVelocity().y), network::DATA, 0x3);
      registerVar( (void*) &(this->getVelocity().z), sizeof(this->getVelocity().z), network::DATA, 0x3);
      // register rotationAxis/rate
      registerVar( (void*) &(this->getRotationRate()), sizeof(this->getRotationRate()), network::DATA, 0x3);
      registerVar( (void*) &(this->getRotationAxis().x), sizeof(this->getRotationAxis().x), network::DATA, 0x3);
      registerVar( (void*) &(this->getRotationAxis().y), sizeof(this->getRotationAxis().y), network::DATA, 0x3);
      registerVar( (void*) &(this->getRotationAxis().z), sizeof(this->getRotationAxis().z), network::DATA, 0x3);
      // register scale of node
      registerVar( (void*) &(this->getScale().x), sizeof(this->getScale().x), network::DATA, 0x3);
      registerVar( (void*) &(this->getScale().y), sizeof(this->getScale().y), network::DATA, 0x3);
      registerVar( (void*) &(this->getScale().z), sizeof(this->getScale().z), network::DATA, 0x3);
      //register staticity
      registerVar( (void*) &(this->bStatic_), sizeof(this->bStatic_), network::DATA, 0x3);
      //register acceleration & momentum
//       registerVar( (void*) &(this->getAcceleration().x), sizeof(this->getAcceleration().x), network::DATA, 0x2);
//       registerVar( (void*) &(this->getAcceleration().y), sizeof(this->getAcceleration().y), network::DATA, 0x2);
//       registerVar( (void*) &(this->getAcceleration().z), sizeof(this->getAcceleration().z), network::DATA, 0x2);
//       registerVar( (void*) &(this->getMomentum()), sizeof(this->getMomentum()), network::DATA, 0x2); // only backsync
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