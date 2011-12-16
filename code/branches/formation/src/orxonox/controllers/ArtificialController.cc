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
 *      Dominik Solenicki
 *      
 */

#include "ArtificialController.h"
#include "core/CoreIncludes.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/SpaceShip.h"

#include "weaponsystem/WeaponMode.h"
#include "weaponsystem/WeaponPack.h"
#include "weaponsystem/Weapon.h"
#include "weaponsystem/WeaponSlot.h"
#include "weaponsystem/WeaponSlot.h"


namespace orxonox
{

    ArtificialController::ArtificialController(BaseObject* creator) : FormationController(creator)
    {
        this->bSetupWorked = false;
        this->botlevel_ = 0.5f;
        this->timeout_ = 0;
        this->currentWaypoint_ = 0;
        this->setAccuracy(5);
        this->defaultWaypoint_ = NULL;
        this->mode_ = DEFAULT;//Vector-implementation: mode_.push_back(DEFAULT);
    }

    ArtificialController::~ArtificialController()
    {
        if (this->isInitialized())
        {//Vector-implementation: mode_.erase(mode_.begin(),mode_.end());
            this->waypoints_.clear();
            this->weaponModes_.clear();
        }
    }


    /**
        @brief Gets called when ControllableEntity is being changed. Resets the bot when it dies.
    */
    void ArtificialController::changedControllableEntity()
    {
        if (!this->getControllableEntity())
            this->removeFromFormation();
    }


    void ArtificialController::aimAtTarget()
    {
        if (!this->target_ || !this->getControllableEntity())
            return;

        static const float hardcoded_projectile_speed = 1250;

        this->targetPosition_ = getPredictedPosition(this->getControllableEntity()->getPosition(), hardcoded_projectile_speed, this->target_->getPosition(), this->target_->getVelocity());
        this->bHasTargetPosition_ = (this->targetPosition_ != Vector3::ZERO);

        Pawn* pawn = orxonox_cast<Pawn*>(this->getControllableEntity());
        if (pawn)
            pawn->setAimPosition(this->targetPosition_);
    }

    bool ArtificialController::isCloseAtTarget(float distance) const
    {
        if (!this->getControllableEntity())
            return false;

        if (!this->target_)
            return (this->getControllableEntity()->getPosition().squaredDistance(this->targetPosition_) < distance*distance);
        else
            return (this->getControllableEntity()->getPosition().squaredDistance(this->target_->getPosition()) < distance*distance);
    }

    bool ArtificialController::isLookingAtTarget(float angle) const
    {
        if (!this->getControllableEntity())
            return false;

        return (getAngle(this->getControllableEntity()->getPosition(), this->getControllableEntity()->getOrientation() * WorldEntity::FRONT, this->targetPosition_) < angle);
    }

    void ArtificialController::abandonTarget(Pawn* target)
    {
        if (target == this->target_)
            this->targetDied();
    }

//****************************************************************************************** NEW
    /**
        @brief DoFire is called when a bot should shoot and decides which weapon is used and whether the bot shoots at all.
    */
    void ArtificialController::doFire()
    {
        if(!this->bSetupWorked)//setup: find out which weapons are active ! hard coded: laser is "0", lens flare is "1", ...
        {
            this->setupWeapons();
        }
        else if(this->getControllableEntity() && weaponModes_.size()&&this->bShooting_ && this->isCloseAtTarget((1 + 2*botlevel_)*1000) && this->isLookingAtTarget(math::pi / 20.0f))
        {
            int firemode;
            float random = rnd(1);//
            if (this->isCloseAtTarget(130) && (firemode = getFiremode("LightningGun")) > -1 )
            {//LENSFLARE: short range weapon
                this->getControllableEntity()->fire(firemode); //ai uses lens flare if they're close enough to the target
            }
            else if( this->isCloseAtTarget(400) && (random < this->botlevel_) && (firemode = getFiremode("RocketFire")) > -1 )
            {//ROCKET: mid range weapon
                this->mode_ = ROCKET; //Vector-implementation: mode_.push_back(ROCKET);
                this->getControllableEntity()->fire(firemode); //launch rocket
                if(this->getControllableEntity() && this->target_) //after fire(3) is called, getControllableEntity() refers to the rocket!
                {
                    float speed = this->getControllableEntity()->getVelocity().length() - target_->getVelocity().length();
                    if(!speed) speed = 0.1f;
                    float distance = target_->getPosition().length() - this->getControllableEntity()->getPosition().length();
                    this->timeout_= distance/speed*sgn(speed*distance) + 1.8f; //predicted time of target hit (+ tolerance)
                }
                else
                    this->timeout_ = 4.0f; //TODO: find better default value
            }
            else if ((firemode = getFiremode("HsW01")) > -1 ) //LASER: default weapon
                this->getControllableEntity()->fire(firemode);
        }
    }

