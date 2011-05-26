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
 *      Maurus Kaufmann
 *   Co-authors:
 *      ...
 *
 */

#include "SpaceBoundaries.h"

#include "worldentities/MobileEntity.h"
#include "worldentities/ControllableEntity.h"
#include "core/ObjectListIterator.h"
#include "core/XMLPort.h"
#include "worldentities/pawns/Pawn.h"
#include "infos/PlayerInfo.h"
#include "interfaces/RadarViewable.h"
#include "graphics/Billboard.h"
#include <OgreBillboardSet.h>


namespace orxonox
{
    CreateFactory(SpaceBoundaries);

    SpaceBoundaries::SpaceBoundaries(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(SpaceBoundaries);

        this->setMaxDistance(3000);
        this->setWarnDistance(this->getMaxDistance());
        this->setShowDistance(this->getMaxDistance());
        this->setReaction(0);
    }
    SpaceBoundaries::~SpaceBoundaries()
    {
        if (this->isInitialized())
        {
            this->pawnsIn_.clear();
        
            for( std::vector<billboardAdministration>::iterator current = this->billboards_.begin(); current != this->billboards_.end(); current++)
            {
                if( current->billy != NULL)
                {
                    delete current->billy;
                }
            }
            this->billboards_.clear();
        }
    }
    
    void SpaceBoundaries::checkWhoIsIn()
    {
        pawnsIn_.clear();
        for(ObjectListIterator<Pawn> current = ObjectList<Pawn>::begin(); current != ObjectList<Pawn>::end(); ++current)
        {
            Pawn* currentPawn = *current;
            if( this->reaction_ == 0 )
            {
                float distance = this->computeDistance(currentPawn);
                if(distance <= this->maxDistance_)
                {
                    pawnsIn_.push_back(currentPawn);
                }
            } else if (this->reaction_ == 2) {
                float distance = this->computeDistance(currentPawn);
                if(distance >= this->maxDistance_)
                {
                    pawnsIn_.push_back(currentPawn);
                }
            } else {
                pawnsIn_.push_back(currentPawn);
            }
        }
    }
    
    void SpaceBoundaries::positionBillboard(const Vector3 position)
    {
        std::vector<billboardAdministration>::iterator current;
        for( current = this->billboards_.begin(); current != this->billboards_.end(); current++)
        {
            if(!current->usedYet)
            {
                break;
            }
        }
        if( current == this->billboards_.end() )
        {
            Billboard *tmp = new Billboard(this);
            tmp->setPosition(position);
            this->setBillboardOptions( tmp );
            Vector3 normalisedVec = (position - this->getPosition()).normalisedCopy(); /* Vektor von Kugelmitte nach aussen */
            tmp->setCommonDirection ( -1.0 * normalisedVec );
            tmp->setCommonUpVector( Vector3::UNIT_Z );
            billboardAdministration tmp2 = { true, tmp };
            this->billboards_.push_back( tmp2 );
        } else {
            current->billy->setPosition(position);
            current->billy->setVisible(true);
            current->usedYet = true;
            Vector3 normalisedVec = (position - this->getPosition()).normalisedCopy(); /* Vektor von Kugelmitte nach aussen */
            current->billy->setCommonDirection ( -1.0 * normalisedVec );
            current->billy->setCommonUpVector( Vector3::UNIT_Z );
        }
    }
    
    void SpaceBoundaries::setBillboardOptions(Billboard *billy)
    {
        if(billy != NULL)
        {
            billy->setMaterial("Grid");
            billy->setBillboardType(Ogre::BBT_PERPENDICULAR_COMMON);
            billy->setDefaultDimensions(150, 150);
            billy->setVisible(true);
        }
    }
    
    void SpaceBoundaries::removeAllBillboards()
    {
        for( std::vector<billboardAdministration>::iterator current = this->billboards_.begin(); current != this->billboards_.end(); current++ )
        {
            current->usedYet = false;
            current->billy->setVisible(false);
        }
    }
    
    void SpaceBoundaries::setMaxDistance(float r)
    {
        this->maxDistance_ = r;
    }
    float SpaceBoundaries::getMaxDistance()
    {
        return this->maxDistance_;
    }
    
    void SpaceBoundaries::setWarnDistance(float r)
    {
        this->warnDistance_ = r;
    }
    float SpaceBoundaries::getWarnDistance()
    {
        return this->warnDistance_;
    }
    
    void SpaceBoundaries::setShowDistance(float r)
    {
        this->showDistance_ = r;
    }
    float SpaceBoundaries::getShowDistance()
    {
        return this->showDistance_;
    }
    
    void SpaceBoundaries::setHealthDecrease(float amount)
    {
        this->healthDecrease_ = amount/1000;
    }
    float SpaceBoundaries::getHealthDecrease()
    {
        return this->healthDecrease_;
    }
    
    void SpaceBoundaries::setReaction(int mode)
    {
        this->reaction_ = mode;
    }
    int SpaceBoundaries::getReaction()
    {
        return this->reaction_;
    }

