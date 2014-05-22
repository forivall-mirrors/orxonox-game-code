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
 *      Noe Pedrazzini
 *
 */

#include "ModularSpaceShip.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/Template.h"
#include "core/XMLPort.h"
#include "util/Math.h"
#include "gametypes/Gametype.h"
#include "core/command/ConsoleCommand.h"

#include "items/ShipPart.h"
#include "items/Engine.h"
#include "worldentities/StaticEntity.h"
#include "collisionshapes/WorldEntityCollisionShape.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>



namespace orxonox
{
    SetConsoleCommand("ModularSpaceShip", "killshippart", &ModularSpaceShip::killShipPartStatic);

    RegisterClass(ModularSpaceShip);

    std::map<StaticEntity*, ShipPart*>* ModularSpaceShip::partMap_s = 0;

    ModularSpaceShip::ModularSpaceShip(Context* context) : SpaceShip(context)
    {
        RegisterObject(ModularSpaceShip);

        this->registerVariables();

        ModularSpaceShip::partMap_s = &(this->partMap_);

    }

    ModularSpaceShip::~ModularSpaceShip()
    {
        if (this->isInitialized())
        {

        }
    }

    void ModularSpaceShip::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ModularSpaceShip, XMLPort, xmlelement, mode);
        XMLPortObject(ModularSpaceShip, ShipPart, "parts", addShipPart, getShipPart, xmlelement, mode);
    }

    void ModularSpaceShip::registerVariables()
    {
        return;
    }

    /**
    @brief
        Searches for ShipParts matching to StaticEntities.
    */
    void ModularSpaceShip::updatePartAssignment()
    {
        // iterate through all attached objects
        for (unsigned int i=0; i < (unsigned int)(this->getNumAttachedObj()); i++)
        {
            if (this->getAttachedObject(i) == NULL)
            {
                break;
            }
            // iterate through all attached parts
            for(unsigned int j = 0; j < this->partList_.size(); j++)
            {
                // if the name of the part matches the name of the object, add the object to that parts entitylist (unless it was already done).
                if((this->partList_[j]->getName() == this->getAttachedObject(i)->getName()) && !this->partList_[j]->hasEntity(orxonox_cast<StaticEntity*>(this->getAttachedObject(i))))
                {
                    // The Entity is added to the part's entityList_
                    this->partList_[j]->addEntity(orxonox_cast<StaticEntity*>(this->getAttachedObject(i)));
                    // An entry in the partMap_ is created, assigning the part to the entity.
                    this->addPartEntityAssignment((StaticEntity*)(this->getAttachedObject(i)), this->partList_[j]);
                }
            }
        }
    }

    void ModularSpaceShip::attach(WorldEntity* object)
    {
        SpaceShip::attach(object);
        this->updatePartAssignment();
    }

    /**
    @brief
        Creates a new assignment for the given StaticEntity and ShipPart in the partMap_
    @param entity
        A pointer to the StaticEntity
    @param part
        A pointer to the ShipPart.
    */
    void ModularSpaceShip::addPartEntityAssignment(StaticEntity* entity, ShipPart* part)
    {
        if (!entity || !part)
            return;

        if (this->partMap_.find(entity) != this->partMap_.end())
                {
                    orxout(internal_warning) << "Assigning an Entity to multiple parts is not yet supported." << endl;
                    return;
                }

        this->partMap_[entity] = part;
    }


    /**
    @brief
        Get the ShipPart an attached entity belongs to.
    @param entity
        The entity to be searched.
    @return
        Returns a pointer to the ShipPart the entity belongs to.
    */
    ShipPart* ModularSpaceShip::getPartOfEntity(StaticEntity* entity) const
    {
        for (std::map<StaticEntity*, ShipPart*>::const_iterator it = this->partMap_.begin(); it != this->partMap_.end(); ++it)
        {
            if (it->first == entity)
                return it->second;
        }
        return NULL;
    }

    /**
    @brief
        If the damage occurred on an attached StaticEntity, the damage is given to the corresponding ShipPart to handle.
    */
    void ModularSpaceShip::damage(float damage, float healthdamage, float shielddamage, Pawn* originator, const btCollisionShape* cs)
    {
        if (this->getPartOfEntity((StaticEntity*)(cs->getUserPointer())) != NULL)
            this->getPartOfEntity((StaticEntity*)(cs->getUserPointer()))->handleHit(damage, healthdamage, shielddamage, originator);
        else
            SpaceShip::damage(damage, healthdamage, shielddamage, originator, cs);
    }

    /**
    @brief
        STATIC: Needed for consolecommand. Kills the ShipPart with the given name. Used from the console-command "ModularSpaceShip killshippart [string]".
    @param name
        The name of the part to be killed.
    */
    void ModularSpaceShip::killShipPartStatic(std::string name)
    {
        for (std::map<StaticEntity*, ShipPart*>::const_iterator it = ModularSpaceShip::partMap_s->begin(); it != ModularSpaceShip::partMap_s->end(); ++it)
        {
            if (it->second->getName() == name)
            {
                it->second->setAlive(false);
                return;
            }
        }
        orxout(internal_warning) << "Could not apply damage to ShipPart \"" << name << "\". Part not found." << endl;
    }

    /**
    @brief
        Kills the ShipPart with the given name. Used from the console-command "ModularSpaceShip killshippart [string]".
    @param name
        The name of the part to be killed.
    */
    void ModularSpaceShip::killShipPart(std::string name)
    {
        for (std::map<StaticEntity*, ShipPart*>::const_iterator it = ModularSpaceShip::partMap_.begin(); it != ModularSpaceShip::partMap_.end(); ++it)
        {
            if (it->second->getName() == name)
            {
                it->second->setAlive(false);
                return;
            }
        }
        orxout(internal_warning) << "Could not apply damage to ShipPart \"" << name << "\". Part not found." << endl;
    }

    /**
    @brief
        Add a ShipPart to the SpaceShip.
    @param engine
        A pointer to the ShipPart to be added.
    */
    void ModularSpaceShip::addShipPart(ShipPart* part)
    {
        OrxAssert(part != NULL, "The ShipPart cannot be NULL.");
        this->partList_.push_back(part);
        part->setParent(this);
        this->updatePartAssignment();
    }

    /**
    @brief
        Get the i-th ShipPart of the SpaceShip.
    @return
        Returns a pointer to the i-the ShipPart. NULL if there is no ShipPart with that index.
    */
    ShipPart* ModularSpaceShip::getShipPart(unsigned int index)
    {
        if(this->partList_.size() <= index)
            return NULL;
        else
            return this->partList_[index];
    }

    /**
    @brief
        Looks for an attached ShipPart with a certain name.
    @param name
        The name of the ShipPart to be returned.
    @return
        Pointer to the ShipPart with the given name, or NULL if not found.
    */
    ShipPart* ModularSpaceShip::getShipPartByName(std::string name)
    {
        for(std::vector<ShipPart*>::iterator it = this->partList_.begin(); it != this->partList_.end(); ++it)
        {
            if(orxonox_cast<ShipPart*>(*it)->getName() == name)
            {
                return orxonox_cast<ShipPart*>(*it);
            }
        }
        orxout(internal_warning) << "Couldn't find ShipPart with name \"" << name << "\"." << endl;
        return NULL;
    }

    /**
    @brief
        Check whether the SpaceShip has a particular Engine.
    @param engine
        A pointer to the Engine to be checked.
    */
    bool ModularSpaceShip::hasShipPart(ShipPart* part) const
    {
        for(unsigned int i = 0; i < this->partList_.size(); i++)
        {
            if(this->partList_[i] == part)
                return true;
        }
        return false;
    }


    /**
    @brief
        Removes a ShipPart from the SpaceShip, destroying the corresponding StaticEntity
    @param part
        The ShipPart to be removed.
    */
    void ModularSpaceShip::removeShipPart(ShipPart* part)
    {
        // Remove the part from the partList_
        std::vector<ShipPart*>::iterator it = this->partList_.begin();
        for(unsigned int i = 0; i < this->partList_.size(); i++)
        {
            if(this->partList_[i] == part)
            {
                this->partList_.erase(it);
                break;
            }
            it++;
        }
        // Remove the part-entity assignment and detach the Entity of this ShipPart
        for (std::map<StaticEntity*, ShipPart*>::iterator itt = this->partMap_.begin(); itt != this->partMap_.end(); )
        {
            if (itt->second == part)
            {
                this->detach(itt->first);
                itt->first->destroy();
                //itt->first->setActive(false);
                //itt->first->setVisible(false);
                //itt->first->setCollisionResponse(false);
                //itt->first->setCollisionType(None);
                //itt->first->deactivatePhysics();
                this->partMap_.erase(itt++);
            } else {
                ++itt;
            }
        }
    }

    /**
    @brief
        Looks for an attached Engine with a certain name.
    @param name
        The name of the engine to be returned.
    @return
        Pointer to the engine with the given name, or NULL if not found.
    */
    Engine* ModularSpaceShip::getEngineByName(std::string name)
    {
        for(std::vector<Engine*>::iterator it = this->engineList_.begin(); it != this->engineList_.end(); ++it)
        {
            if(orxonox_cast<Engine*>(*it)->getName() == name)
            {
                return orxonox_cast<Engine*>(*it);
            }
        }
        orxout(internal_warning) << "Couldn't find Engine with name \"" << name << "\"." << endl;
        return NULL;
    }

    /**
    @brief
        Detaches a child WorldEntity from this instance.
    */
    void ModularSpaceShip::detach(WorldEntity* object)
    {
        std::set<WorldEntity*>::iterator it = this->children_.find(object);
        if (it == this->children_.end())
        {
            orxout(internal_warning) << "Cannot detach an object that is not a child." << endl;
            return;
        }

        // collision shapes

        //this->printBtChildShapes((btCompoundShape*)(this->getWorldEntityCollisionShape()->getCollisionShape()), 2, 0);
        this->detachCollisionShape(object->collisionShape_);  // after succeeding, causes a crash in the collision handling
        //this->printBtChildShapes((btCompoundShape*)(this->getWorldEntityCollisionShape()->getCollisionShape()), 2, 0);

        // mass
        if (object->getMass() > 0.0f)
        {
            this->childrenMass_ -= object->getMass();
            recalculateMassProps();
        }

        this->detachNode(object->node_);
        this->children_.erase(it);        // this causes a crash when unloading the level. Or not?

        object->notifyDetached();
    }
}
