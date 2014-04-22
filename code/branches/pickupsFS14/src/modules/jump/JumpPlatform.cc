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
    @file JumpPlatform.cc
    @brief Implementation of the JumpPlatform class.
*/

#include "JumpPlatform.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "graphics/Model.h"
#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpPlatform);

    const float JumpPlatform::MAX_REL_Z_VELOCITY = 1.5;

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpPlatform::JumpPlatform(Context* context) : MovableEntity(context)
    {
        RegisterObject(JumpPlatform);

        this->figure_ = 0;
        this->bDeleteBats_ = false;
        this->batID_ = new unsigned int[1];
        this->batID_[0] = OBJECTID_UNKNOWN;
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

        this->setPosition(Vector3(0,0,0));
        this->setVelocity(Vector3(0,0,0));
        this->setAcceleration(Vector3(0,0,0));

        model = NULL;
    }

    /**
    @brief
        Destructor.
    */
    JumpPlatform::~JumpPlatform()
    {
        /*if (this->isInitialized())
        {
            if (this->bDeleteBats_)
                delete this->figure_;

            delete[] this->batID_;
        }*/
    }

    //xml port for loading sounds
    void JumpPlatform::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPlatform, XMLPort, xmlelement, mode);
        XMLPortParam(JumpPlatform, "defScoreSound",  setDefScoreSound,  getDefScoreSound,  xmlelement, mode);
        XMLPortParam(JumpPlatform, "defBatSound",  setDefBatSound,  getDefBatSound,  xmlelement, mode);
        XMLPortParam(JumpPlatform, "defBoundarySound",  setDefBoundarySound,  getDefBoundarySound,  xmlelement, mode);
    }

    /**
    @brief
        Register variables to synchronize over the network.
    */
    void JumpPlatform::registerVariables()
    {
        registerVariable( this->fieldWidth_ );
        registerVariable( this->fieldHeight_ );
        registerVariable( this->relMercyOffset_ );
        registerVariable( this->batID_[0] );
        //registerVariable( this->batID_[1], VariableDirection::ToClient, new NetworkCallback<JumpPlatform>( this, &JumpPlatform::applyBats) );
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
    void JumpPlatform::tick(float dt)
    {
        SUPER(JumpPlatform, tick, dt);

        Vector3 platformPosition = this->getPosition();

        if (figure_ != NULL)
        {
            Vector3 figurePosition = figure_->getPosition();
            Vector3 figureVelocity = figure_->getVelocity();

            if(figureVelocity.z < 0 && figurePosition.x > platformPosition.x-10 && figurePosition.x < platformPosition.x+10 && figurePosition.z > platformPosition.z-4 && figurePosition.z < platformPosition.z+4)
            {
            	figure_->JumpFromPlatform(200.0f);
            }
        }





        /*
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
                        velocity.z = distance * distance * sgn(distance) * JumpPlatform::MAX_REL_Z_VELOCITY * this->speed_;
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
                    distance = (position.z - this->figure_->getPosition().z) / (this->fieldHeight_ * (this->batlength_ * 1.10f) / 2);
                    if (fabs(distance) <= 1) // If the bat is there to parry.
                    {
                        defBatSound_->play(); //play bat sound
                        // Set the ball to be exactly at the boundary.
                        position.x = -this->fieldWidth_ / 2;
                        // Invert its velocity in x-direction (i.e. it bounces off).
                        velocity.x = -velocity.x;
                        // Adjust the velocity in the z-direction, depending on where the ball hit the bat.
                        velocity.z = distance * distance * sgn(distance) * JumpPlatform::MAX_REL_Z_VELOCITY * this->speed_;
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
        */
    }

    /**
    @brief
        Set the bats for the ball.
    @param bats
        An array (of size 2) of weak pointers, to be set as the new bats.
    */
    void JumpPlatform::setFigure(WeakPtr<JumpFigure> newFigure)
    {
        if (this->bDeleteBats_) // If there are already some bats, delete them.
        {
            delete this->figure_;
            this->bDeleteBats_ = false;
        }

        this->figure_ = newFigure;
        // Also store their object IDs, for synchronization.
        this->batID_[0] = this->figure_->getObjectID();
    }

    /**
    @brief
        Get the bats over the network.
    */
    void JumpPlatform::applyBats()
    {
        // Make space for the bats, if they don't exist, yet.
        if (this->figure_ == NULL)
        {
            this->figure_ = *(new WeakPtr<JumpFigure>);
            this->bDeleteBats_ = true;
        }

        if (this->batID_[0] != OBJECTID_UNKNOWN)
        {
        	// WAR IM PONG NICHT AUSKOMMENTIERT!!!
            //this->figure_ = orxonox_cast<JumpFigure>(Synchronisable::getSynchronisable(this->batID_[0]));
        }
    }

    void JumpPlatform::setDefScoreSound(const std::string &jumpSound)
    {
        if( defScoreSound_ )
            defScoreSound_->setSource(jumpSound);
        else
            assert(0); // This should never happen, because soundpointer is only available on master
    }

    const std::string& JumpPlatform::getDefScoreSound()
    {
        if( defScoreSound_ )
            return defScoreSound_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }

    void JumpPlatform::setDefBatSound(const std::string &jumpSound)
    {
        if( defBatSound_ )
            defBatSound_->setSource(jumpSound);
        else
            assert(0); // This should never happen, because soundpointer is only available on master
    }

    const std::string& JumpPlatform::getDefBatSound()
    {
        if( defBatSound_ )
            return defBatSound_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }

    void JumpPlatform::setDefBoundarySound(const std::string &jumpSound)
    {
        if( defBoundarySound_ )
            defBoundarySound_->setSource(jumpSound);
        else
            assert(0); // This should never happen, because soundpointer is only available on master
    }

    const std::string& JumpPlatform::getDefBoundarySound()
    {
        if( defBoundarySound_ )
            return defBoundarySound_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }
}
