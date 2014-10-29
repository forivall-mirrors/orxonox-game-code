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
    @file Mini4DgameCenterpoint.h
    @brief Declaration of the Mini4DgameCenterpoint class.
*/

#ifndef _Mini4DgameCenterpoint_H__
#define _Mini4DgameCenterpoint_H__

#include <string>

#include <util/Math.h>

#include "worldentities/StaticEntity.h"
#include "mini4Dgame/Mini4DgamePrereqs.h"

namespace orxonox
{
    
    /**
    @brief
        The Mini4DgameCenterpoint implements the playing field @ref orxonox::Mini4Dgame "Mini4Dgame" takes place in and allows for many parameters of the minigame to be set.
        
        Various parameters can be set:
        - The <b>dimension</b> is a vector, that defines the width and height of the playing field. The default is <em>(200, 120)</em>.
        - The <b>balltemplate</b> is a template that is applied to the @ref orxonox::PongBall "PongBall", it can be used to attach different things to it, e.g. its @ref orxonox::Model "Model". See below for a usage example.
        - The <b>battemplate</b> is a template that is applied to the @ref orxonox::PongBall "PongBat", it can be used to attach different things to it, e.g. its @ref orxonox::Model "Model". See below for a usage example.
        - The <b>ballspeed</b> is the speed with which the @ref orxonox::PongBall "PongBall" moves. The default is <em>100</em>.
        - The <b>ballaccfactor</b> is the acceleration factor for the @ref orxonox::PongBall "PongBall". The default is <em>1.0</em>.
        - The <b>batspeed</b> is the speed with which the @ref orxonox::PongBat "PongBats" move. The default is <em>60</em>.
        - The <b>batlength</b> is the length of the @ref orxonox::PongBat "PongBats" as the percentage of the height of the playing field. The default is <em>0.25</em>.
    */
    class _Mini4DgameExport Mini4DgameCenterpoint : public StaticEntity
    {
        public:
            Mini4DgameCenterpoint(Context* context); //!< Constructor. Registers and initializes the object and checks whether the gametype is actually Mini4Dgame.
            virtual ~Mini4DgameCenterpoint() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method to create a Mini4DgameCenterpoint through XML.

            virtual void changedGametype(); //!< Is called when the gametype has changed.

            /**
            @brief Set the template for the ball. (e.g. to attach the model of the ball, but also to attach an EventListener to it to detect, when it hits the boundaries, and e.g. display some ParticleEffets, when it does.)
            @param balltemplate The name of the template to be set.

            void setBalltemplate(const std::string& balltemplate)
                { this->balltemplate_ = balltemplate; }
            */

            /**
            @brief Get the template of the ball.
            @return Returns the name of the template of the ball. 

            const std::string& getBalltemplate() const
                { return this->balltemplate_; }
            */

            /**
            @brief Set the template for the bats. (e.g. to attach the model of the bat, but also to attach CameraPositions to it, to be able to view the game from the bats perspective)
            @param battemplate The name of the template to be set.

            void setBattemplate(const std::string& battemplate)
                { this->battemplate_ = battemplate; }
            */

            /**
            @brief Set the dimensions of the playing field.
            @param dimension A vector with the width of the playing field as first component, the height as second and the length as third.
            */
            void setFieldDimension(const Vector3& dimension)
                { this->width_ = dimension.x; this->height_ = dimension.y; this->length_ = dimension.z;}
            /**
            @brief Get the dimensions of the playing field.
            @return Returns a vector with the width of the playing field as first component, the height as second and the length as third.
            */
            Vector3 getFieldDimension() const
                { return Vector3(this->width_, this->height_, this->length_); }


        private:
            void checkGametype(); //!< Checks whether the gametype is Mini4Dgame and if it is, sets its centerpoint.

            //std::string balltemplate_; //!< The template for the ball.
            //std::string battemplate_; //!< The template for the bats.

            float width_; //!< The height of the playing field.
            float height_; //!< The width of the playing field.
            float length_; //!< The length of the playing field.
    };
}

#endif /* _Mini4DgameCenterpoint_H__ */