    /**
        @brief Information gathering: Which weapons are ready to use?
    */
    void ArtificialController::setupWeapons() //TODO: Make this function generic!! (at the moment is is based on conventions)
    {
        this->bSetupWorked = false;
        if(this->getControllableEntity())
        {
            Pawn* pawn = orxonox_cast<Pawn*>(this->getControllableEntity());
            if(pawn && pawn->isA(Class(SpaceShip))) //fix for First Person Mode: check for SpaceShip
            {
                this->weaponModes_.clear(); // reset previous weapon information
                WeaponSlot* wSlot = 0;
                for(int l=0; (wSlot = pawn->getWeaponSlot(l)) ; l++)
                {
                    WeaponMode* wMode = 0;
                    for(int i=0; (wMode = wSlot->getWeapon()->getWeaponmode(i)) ; i++)
                    {
                        std::string wName = wMode->getIdentifier()->getName();
                        if(this->getFiremode(wName) == -1) //only add a weapon, if it is "new"
                            weaponModes_[wName] = wMode->getMode();
                    }
                }
                if(weaponModes_.size())//at least one weapon detected
                    this->bSetupWorked = true;
            }//pawn->weaponSystem_->getMunition(SubclassIdentifier< Munition > *identifier)->getNumMunition (WeaponMode *user);
        }
    }


    void ArtificialController::setBotLevel(float level)
    {
        if (level < 0.0f)
            this->botlevel_ = 0.0f;
        else if (level > 1.0f)
            this->botlevel_ = 1.0f;
        else
            this->botlevel_ = level;
    }

    void ArtificialController::setAllBotLevel(float level)
    {
        for (ObjectList<ArtificialController>::iterator it = ObjectList<ArtificialController>::begin(); it != ObjectList<ArtificialController>::end(); ++it)
            it->setBotLevel(level);
    }

    void ArtificialController::setPreviousMode()
    {
        this->mode_ = DEFAULT; //Vector-implementation: mode_.pop_back();
    }

    /**
        @brief Manages boost. Switches between boost usage and boost safe mode.
    */
    void ArtificialController::boostControl()
    {
        SpaceShip* ship = orxonox_cast<SpaceShip*>(this->getControllableEntity());
        if(ship == NULL) return;
        if(ship->getBoostPower()*1.5f > ship->getInitialBoostPower() ) //upper limit ->boost
            this->getControllableEntity()->boost(true);
        else if(ship->getBoostPower()*4.0f < ship->getInitialBoostPower()) //lower limit ->do not boost
            this->getControllableEntity()->boost(false);
    }

    int ArtificialController::getFiremode(std::string name)
    {
        for (std::map< std::string, int >::iterator it = this->weaponModes_.begin(); it != this->weaponModes_.end(); ++it)
        {
            if (it->first == name)
                return it->second;
        }
        return -1;
    }

    void ArtificialController::addWaypoint(WorldEntity* waypoint)
    {
        this->waypoints_.push_back(waypoint);
    }

    WorldEntity* ArtificialController::getWaypoint(unsigned int index) const
    {
        if (index < this->waypoints_.size())
            return this->waypoints_[index];
        else
            return 0;
    }

    /**
        @brief Adds first waypoint of type name to the waypoint stack, which is within the searchDistance
        @param name object-name of a point of interest (e.g. "PickupSpawner", "ForceField")
    */
    void ArtificialController::updatePointsOfInterest(std::string name, float searchDistance)
    {
        WorldEntity* waypoint = NULL;
        for (ObjectList<WorldEntity>::iterator it = ObjectList<WorldEntity>::begin(); it != ObjectList<WorldEntity>::end(); ++it)
        {
            if((*it)->getIdentifier() == ClassByString(name))
            {
                ControllableEntity* controllable = this->getControllableEntity();
                if(!controllable) continue;
                float actualDistance = ( (*it)->getPosition() - controllable->getPosition() ).length();
                if(actualDistance > searchDistance || actualDistance < 5.0f) continue;
                    // TODO: PickupSpawner: adjust waypoint accuracy to PickupSpawner's triggerdistance
                    // TODO: ForceField: analyze is angle between forcefield boost and own flying direction is acceptable
                else
                {
                    waypoint = *it;
                    break;
                }
            }
        }
        if(waypoint)
            this->waypoints_.push_back(waypoint);
    }

    /**
        @brief Adds point of interest depending on context.  TODO: Further Possibilites: "ForceField", "PortalEndPoint", "MovableEntity", "Dock"
    */
    void ArtificialController::manageWaypoints()
    {
        if(!defaultWaypoint_)
            this->updatePointsOfInterest("PickupSpawner", 200.0f); // long search radius if there is no default goal
        else
            this->updatePointsOfInterest("PickupSpawner", 20.0f); // take pickup en passant if there is a default waypoint
    }
 
}
