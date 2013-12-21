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
    @file PongBall.h
    @brief Declaration of the PongBall class.
    @ingroup Pong
*/

#ifndef _PongBall_H__
#define _PongBall_H__

#include "pong/PongPrereqs.h"

#include "util/Math.h"

#include "worldentities/MovableEntity.h"


namespace orxonox
{

    /**
    @brief
        This class manages the ball for @ref orxonox::Pong "Pong".

        It is responsible for both the movement of the ball in the x,z-plane as well as its interaction with the boundaries of the playing field (defined by the @ref orxonox::PongCenterpoint "PongCenterpoint") and the @ref orxonox::PongBat "PongBats". Or more precisely, it makes the ball bounce off then upper and lower delimiters of the playing field, it makes the ball bounce off the bats and also detects when a player scores and takes appropriate measures.

    @author
        Fabian 'x3n' Landau

    @ingroup Pong
    */
    class _PongExport PongBall : public MovableEntity
    {
        public:
            PongBall(Context* context);
            virtual ~PongBall();

            virtual void tick(float dt);

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            /**
            @brief Set the dimensions of the playing field.
            @param width The width of the playing field.
            @param height The height of the playing field.
            */
            void setFieldDimension(float width, float height)
                { this->fieldWidth_ = width; this->fieldHeight_ = height; }
            /**
            @brief Get the dimensions of the playing field.
            @param dimension A vector with the width as the first and height as the second component.
            */
            void setFieldDimension(const Vector2& dimension)
                { this->setFieldDimension(dimension.x, dimension.y); }
            /**
            @brief Get the dimensions of the playing field.
            @return Returns a vector with the width as the first and height as the second component.
            */
            Vector2 getFieldDimension() const
                { return Vector2(this->fieldWidth_, this->fieldHeight_); }

            void setSpeed(float speed); //!< Set the speed of the ball (in x-direction).
            /**
            @brief Get the speed of the ball (in x-direction).
            @return Returns the speed of the ball (in x-direction).
            */
            float getSpeed() const
                { return this->speed_; }

            /**
            @brief Set the acceleration factor of the ball.
            @param factor The factor the acceleration of the ball is set to.
            */
            void setAccelerationFactor(float factor)
                { this->accelerationFactor_ = factor; }
            /**
            @brief Get the acceleration factor of the ball.
            @return Returns the acceleration factor of the ball.
            */
            float getAccelerationFactor() const
                { return this->accelerationFactor_; }

            /**
            @brief Set the length of the bats.
            @param batlength The length of the bats (in z-direction) as percentage of the height of the playing field.
            */
            void setBatLength(float batlength)
                { this->batlength_ = batlength; }
            /**
            @brief Get the length of the bats.
            @return Returns the length of the bats (in z-direction) as percentage of the height of the playing field.
            */
            float getBatLength() const
                { return this->batlength_; }

            void setBats(WeakPtr<PongBat>* bats); //!< Set the bats for the ball.
            void applyBats(); //!< Get the bats over the network.

            static const float MAX_REL_Z_VELOCITY;

            void setDefScoreSound(const std::string& engineSound);
            const std::string& getDefScoreSound();
            void setDefBatSound(const std::string& engineSound);
            const std::string& getDefBatSound();
            void setDefBoundarySound(const std::string& engineSound);
            const std::string& getDefBoundarySound();

        private:
            void registerVariables();

            float fieldWidth_; //!< The width of the playing field.
            float fieldHeight_; //!< The height of the playing field.
            float speed_; //!< The speed (in x-direction) of the ball.
            float accelerationFactor_; //!< The acceleration factor of the ball.
            float batlength_; //!< The length of the bats (in z-direction) as percentage of the height of the playing field.
            WeakPtr<PongBat>* bat_; //!< An array with the two bats.
            bool bDeleteBats_; //!< Bool, to keep track, of whether this->bat_ exists or not.
            unsigned int* batID_; //!< The object IDs of the bats, to be able to synchronize them over the network.
            float relMercyOffset_; //!< Offset, that makes the player not loose, when, in all fairness, he would have.
            WorldSound* defScoreSound_;
            WorldSound* defBatSound_;
            WorldSound* defBoundarySound_;
    };
}

#endif /* _PongBall_H__ */
