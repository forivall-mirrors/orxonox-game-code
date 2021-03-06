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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

#ifndef _JumpCenterpoint_H__
#define _JumpCenterpoint_H__

#include "jump/JumpPrereqs.h"

#include <string>

#include <util/Math.h>

#include "worldentities/StaticEntity.h"

namespace orxonox
{
    
    /**
    @brief
        The JumpCenterpoint implements the playing field @ref orxonox::Jump "Jump" takes place in and allows for many parameters of the minigame to be set.
        The playing field resides in the x,z-plane, with the x-axis being the horizontal axis and the z-axis being the vertical axis.
        
        Various parameters can be set:
        - The <b>dimension</b> is a vector, that defines the width and height of the playing field. The default is <em>(200, 120)</em>.
        - The <b>balltemplate</b> is a template that is applied to the @ref orxonox::JumpPlatform "JumpPlatform", it can be used to attach different things to it, e.g. its @ref orxonox::Model "Model". See below for a usage example.
        - The <b>battemplate</b> is a template that is applied to the @ref orxonox::JumpPlatform "JumpFigure", it can be used to attach different things to it, e.g. its @ref orxonox::Model "Model". See below for a usage example.
        - The <b>ballspeed</b> is the speed with which the @ref orxonox::JumpPlatform "JumpPlatform" moves. The default is <em>100</em>.
        - The <b>ballaccfactor</b> is the acceleration factor for the @ref orxonox::JumpPlatform "JumpPlatform". The default is <em>1.0</em>.
        - The <b>batspeed</b> is the speed with which the @ref orxonox::JumpFigure "JumpFigures" move. The default is <em>60</em>.
        - The <b>batlength</b> is the length of the @ref orxonox::JumpFigure "JumpFigures" as the percentage of the height of the playing field. The default is <em>0.25</em>.
        
        An example in XML of the JumpCenterpoint would be:
        
        First the needed templates:
        The template for the @ref orxonox::JumpPlatform "JumpPlatform".
        @code
        <Template name="jumpball">
          <JumpPlatform>
            <attached>
              <Model mesh="sphere.mesh" scale="2" />
              <ParticleSpawner name="hiteffect" position="0,0,0" source="Orxonox/sparks2" lifetime="0.01" autostart="0" mainstate="spawn" />
            </attached>
            <eventlisteners>
              <EventTarget target="hiteffect" />
            </eventlisteners>
          </JumpPlatform>
        </Template>
        @endcode
        As can be seen, a sphere is attached as the @ref orxonox::Model "Model" for the @ref orxonox::JumpPlatform "JumpPlatform", and also an @ref orxonox::EventListener "EventListener" that triggers a @ref orxonox::ParticleSpawner "ParticleSpawner", whenever the ball hits the boundaries is attached.
        
        Additionally the template for the @ref orxonox::JumpFigure "JumpFigure".
        @code
        <Template name="jumpbatcameras" defaults="0">
          <JumpFigure>
            <camerapositions>
              <CameraPosition position="0,200,0" pitch="-90" absolute="true" />
            </camerapositions>
          </JumpFigure>
        </Template>

        <Template name="jumpbat">
          <JumpFigure camerapositiontemplate=jumpbatcameras>
            <attached>
              <Model position="0,0,3" mesh="cube.mesh" scale3D="14,2,2" />
            </attached>
          </JumpFigure>
        </Template>
        @endcode
        As can be seen, there are actually two templates. The first template is needed to set the camera for the @ref orxonox::JumpFigure "JumpFigure". The second template ist the actual template for the @ref orxonox::JumpFigure "JumpFigure", the template for the camera position is added and a @ref orxonox::Model "Model" for the @ref orxonox::JumpFigure "JumpFigure" is attached.
        propellerTemplate_
        Finally the JumpCenterpoint is created.
        @code
        <JumpCenterpoint name="jumpcenter" dimension="200,120" balltemplate="jumpball" battemplate="jumpbat" ballspeed="200" ballaccfactor="1.0" batspeed="130" batlength="0.25">
          <attached>
            <Model position="0,0,60" mesh="cube.mesh" scale3D="105,1,1" />
            <Model position="0,0,-60" mesh="cube.mesh" scale3D="105,1,1" />
          </attached>
        </JumpCenterpoint>
        @endcode
        All parameters are specified. And also two @ref orxonox::Model "Models" (for the upper and lower boundary) are attached.
        
        For a more elaborate example, have a look at the <code>jump.oxw</code> level file.

    */
    class _JumpExport JumpCenterpoint : public StaticEntity
    {
        public:
            JumpCenterpoint(Context* context); //!< Constructor. Registers and initializes the object and checks whether the gametype is actually Jump.
            virtual ~JumpCenterpoint() {}
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method to create a JumpCenterpoint through XML.
            virtual void changedGametype();
            void setPlatformStaticTemplate(const std::string& balltemplate)
                { this->platformStaticTemplate_ = balltemplate; }
            const std::string& getPlatformStaticTemplate() const
                { return this->platformStaticTemplate_; }
            void setPlatformHMoveTemplate(const std::string& balltemplate)
                { this->platformHMoveTemplate_ = balltemplate; }
            const std::string& getPlatformHMoveTemplate() const
                { return this->platformHMoveTemplate_; }
            void setPlatformVMoveTemplate(const std::string& balltemplate)
                { this->platformVMoveTemplate_ = balltemplate; }
            const std::string& getPlatformVMoveTemplate() const
                { return this->platformVMoveTemplate_; }
            void setPlatformDisappearTemplate(const std::string& balltemplate)
                { this->platformDisappearTemplate_ = balltemplate; }
            const std::string& getPlatformDisappearTemplate() const
                { return this->platformDisappearTemplate_; }
            void setPlatformTimerTemplate(const std::string& balltemplate)
                { this->platformTimerTemplate_ = balltemplate; }
            const std::string& getPlatformTimerTemplate() const
                { return this->platformTimerTemplate_; }
            void setPlatformFakeTemplate(const std::string& balltemplate)
                { this->platformFakeTemplate_ = balltemplate; }
            const std::string& getPlatformFakeTemplate() const
                { return this->platformFakeTemplate_; }
            void setProjectileTemplate(const std::string& newTemplate)
                { this->projectileTemplate_ = newTemplate; }
            const std::string& getProjectileTemplate() const
                { return this->projectileTemplate_; }
            void setSpringTemplate(const std::string& newTemplate)
                { this->springTemplate_ = newTemplate; }
            const std::string& getSpringTemplate() const
                { return this->springTemplate_; }
            void setRocketTemplate(const std::string& newTemplate)
                { this->rocketTemplate_ = newTemplate; }
            const std::string& getRocketTemplate() const
                { return this->rocketTemplate_; }
            void setPropellerTemplate(const std::string& newTemplate)
                { this->propellerTemplate_ = newTemplate; }
            const std::string& getPropellerTemplate() const
                { return this->propellerTemplate_; }
            void setBootsTemplate(const std::string& newTemplate)
                { this->bootsTemplate_ = newTemplate; }
            const std::string& getBootsTemplate() const
                { return this->bootsTemplate_; }
            void setShieldTemplate(const std::string& newTemplate)
                { this->shieldTemplate_ = newTemplate; }
            const std::string& getShieldTemplate() const
                { return this->shieldTemplate_; }
            void setFigureTemplate(const std::string& newTemplate)
                { this->figureTemplate_ = newTemplate; }
            const std::string& getFigureTemplate() const
                { return this->figureTemplate_; }
            void setEnemy1Template(const std::string& newTemplate)
                { this->enemy1Template_ = newTemplate; }
            const std::string& getEnemy1Template() const
                { return this->enemy1Template_; }
            void setEnemy2Template(const std::string& newTemplate)
                { this->enemy2Template_ = newTemplate; }
            const std::string& getEnemy2Template() const
                { return this->enemy2Template_; }
            void setEnemy3Template(const std::string& newTemplate)
                { this->enemy3Template_ = newTemplate; }
            const std::string& getEnemy3Template() const
                { return this->enemy3Template_; }
            void setEnemy4Template(const std::string& newTemplate)
                { this->enemy4Template_ = newTemplate; }
            const std::string& getEnemy4Template() const
                { return this->enemy4Template_; }
            void setFieldDimension(const Vector2& dimension)
                { this->width_ = dimension.x; this->height_ = dimension.y; }
            Vector2 getFieldDimension() const
                { return Vector2(this->width_, this->height_); }
            void setSectionLength(const float sectionLength)
                { this->sectionLength_ = sectionLength; }
            float getSectionLength() const
                { return sectionLength_; }
            void setPlatformSpeed(const float platformSpeed)
                { this->platformSpeed_ = platformSpeed; }
            float getPlatformSpeed() const
                { return platformSpeed_; }
            void setCameraOffset(const float cameraOffset)
                { this->cameraOffset_ = cameraOffset; }
            float getCameraOffset() const
                { return cameraOffset_; }
        private:
            void checkGametype();
            std::string platformStaticTemplate_;
            std::string platformHMoveTemplate_;
            std::string platformVMoveTemplate_;
            std::string platformDisappearTemplate_;
            std::string platformTimerTemplate_;
            std::string platformFakeTemplate_;
            std::string projectileTemplate_;
            std::string springTemplate_;
            std::string rocketTemplate_;
            std::string propellerTemplate_;
            std::string bootsTemplate_;
            std::string shieldTemplate_;
            std::string figureTemplate_;
            std::string enemy1Template_;
            std::string enemy2Template_;
            std::string enemy3Template_;
            std::string enemy4Template_;
            float width_;
            float height_;
            float sectionLength_;
            float platformSpeed_;
            float cameraOffset_;
    };
}

#endif /* _JumpCenterpoint_H__ */
