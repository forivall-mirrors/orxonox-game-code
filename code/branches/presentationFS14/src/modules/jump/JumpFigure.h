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

#ifndef _JumpFigure_H__
#define _JumpFigure_H__

#include "jump/JumpPrereqs.h"

#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    class _JumpExport JumpFigure : public ControllableEntity
    {
        public:
            JumpFigure(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~JumpFigure() {}
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            virtual void moveFrontBack(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
            virtual void moveRightLeft(const Vector2& value); //!< Overloaded the function to steer the bat up and down.
            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);
            void fire(unsigned int firemode);
            virtual void fired(unsigned int firemode);
            virtual void JumpFromPlatform(JumpPlatform* platform);
            virtual void JumpFromSpring(JumpSpring* spring);
            virtual void CollisionWithEnemy(JumpEnemy* enemy);
            virtual bool StartRocket(JumpRocket* rocket);
            virtual void StopRocket(JumpRocket* rocket);
            virtual bool StartPropeller(JumpPropeller* propeller);
            virtual void StopPropeller(JumpPropeller* propeller);
            virtual bool StartBoots(JumpBoots* boots);
            virtual void StopBoots(JumpBoots* boots);
            virtual bool StartShield(JumpShield* shield);
            virtual void StopShield(JumpShield* shield);
            virtual void InitializeAnimation(Context* context);

            void setFieldDimension(float width, float height)
                { fieldWidth_ = width; fieldHeight_ = height; }
            void setFieldDimension(const Vector2& dimension)
                { setFieldDimension(dimension.x, dimension.y); }
            Vector2 getFieldDimension() const
                { return Vector2(fieldWidth_, fieldHeight_); }
            void setMouseFactor(const float mouseFactor)
                { mouseFactor_ = mouseFactor; }
            const float getMouseFactor() const
                { return mouseFactor_; }
            void setModelLeftHand(const std::string& modelLeftHand)
                { modelLeftHand_ = modelLeftHand; }
            const std::string& getModelLeftHand() const
                { return modelLeftHand_; }
            void setModelRightHand(const std::string& modelRightHand)
                { modelRightHand_ = modelRightHand; }
            const std::string& getModelRightHand() const
                { return modelRightHand_; }
            void setRocketPos(const float rocketPos)
                { rocketPos_ = rocketPos; }
            const float getRocketPos() const
                { return rocketPos_; }
            void setPropellerPos(const float propellerPos)
                { propellerPos_ = propellerPos; }
            const float getPropellerPos() const
                { return propellerPos_; }
            void setBootsPos(const float bootsPos)
                { bootsPos_ = bootsPos; }
            const float getBootsPos() const
                { return bootsPos_; }
            void setJumpSpeed(const float jumpSpeed)
                { jumpSpeed_ = jumpSpeed; }
            const float getJumpSpeed() const
                { return jumpSpeed_; }
            void setRocketSpeed(const float rocketSpeed)
                { rocketSpeed_ = rocketSpeed; }
            const float getRocketSpeed() const
                { return rocketSpeed_; }
            void setPropellerSpeed(const float propellerSpeed)
                { propellerSpeed_ = propellerSpeed; }
            const float getPropellerSpeed() const
                { return propellerSpeed_; }
            void setHandMinAngle(const float handMinAngle)
                { handMinAngle_ = handMinAngle; }
            const float getHandMinAngle() const
                { return handMinAngle_; }
            void setHandMaxAngle(const float handMaxAngle)
                { handMaxAngle_ = handMaxAngle; }
            const float getHandMaxAngle() const
                { return handMaxAngle_; }
            void setHandSpeed(const float handSpeed)
                { handSpeed_ = handSpeed; }
            const float getHandSpeed() const
                { return handSpeed_; }

            bool fireSignal_;
            bool rocketActive_;
            bool propellerActive_;
            bool dead_;
        private:
            std::string modelLeftHand_;
            std::string modelRightHand_;
            Model* leftHand_;
            Model* rightHand_;
            float fieldWidth_;
            float fieldHeight_;
            float timeSinceLastFire_;
            bool moveUpPressed_;
            bool moveDownPressed_;
            bool moveLeftPressed_;
            bool moveRightPressed_;
            bool firePressed_;
            float gravityAcceleration_;
            float mouseFactor_;
            float jumpSpeed_;
            float handSpeed_;
            float handMaxAngle_;
            float handMinAngle_;
            float rocketPos_;
            float propellerPos_;
            float bootsPos_;
            float maxFireRate_;
            float horizontalSpeed_;
            float handAngle_;
            bool animateHands_;
            bool turnUp_;
            bool bootsActive_;
            bool shieldActive_;
            float rocketSpeed_;
            float propellerSpeed_;
    };
}

#endif /* _JumpFigure_H__ */
