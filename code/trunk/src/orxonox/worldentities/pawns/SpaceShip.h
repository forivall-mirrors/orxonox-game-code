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
#include "util/OrxAssert.h"

#include "Pawn.h"

namespace orxonox
{

    /**
    @brief
        The SpaceShip is the principal entity through which the player interacts with the game. Its main function is to fly, however many things, such as @ref orxonox::Engine Engines or @ref orxonox::Weapon Weapons, can be attached to it.

        There are several parameters that define the behavior of the SpaceShip>
        - The <b>rotationThrust</b>, specifies the force with which the SpaceShip rotates.
        - The <b>boost</b>, there are quite some parameters pertaining to boosting. The boost is a special move of the SpaceShip, where, for a limited amount of time, it can fly considerably faster than usual. The <b>boostPower</b> is the amount of power available for boosting. The <b>boostPowerRate</b> is the rate at which the boost power is replenished. The <b>boostRate</b> is the rate at which boosting uses power. And the <b>boostCooldownDuration</b> is the time the SpaceShip cannot boost, once all the boost power has been used up. Naturally all of these parameters must be non-negative.
        - The <b>boost shaking</b>, when the SpaceShip boosts, the camera shakes to create a more immersive effect. Two parameters can be used to adjust the effect. The <b>shakeFrequency</b> is the frequency with which the camera shakes. And the <b>shakeAmplitude</b> is the amount with which the camera shakes. Again these parameters must bee non-negative.
        - The <b>lift</b> creates a more natural flight feeling through the addition of a lift force. There are again tow parameters that can be specified. The <b>lift</b> which is the lift force that is applied. And the <b>stallSpeed</b> which is the forward speed after which no more lift is generated.

        As mentioned @ref orxonox::Engine Engines can be mounted on the SpaceShip. Here is a (primitive) example of a SpaceShip defined in XML:
        @code
        <SpaceShip
            rotationThrust    = 50

            lift = 1;
            stallSpeed = 220;

            boostPower            = 15
            boostPowerRate        = 1
            boostRate             = 5
            boostCooldownDuration = 10

            shakeFrequency = 15
            shakeAmplitude = 9

            collisionType     = "dynamic"
            mass              = 100
            linearDamping     = 0.7
            angularDamping    = 0.9999999
            >
                <engines>
                    <Engine />
                    <Engine />
                </engines>
            </SpaceShip>
        @endcode

    @author
        Fabian 'x3n' Landau
    */
    class _OrxonoxExport SpaceShip : public Pawn
    {
        public:
            SpaceShip(Context* context);
            virtual ~SpaceShip();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            void setConfigValues();

            /**
            @brief Move forward or backward,
            @param value A vector whose first component specifies the amount of movement. Positive means forward, negative means backward.
            */
            virtual void moveFrontBack(const Vector2& value)
                { this->steering_.z -= value.x; }
            /**
            @brief Move right or left.
            @param value A vector whose first component specifies the amount of movement. Positive means right, negative means left.
            */
            virtual void moveRightLeft(const Vector2& value)
                { this->steering_.x += value.x; }
            /**
            @brief Move up or down.
            @param value A vector whose first component specifies the amount of movement. Positive means up, negative means down.
            */
            virtual void moveUpDown(const Vector2& value)
                { this->steering_.y += value.x; }

            virtual void rotateYaw(const Vector2& value); // Rotate in yaw direction.
            virtual void rotatePitch(const Vector2& value); // Rotate in pitch direction.
            virtual void rotateRoll(const Vector2& value); // Rotate in roll direction.

            virtual void fire();
            virtual void boost(bool bBoost); // Starts or stops boosting.

            void addEngine(Engine* engine); // Add an Engine to the SpaceShip.
            bool hasEngine(Engine* engine) const; // Check whether the SpaceShip has a particular Engine.
            Engine* getEngine(unsigned int i); // Get the i-th Engine of the SpaceShip.
            Engine* getEngineByName(const std::string& name);
            /**
            @brief Get the list of all Engines that are mounted on the SpaceShip.
            @return Returns a vector of all Engines of the SpaceShip.
            */
            inline const std::vector<Engine*>& getEngineList() const
                { return this->engineList_; }
            void removeEngine(Engine* engine); // Remove and destroy all Engines of the SpaceShip.
            void removeAllEngines(); // Remove a particular Engine from the SpaceShip.

            void addSpeedFactor(float factor); // Add to the speed factor for all engines of the SpaceShip.
            void addSpeed(float speed); // Add to the speed of all engines of the SpaceShip.
            float getSpeedFactor() const; // Get the mean speed factor over all engines of the SpaceShip.
    
            float getMaxSpeedFront() const; // Get the largest maximal forward speed over all engines of the SpaceShip.
            float getBoostFactor() const; // Get the mean boost factor over all engines of the SpaceShip.

            /**
            @brief Set the steering direction of the SpaceShip.
                   This is set through the user input.
            @param direction The direction the SpaceShip should steer in.
            */
            inline void setSteeringDirection(const Vector3& direction)
                { this->steering_ = direction; }
            /**
            @brief Get the steering direction of the SpaceShip.
            @return Returns the steering direction of the SpaceShip. The length of the vector is the amount of steering needed.
            */
            inline const Vector3& getSteeringDirection() const
                { return this->steering_; }

            /**
            @brief Check whether the SpaceShip is currently boosting.
            @return Returns true if the SpaceShip is boosting, false if not.
            */
            inline bool isBoosting() const
                { return this->bBoost_; }
            /**
            @brief Check whether the SpaceShip boost is cooling down.
            @return Returns true if the SpaceShip is cooling down from boosting.
            */
            inline bool isBoostCoolingDown() const
                { return bBoostCooldown_; }

