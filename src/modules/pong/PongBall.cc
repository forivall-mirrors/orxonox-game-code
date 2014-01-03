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

/**
    @file PongBall.cc
    @brief Implementation of the PongBall class.
*/

#include "PongBall.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"

#include "gametypes/Gametype.h"

#include "PongBat.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(PongBall);

    const float PongBall::MAX_REL_Z_VELOCITY = 1.5;

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    PongBall::PongBall(Context* context)
        : MovableEntity(context)
    {
        RegisterObject(PongBall);

        this->speed_ = 0;
        this->accelerationFactor_ = 1.0f;
        this->bat_ = 0;
        this->bDeleteBats_ = false;
        this->batID_ = new unsigned int[2];
        this->batID_[0] = OBJECTID_UNKNOWN;
        this->batID_[1] = OBJECTID_UNKNOWN;
        this->relMercyOffset_ = 0.05f;

        this->registerVariables();

        //initialize sound
        if (GameMode::isMaster())
             {
                 this->defScoreSound_ = new WorldSound(this->getContext());
                 this->defScoreSound_->setVolume(1.0f);
                 this->defBatSound_ = new WorldSound(this->getContext());
                 this->defBatSound_->setVolume(0.4f);
                 this->defBoundarySound_ = new WorldSound(this->getContext());
                 this->defBoundarySound_->setVolume(0.5f);
             }
             else
             {
                 this->defScoreSound_ = 0;
                 this->defBatSound_ = 0;
                 this->defBoundarySound_ = 0;
             }
    }

    /**
    @brief
        Destructor.
    */
    PongBall::~PongBall()
    {
        if (this->isInitialized())
        {
            if (this->bDeleteBats_)
                delete[] this->bat_;

            delete[] this->batID_;
        }
    }

    //xml port for loading sounds
    void PongBall::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PongBall, XMLPort, xmlelement, mode);
        XMLPortParam(PongBall, "defScoreSound",  setDefScoreSound,  getDefScoreSound,  xmlelement, mode);
        XMLPortParam(PongBall, "defBatSound",  setDefBatSound,  getDefBatSound,  xmlelement, mode);
        XMLPortParam(PongBall, "defBoundarySound",  setDefBoundarySound,  getDefBoundarySound,  xmlelement, mode);
    }

    /**
    @brief
        Register variables to synchronize over the network.
    */
    void PongBall::registerVariables()
    {
        registerVariable( this->fieldWidth_ );
        registerVariable( this->fieldHeight_ );
        registerVariable( this->batlength_ );
        registerVariable( this->speed_ );
        registerVariable( this->relMercyOffset_ );
        registerVariable( this->batID_[0] );
        registerVariable( this->batID_[1], VariableDirection::ToClient, new NetworkCallback<PongBall>( this, &PongBall::applyBats) );
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
    void PongBall::tick(float dt)
    {
        SUPER(PongBall, tick, dt);

        // Get the current position, velocity and acceleration of the ball.
        Vector3 position = this->getPosition();
        Vector3 velocity = this->getVelocity();
        Vector3 acceleration = this->getAcceleration();

        // If the ball has gone over the top or bottom boundary of the playing field (i.e. the ball has hit the top or bottom delimiters).
        if (position.z > this->fieldHeight_ / 2 || position.z < -this->fieldHeight_ / 2)
        {
            defBoundarySound_->play(); //play boundary sound
            // Its velocity in z-direction is inverted (i.e. it bounces off).
            velocity.z = -velocity.z;
            // And its position is set as to not overstep the boundary it has just crossed.
            if (position.z > this->fieldHeight_ / 2)
                position.z = this->fieldHeight_ / 2;
            if (position.z < -this->fieldHeight_ / 2)
                position.z = -this->fieldHeight_ / 2;

            this->fireEvent();
        }

        // If the ball has crossed the left or right boundary of the playing field (i.e. a player has just scored, if the bat isn't there to parry).
        if (position.x > this->fieldWidth_ / 2 || position.x < -this->fieldWidth_ / 2)
        {
            float distance = 0;

            if (this->bat_ != NULL) // If there are bats.
            {
                // If the right boundary has been crossed.
                if (position.x > this->fieldWidth_ / 2 && this->bat_[1] != NULL)
                {
                    // Calculate the distance (in z-direction) between the ball and the center of the bat, weighted by half of the effective length of the bat (with additional 10%)
                    distance = (position.z - this->bat_[1]->getPosition().z) / (this->fieldHeight_ * (this->batlength_ * 1.10f) / 2);
                    if (fabs(distance) <= 1) // If the bat is there to parry.
                    {
                        defBatSound_->play(); //play bat sound
                        // Set the ball to be exactly at the boundary.
                        position.x = this->fieldWidth_ / 2;
                        // Invert its velocity in x-direction (i.e. it bounces off).
                        velocity.x = -velocity.x;
                        // Adjust the velocity in the z-direction, depending on where the ball hit the bat.
                        velocity.z = distance * distance * sgn(distance) * PongBall::MAX_REL_Z_VELOCITY * this->speed_;
                        acceleration = this->bat_[1]->getVelocity() * this->accelerationFactor_ * -1;

                        this->fireEvent();
                    }
                    // If the left player scores.
                    else if (GameMode::isMaster() && position.x > this->fieldWidth_ / 2 * (1 + this->relMercyOffset_))
                    {
                        defScoreSound_->play();//play score sound
                        if (this->getGametype() && this->bat_[0])
                        {
                            this->getGametype()->playerScored(this->bat_[0]->getPlayer());
                            return;
                        }
                    }
                }
                // If the left boundary has been crossed.
                else if (position.x < -this->fieldWidth_ / 2 && this->bat_[0] != NULL)
                {
                    // Calculate the distance (in z-direction) between the ball and the center of the bat, weighted by half of the effective length of the bat (with additional 10%)
                    distance = (position.z - this->bat_[0]->getPosition().z) / (this->fieldHeight_ * (this->batlength_ * 1.10f) / 2);
                    if (fabs(distance) <= 1) // If the bat is there to parry.
                    {
                        defBatSound_->play(); //play bat sound
                        // Set the ball to be exactly at the boundary.
                        position.x = -this->fieldWidth_ / 2;
                        // Invert its velocity in x-direction (i.e. it bounces off).
                        velocity.x = -velocity.x;
                        // Adjust the velocity in the z-direction, depending on where the ball hit the bat.
                        velocity.z = distance * distance * sgn(distance) * PongBall::MAX_REL_Z_VELOCITY * this->speed_;
                        acceleration = this->bat_[0]->getVelocity() * this->accelerationFactor_ * -1;

                        this->fireEvent();
                    }
                    // If the right player scores.
                    else if (GameMode::isMaster() && position.x < -this->fieldWidth_ / 2 * (1 + this->relMercyOffset_))
                    {
                        defScoreSound_->play();//play score sound
                        if (this->getGametype() && this->bat_[1])
                        {
                            this->getGametype()->playerScored(this->bat_[1]->getPlayer());
                            return;
                        }
                    }
                }
            }
        }

        // Set the position, velocity and acceleration of the ball, if they have changed.
        if (acceleration != this->getAcceleration())
            this->setAcceleration(acceleration);
        if (velocity != this->getVelocity())
            this->setVelocity(velocity);
        if (position != this->getPosition())
            this->setPosition(position);
    }

    /**
    @brief
        Set the speed of the ball (in x-direction).
    @param speed
        The speed to be set.
    */
    void PongBall::setSpeed(float speed)
    {
        if (speed != this->speed_) // If the speed changes
        {
            this->speed_ = speed;

            // Set the speed in the direction of the balls current velocity.
            Vector3 velocity = this->getVelocity();
            if (velocity.x != 0)
                velocity.x = sgn(velocity.x) * this->speed_;
            else // If the balls current velocity is zero, the speed is set in a random direction.
                velocity.x = this->speed_ * sgn(rnd(-1,1));

            this->setVelocity(velocity);
        }
    }

    /**
    @brief
        Set the bats for the ball.
    @param bats
        An array (of size 2) of weak pointers, to be set as the new bats.
    */
    void PongBall::setBats(WeakPtr<PongBat>* bats)
    {
        if (this->bDeleteBats_) // If there are already some bats, delete them.
        {
            delete[] this->bat_;
            this->bDeleteBats_ = false;
        }

        this->bat_ = bats;
        // Also store their object IDs, for synchronization.
        this->batID_[0] = this->bat_[0]->getObjectID();
        this->batID_[1] = this->bat_[1]->getObjectID();
    }

    /**
    @brief
        Get the bats over the network.
    */
    void PongBall::applyBats()
    {
        // Make space for the bats, if they don't exist, yet.
        if (this->bat_ == NULL)
        {
            this->bat_ = new WeakPtr<PongBat>[2];
            this->bDeleteBats_ = true;
        }

        if (this->batID_[0] != OBJECTID_UNKNOWN)
            this->bat_[0] = orxonox_cast<PongBat*>(Synchronisable::getSynchronisable(this->batID_[0]));
        if (this->batID_[1] != OBJECTID_UNKNOWN)
            this->bat_[1] = orxonox_cast<PongBat*>(Synchronisable::getSynchronisable(this->batID_[1]));
    }

    void PongBall::setDefScoreSound(const std::string &pongSound)
    {
        if( defScoreSound_ )
            defScoreSound_->setSource(pongSound);
        else
            assert(0); // This should never happen, because soundpointer is only available on master
    }

    const std::string& PongBall::getDefScoreSound()
    {
        if( defScoreSound_ )
            return defScoreSound_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }

    void PongBall::setDefBatSound(const std::string &pongSound)
    {
        if( defBatSound_ )
            defBatSound_->setSource(pongSound);
        else
            assert(0); // This should never happen, because soundpointer is only available on master
    }

    const std::string& PongBall::getDefBatSound()
    {
        if( defBatSound_ )
            return defBatSound_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }

    void PongBall::setDefBoundarySound(const std::string &pongSound)
    {
        if( defBoundarySound_ )
            defBoundarySound_->setSource(pongSound);
        else
            assert(0); // This should never happen, because soundpointer is only available on master
    }

    const std::string& PongBall::getDefBoundarySound()
    {
        if( defBoundarySound_ )
            return defBoundarySound_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }
}
