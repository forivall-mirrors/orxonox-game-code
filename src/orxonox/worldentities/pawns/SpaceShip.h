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

#ifndef _SpaceShip_H__
#define _SpaceShip_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <LinearMath/btVector3.h>
#include "tools/Timer.h"
#include "util/Math.h"
#include "Pawn.h"

namespace orxonox
{
    class _OrxonoxExport SpaceShip : public Pawn
    {
        public:
            SpaceShip(BaseObject* creator);
            virtual ~SpaceShip();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            void setConfigValues();

            virtual void moveFrontBack(const Vector2& value);
            virtual void moveRightLeft(const Vector2& value);
            virtual void moveUpDown(const Vector2& value);

            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);

            virtual void fire();
            virtual void boost(bool bBoost); // Starts or stops boosting.

            void addEngine(Engine* engine);
            bool hasEngine(Engine* engine);
            Engine* getEngine(unsigned int i); // This one's for XMLPort
            inline const std::vector<Engine*>& getEngineList()
                { return this->engineList_; }
            void removeEngine(Engine* engine);
            void removeAllEngines();

            void setSpeedFactor(float factor);
            float getSpeedFactor(); // Gets mean speed factor
            float getMaxSpeedFront(); // gets largest speed forward
            float getBoostFactor(); // gets mean boost factor

            inline void setSteeringDirection(const Vector3& direction)
                { this->steering_ = direction; }
            inline const Vector3& getSteeringDirection() const
                { return this->steering_; }
            inline void resetEngineTicks()
                { this->engineTicksNotDone = this->engineList_.size(); }
            inline void oneEngineTickDone()
                { this->engineTicksNotDone--; }
            inline bool hasEngineTicksRemaining()
                { return (this->engineTicksNotDone>0); }

            inline bool getBoost() const
                { return this->bBoost_; }

            inline float getBoostPower() const
                { return this->boostPower_; }
            inline float getInitialBoostPower() const
                { return this->initialBoostPower_; }

            inline bool isBoostCoolingDown() const
                { return bBoostCooldown_; }

        protected:
            virtual std::vector<PickupCarrier*>* getCarrierChildren(void) const;
            bool bInvertYAxis_;

            bool bBoost_;
            bool bBoostCooldown_;
            float boostPower_;
            float initialBoostPower_;
            float boostRate_;
            float boostPowerRate_;
            float boostCooldownDuration_;
            float lift_;
            float stallSpeed_;
            Vector3 steering_;
            float primaryThrust_;
            float auxilaryThrust_;
            float rotationThrust_;
            btVector3 localLinearAcceleration_;
            btVector3 localAngularAcceleration_;

            float shakeFrequency_;
            float shakeAmplitude_;

        private:
            void registerVariables();
            virtual bool isCollisionTypeLegal(WorldEntity::CollisionType type) const;
            
            //All things booster
            void changedEnableMotionBlur();
            void boostCooledDown(void);
        
            void resetCamera();
            void backupCamera();
            void shakeCamera(float dt);

            Shader* boostBlur_;
            float blurStrength_;
            bool bEnableMotionBlur_;

            std::vector<Engine*> engineList_;
            int engineTicksNotDone; // Used for knowing when to reset temporary variables.
            Timer timer_;
            Vector3 cameraOriginalPosition_;
            Quaternion cameraOriginalOrientation_;
        
            float shakeDt_;
    };
}

#endif /* _SpaceShip_H__ */