            /**
            @brief Set the initial power available for boosting to the input value.
                   The current boost power is set to the input value as well.
            @param power The initial boost power. Must be non-negative.
            */
            inline void setInitialBoostPower(float power)
                { OrxAssert(power >= 0.0f, "The boost power must be non-negative."); this->initialBoostPower_ = power; this->boostPower_ = power; }
            /**
            @brief Set the rate, at which boost power is recharged, to the input value.
            @param rate The boost power rate in units per second. Must be non-negative.
            */
            inline void setBoostPowerRate(float rate)
                { OrxAssert(rate >= 0.0f, "The boost power rate must be non-negative."); this->boostPowerRate_ = rate; }
            /**
            @brief Set the rate, at which boost power us used up, to the input value.
            @param rate The boost rate in units per second. Must be non-negative.
            */
            inline void setBoostRate(float rate)
                { OrxAssert(rate >= 0.0f, "The boost rate must be non-negative."); this->boostRate_ = rate; }
            /**
            @brief Set the duration for which boosting, if in cooldown, is not possible.
                   Cooldown is reached if all boost power is depleted.
            @param duration The cooldown duration in seconds. Must be non-negative.
            */
            inline void setBoostCooldownDuration(float duration)
                { OrxAssert(duration >= 0.0f, "The boost cooldown duration must be non-negative."); this->boostCooldownDuration_ = duration; }
            /**
            @brief Set the frequency with which the camera shakes during boosting.
            @param frequency The frequency in times per second. Must be non-negative.
            */
            inline void setShakeFrequency(float frequency)
                { OrxAssert(frequency >= 0.0f, "The shake frequency must be non-negative."); this->shakeFrequency_ = frequency; }
            /**
            @brief Set the amplitude with which the camera shakes during boosting.
            @param amplitude The amplitude. Must be non-negative.
            */
            inline void setShakeAmplitude(float amplitude)
                { OrxAssert(amplitude >= 0.0f, "The shake amplitude must be non-negative."); this->shakeAmplitude_ = amplitude; }

            /**
            @brief Get the initial boost power. Is non-negative.
            @return Returns the initial boost power.
            */
            inline float getInitialBoostPower() const
                { return this->initialBoostPower_; }
            /**
            @brief Get the current boost power. Is non-negative.
            @return Returns the current boost power.
            */
            inline float getBoostPower() const
                { return this->boostPower_; }
            /**
            @brief Get the boost power rate.
            @return Returns the boost power rate in units per second. Is non-negative.
            */
            inline float getBoostPowerRate() const
                { return this->boostPowerRate_; }
            /**
            @brief Get the boost rate.
            @return Returns the boost rate in units per second. Is non-negative.
            */
            inline float getBoostRate() const
                { return this->boostRate_; }
            /**
            @brief Get the cooldown duration.
            @return Returns the cooldown duration in seconds. Is non-negative.
            */
            inline float getBoostCooldownDuration() const
                { return this->boostCooldownDuration_; }
            /**
            @brief Get the shake frequency.
            @return Returns the shake frequency in times per seconds. Is non-negative.
            */
            inline float getShakeFrequency() const
                { return this->shakeFrequency_; }
            /**
            @brief Get the shake amplitude.
            @return Returns the shake amplitude. Is non-negative.
            */
            inline float getShakeAmplitude() const
                { return this->shakeAmplitude_; }

        protected:
            bool bInvertYAxis_;

            Vector3 steering_; //!< The direction and magnitude of the steering action given through user input.

            float rotationThrust_;               //!< Force with which the SpaceShip rotates.
            btVector3 localAngularAcceleration_; //!< The acceleration that accounts for angular movement and is used internally.

            bool bBoost_;                 //!< Whether the SpaceShip is currently boosting.
            bool bBoostCooldown_;         //!< Whether the SpaceShip is currently in boost cooldown, during which boosting is impossible.
            float initialBoostPower_;     //!< The initial (and maximal) boost power.
            float boostPower_;            //!< The current boost power.
            float boostPowerRate_;        //!< The rate at which the boost power is recharged.
            float boostRate_;             //!< The rate at which boost power is used up.
            float boostCooldownDuration_; //!< The duration for which boost cooldown is in effect.
            float shakeFrequency_;        //!< The frequency of the shaking of the camera due to boosting.
            float shakeAmplitude_;        //!< The amplitude of the shaking of the camera due to boosting.

            float lift_;       //!< The amount of lift that is added.
            float stallSpeed_; //!< The forward speed where no more lift is added.

        private:
            void registerVariables();
            virtual bool isCollisionTypeLegal(WorldEntity::CollisionType type) const;

            void changedEnableMotionBlur(); // Is called when the enableMotionBlur config value has changed.
            /**
            @brief Callback function. Is called when the boost has cooled down.
            */
            void boostCooledDown(void)
                { this->bBoostCooldown_ = false; }

            void shakeCamera(float dt); // Shake the camera for a given time interval.
            void backupCamera(); // Save the original position and orientation of the camera.
            void resetCamera(); // Reset the camera to its original position.

            std::vector<Engine*> engineList_; //!< The list of all Engines mounted on this SpaceShip.

            Timer timer_;                          //!< Timer for the cooldown duration.
            float shakeDt_;                        //!< Temporary variable for the shaking of the camera.
            Vector3 cameraOriginalPosition_;       //!< The original position of the camera before shaking it.
            Quaternion cameraOriginalOrientation_; //!< The original orientation of the camera before shaking it.

            Shader* boostBlur_;      //!< A radial blur shader, applied when boosting according to the amount of boosting.
            float blurStrength_;     //!< The strength of the applied blur.
            bool bEnableMotionBlur_; //!< Whether motion blur is enabled or not.
        
    };
}

#endif /* _SpaceShip_H__ */