    void SpaceBoundaries::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceBoundaries, XMLPort, xmlelement, mode);

        XMLPortParam(SpaceBoundaries, "maxDistance", setMaxDistance, getMaxDistance, xmlelement, mode);
        XMLPortParam(SpaceBoundaries, "warnDistance", setWarnDistance, getWarnDistance, xmlelement, mode);
        XMLPortParam(SpaceBoundaries, "showDistance", setShowDistance, getShowDistance, xmlelement, mode);
        XMLPortParam(SpaceBoundaries, "healthDecrease", setHealthDecrease, getHealthDecrease, xmlelement, mode);
        XMLPortParam(SpaceBoundaries, "reactionMode", setReaction, getReaction, xmlelement, mode);
    }
    
    void SpaceBoundaries::tick(float dt)
    {
        this->checkWhoIsIn();
        this->removeAllBillboards();
        
        float distance;
        bool humanItem;
        for( std::list<WeakPtr<Pawn> >::iterator current = pawnsIn_.begin(); current != pawnsIn_.end(); current++ )
        {
            Pawn* currentPawn = current->get();
            if( currentPawn && currentPawn->getNode() ) 
            {
                distance = this->computeDistance(currentPawn);
                humanItem = this->isHumanPlayer(currentPawn);
                COUT(5) << "Distance:" << distance << std::endl; // message for debugging
                if(distance > this->warnDistance_ && distance < this->maxDistance_) // Display warning
                {
                    if(humanItem)
                    {
                        this->displayWarning("Attention! You are close to the boundary!");
                    }
                }
                if(/* humanItem &&*/ abs(this->maxDistance_ - distance) < this->showDistance_ )
                {
                    this->displayBoundaries(currentPawn); // Show the boundary
                }
                if(distance > this->maxDistance_ && (this->reaction_ == 1) )
                {
                    if( humanItem )
                    {
                        COUT(5) << "Health should be decreasing!" << std::endl;
                        this->displayWarning("You are out of the area now!");
                    }
                    currentPawn->removeHealth( (distance - this->maxDistance_) * this->healthDecrease_);
                }
                if( (this->reaction_ == 0) && (distance + 100 > this->maxDistance_)) // Exception: A Pawn can't move more than 100 units per tick.
                {
                    this->conditionalBounceBack(currentPawn, distance, dt);
                }
                if( this->reaction_ == 2 && (distance - 100 < this->maxDistance_) )
                {
                    this->conditionalBounceBack(currentPawn, distance, dt);
                }
            }
        }
    }
    
    float SpaceBoundaries::computeDistance(WorldEntity *item)
    {
        if(item != NULL)
        {
            Vector3 itemPosition = item->getPosition();
            return (itemPosition.distance(this->getPosition()));
        } else {
            return -1;
        }
    }
    
    void SpaceBoundaries::displayWarning(const std::string warnText)
    {   
        // TODO
    }
    
    void SpaceBoundaries::displayBoundaries(Pawn *item)
    {
        
        Vector3 direction = item->getPosition() - this->getPosition();
        direction.normalise();
        
        Vector3 boundaryPosition = this->getPosition() + direction * this->maxDistance_;
        
        this->positionBillboard(boundaryPosition);
    }
    
    void SpaceBoundaries::conditionalBounceBack(Pawn *item, float currentDistance, float dt)
    {
        Vector3 normal = item->getPosition() - this->getPosition();
        normal.normalise();
        Vector3 velocity = item->getVelocity();
        float normalSpeed = item->getVelocity().dotProduct(normal);
        
        /* Check, whether the Pawn would leave the boundary in the next tick, if so send it back. */
        if( this->reaction_ == 0 && currentDistance + normalSpeed * dt > this->maxDistance_ - 10 ) // -10: "security measure"
        {
            bounceBack(item, &normal, &velocity);
        } else if (this->reaction_ == 2 && currentDistance - normalSpeed * dt < this->maxDistance_ + 10 ) // 10: "security measure"
        {
            normal = normal * (-1);
            bounceBack(item, &normal, &velocity);
        }
    }
    
    void SpaceBoundaries::bounceBack(Pawn *item, Vector3 *normal, Vector3 *velocity)
    {
        float dampingFactor = 0.5;
        *velocity = velocity->reflect(*normal);
        Vector3 acceleration = item->getAcceleration();
        acceleration = acceleration.reflect(*normal);
        
        item->lookAt( *velocity + this->getPosition() );
        
        item->setAcceleration(acceleration * dampingFactor);
        item->setVelocity(*velocity * dampingFactor);
        
        item->setPosition( item->getPosition() - *normal * 10 ); // Set the position of the Pawn to be well inside the boundary.
    }
    
    bool SpaceBoundaries::isHumanPlayer(Pawn *item)
    {
        if(item != NULL)
        {
            if(item->getPlayer())
            {
                return item->getPlayer()->isHumanPlayer();
            }
        }
        return false;
    }
    
}
