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
    @file PongCenterpoint.h
    @brief Declaration of the PongCenterpoint class.
    @ingroup Pong
*/

#ifndef _PongCenterpoint_H__
#define _PongCenterpoint_H__

#include "pong/PongPrereqs.h"

#include <string>

#include <util/Math.h>

#include "worldentities/StaticEntity.h"

namespace orxonox
{
    
    /**
    @brief
        The PongCenterpoint implements the playing field @ref orxonox::Pong "Pong" takes place in and allows for many parameters of the minigame to be set.
        The playing field resides in the x,z-plane, with the x-axis being the horizontal axis and the z-axis being the vertical axis.
        
        Various parameters can be set:
        - The <b>dimension</b> is a vector, that defines the width and height of the playing field. The default is <em>(200, 120)</em>.
        - The <b>balltemplate</b> is a template that is applied to the @ref orxonox::PongBall "PongBall", it can be used to attach different things to it, e.g. its @ref orxonox::Model "Model". See below for a usage example.
        - The <b>battemplate</b> is a template that is applied to the @ref orxonox::PongBall "PongBat", it can be used to attach different things to it, e.g. its @ref orxonox::Model "Model". See below for a usage example.
        - The <b>ballspeed</b> is the speed with which the @ref orxonox::PongBall "PongBall" moves. The default is <em>100</em>.
        - The <b>ballaccfactor</b> is the acceleration factor for the @ref orxonox::PongBall "PongBall". The default is <em>1.0</em>.
        - The <b>batspeed</b> is the speed with which the @ref orxonox::PongBat "PongBats" move. The default is <em>60</em>.
        - The <b>batlength</b> is the length of the @ref orxonox::PongBat "PongBats" as the percentage of the height of the playing field. The default is <em>0.25</em>.
        
        An example in XML of the PongCenterpoint would be:
        
        First the needed templates:
        The template for the @ref orxonox::PongBall "PongBall".
        @code
        <Template name="pongball">
          <PongBall>
            <attached>
              <Model mesh="sphere.mesh" scale="2" />
              <ParticleSpawner name="hiteffect" position="0,0,0" source="Orxonox/sparks2" lifetime="0.01" autostart="0" mainstate="spawn" />
            </attached>
            <eventlisteners>
              <EventTarget target="hiteffect" />
            </eventlisteners>
          </PongBall>
        </Template>
        @endcode
        As can be seen, a sphere is attached as the @ref orxonox::Model "Model" for the @ref orxonox::PongBall "PongBall", and also an @ref orxonox::EventListener "EventListener" that triggers a @ref orxonox::ParticleSpawner "ParticleSpawner", whenever the ball hits the boundaries is attached.
        
        Additionally the template for the @ref orxonox::PongBat "PongBat".
        @code
        <Template name="pongbatcameras" defaults="0">
          <PongBat>
            <camerapositions>
              <CameraPosition position="0,200,0" pitch="-90" absolute="true" />
            </camerapositions>
          </PongBat>
        </Template>

        <Template name="pongbat">
          <PongBat camerapositiontemplate=pongbatcameras>
            <attached>
              <Model position="0,0,3" mesh="cube.mesh" scale3D="14,2,2" />
            </attached>
          </PongBat>
        </Template>
        @endcode
        As can be seen, there are actually two templates. The first template is needed to set the camera for the @ref orxonox::PongBat "PongBat". The second template ist the actual template for the @ref orxonox::PongBat "PongBat", the template for the camera position is added and a @ref orxonox::Model "Model" for the @ref orxonox::PongBat "PongBat" is attached.
        
        Finally the PongCenterpoint is created.
        @code
        <PongCenterpoint name="pongcenter" dimension="200,120" balltemplate="pongball" battemplate="pongbat" ballspeed="200" ballaccfactor="1.0" batspeed="130" batlength="0.25">
          <attached>
            <Model position="0,0,60" mesh="cube.mesh" scale3D="105,1,1" />
            <Model position="0,0,-60" mesh="cube.mesh" scale3D="105,1,1" />
          </attached>
        </PongCenterpoint>
        @endcode
        All parameters are specified. And also two @ref orxonox::Model "Models" (for the upper and lower boundary) are attached.
        
        For a more elaborate example, have a look at the <code>pong.oxw</code> level file.
    
    @author
        Fabian 'x3n' Landau
        
    @ingroup Pong
    */
    class _PongExport PongCenterpoint : public StaticEntity
    {
        public:
            PongCenterpoint(Context* context); //!< Constructor. Registers and initializes the object and checks whether the gametype is actually Pong.
            virtual ~PongCenterpoint() {}

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method to create a PongCenterpoint through XML.

            virtual void changedGametype(); //!< Is called when the gametype has changed.

            /**
            @brief Set the template for the ball. (e.g. to attach the model of the ball, but also to attach an EventListener to it to detect, when it hits the boundaries, and e.g. display some ParticleEffets, when it does.)
            @param balltemplate The name of the template to be set.
            */
            void setBalltemplate(const std::string& balltemplate)
                { this->balltemplate_ = balltemplate; }
            /**
            @brief Get the template of the ball.
            @return Returns the name of the template of the ball. 
            */
            const std::string& getBalltemplate() const
                { return this->balltemplate_; }

            /**
            @brief Set the template for the bats. (e.g. to attach the model of the bat, but also to attach CameraPositions to it, to be able to view the game from the bats perspective)
            @param battemplate The name of the template to be set.
            */
            void setBattemplate(const std::string& battemplate)
                { this->battemplate_ = battemplate; }
            /**
            @brief Get the template of the bats.
            @return Returns the name of the template of the bats.
            */
            const std::string& getBattemplate() const
                { return this->battemplate_; }

            /**
            @brief Set the dimensions of the playing field.
            @param dimension A vector with the width of the playing field as first component and the height as second.
            */
            void setFieldDimension(const Vector2& dimension)
                { this->width_ = dimension.x; this->height_ = dimension.y; }
            /**
            @brief Get the dimensions of the playing field.
            @return Returns a vector with the width of the playing field as first component and the height as second.
            */
            Vector2 getFieldDimension() const
                { return Vector2(this->width_, this->height_); }

            /**
            @brief Set the speed of the ball.
            @param ballspeed The speed of the ball.
            */
            void setBallSpeed(float ballspeed)
                { this->ballspeed_ = ballspeed; }
            /**
            @brief Get the speed of the ball.
            @return Returns the speed of the ball.
            */
            float getBallSpeed() const
                { return this->ballspeed_; }

            /**
            @brief Set the ball's acceleration factor.
            @param ballaccfactor The ball's acceleration factor.
            */
            void setBallAccelerationFactor(float ballaccfactor)
                { this->ballaccfactor_ = ballaccfactor; }
            /**
            @brief Get the ball's acceleration factor
            @return Returns the ball's acceleration factor.
            */
            float getBallAccelerationFactor() const
                { return this->ballaccfactor_; }

            /**
            @brief Set the speed of the bats.
            @param batspeed The speed of the bats.
            */
            void setBatSpeed(float batspeed)
                { this->batspeed_ = batspeed; }
            /**
            @brief Get the speed of the bats.
            @return Returns the speed of the bats.
            */
            float getBatSpeed() const
                { return this->batspeed_; }

            /**
            @brief Set the length of the bats.
            @param batlength The length of the bats (in z-direction) as a percentage of the height of the playing field.
            */
            void setBatLength(float batlength)
                { this->batlength_ = batlength; }
            /**
            @brief Get the length of the bats.
            @return Returns the length of the bats (in z-direction) as a percentage of the height of the playing field.
            */
            float getBatLength() const
                { return this->batlength_; }

        private:
            void checkGametype(); //!< Checks whether the gametype is Pong and if it is, sets its centerpoint.

            std::string balltemplate_; //!< The template for the ball.
            std::string battemplate_; //!< The template for the bats.

            float ballspeed_; //!< The speed of then ball.
            float ballaccfactor_; //!< The acceleration factor of the ball.
            float batspeed_; //!< The speed of the bat.
            float batlength_; //!< The length of the bat (in z-direction) as a percentage of the height of the playing field.

            float width_; //!< The height of the playing field.
            float height_; //!< The width of the playing field.
    };
}

#endif /* _PongCenterpoint_H__ */
