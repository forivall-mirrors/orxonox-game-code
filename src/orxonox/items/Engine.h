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

#ifndef _Engine_H__
#define _Engine_H__

#include "OrxonoxPrereqs.h"

#include "util/OrxAssert.h"

#include "Item.h"

namespace orxonox
{

    /**
    @brief
        The Engine class provides propulsion to the SpaceShip.
        
        There are many parameters that can be specified:
        - The <b>relativePosition</b>, specifies the position relative to the center of the SpaceShip the Engine is mounted on.
        - The <b>maximal speed</b>, there are four maximal speeds that can be specified: The <b>speedfront</b>, the maximal forward speed. The <b>speedback>, the maximal backward speed. The <b>speedleftright</b>, the maximal speed in y-direction of the SpaceShip coordinate frame. The <b>speedupdown</b>, the maximal speed in z-direction of the SpaceShip coordinate frame. All maximal speeds (naturally) have to be non-negative.
        - The <b>acceleration</b>, there are five types of acceleration that can be specified: The <b>accelerationfront</b>, the forward acceleration. The <b>accelerationbrake</b>, the braking acceleration. The <b>accelerationback</b>, the backward acceleration. The <b>accelerationleftright</b>, the acceleration in y-direction. The <b>accelerationupdown</b>, the acceleration in z-direction. All accelerations have to be non-negative.
        - The <b>boostfactor</b>, specifies the factor by which boosting increases the speed. This has to be non-negative, as well. Beware that maximal speeds can be overcome through boosting.
        - The <b>template</b>, the name of the engine template. Allows for parameters of the Engine be set trough a template.
        
    @author
        Fabian 'x3n' Landau
    */
    class _OrxonoxExport Engine : public Item
    {
        public:
            Engine(Context* context);
            virtual ~Engine();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void setConfigValues();

            virtual void run(float dt); // Run the engine for a given time interval.

            virtual void addToSpaceShip(SpaceShip* ship); // Adds the Engine to the input SpaceShip.
            /**
            @brief Get the SpaceShip this Engine is mounted on.
            @return Returns a pointer to the SpaceShip. NULL if it isn't mounted on any ship.
            */
            inline SpaceShip* getShip() const
                { return this->ship_; }

            /**
            @brief Set the relative position of the Engine.
            @param position The relative position with respect to the SpaceShip it is mounted on.
            */
            inline void setRelativePosition(const Vector3 &position)
                { this->relativePosition_ = position; }
            /**
            @brief Get the relative position of the Engine.
            @return Returns the relative position with respect to the SpaceShip it is mounted on.
            */
            inline const Vector3& getRelativePosition() const
                { return this->relativePosition_; }

            /**
            @brief Set the maximal forward speed of the Engine.
            @param speed The speed to be set. Must be non-negative.
            */
            //TODO: Better OrxVerify()?
            inline void setMaxSpeedFront(float speed)
                { OrxAssert(speed >= 0.0f, "The input speed must be non-negative."); this->maxSpeedFront_ = speed; }
            /**
            @brief Set the maximal backward speed of the Engine.
            @param speed The speed to be set. Must be non-negative.
            */
            inline void setMaxSpeedBack(float speed)
                { OrxAssert(speed >= 0.0f, "The input speed must be non-negative."); this->maxSpeedBack_ = speed; }
            /**
            @brief Set the maximal left-right speed of the Engine.
            @param speed The speed to be set. Must be non-negative.
            */
            inline void setMaxSpeedLeftRight(float speed)
                { OrxAssert(speed >= 0.0f, "The input speed must be non-negative."); this->maxSpeedLeftRight_ = speed; }
            /**
            @brief Set the maximal up-down speed of the Engine.
            @param speed The speed to be set. Must be non-negative.
            */
            inline void setMaxSpeedUpDown(float speed)
                { OrxAssert(speed >= 0.0f, "The input speed must be non-negative."); this->maxSpeedUpDown_ = speed; }

            /**
            @brief Get the maximal forward speed of the Engine.
            @return Returns the maximal forward speed of the Engine. Is non-negative.
            */
            inline float getMaxSpeedFront() const
                { return this->maxSpeedFront_; }
            /**
            @brief Get the maximal backward speed of the Engine.
            @return Returns the maximal backward speed of the Engine. Is non-negative.
            */
            inline float getMaxSpeedBack() const
                { return this->maxSpeedBack_; }
            /**
            @brief Get the maximal left-right speed of the Engine.
            @return Returns the maximal left-right speed of the Engine. Is non-negative.
            */
            inline float getMaxSpeedLeftRight() const
                { return this->maxSpeedLeftRight_; }
            /**
            @brief Get the maximal up-down speed of the Engine.
            @return Returns the maximal up-down speed of the Engine. Is non-negative.
            */
            inline float getMaxSpeedUpDown() const
                { return this->maxSpeedUpDown_; }

            /**
            @brief Set the forward acceleration produced by the Engine.
            @param acceleration The forward acceleration produced by the Engine. Must be non-negative.
            */
            inline void setAccelerationFront(float acceleration)
                { OrxAssert(acceleration >= 0.0f, "The input acceleration must be non-negative."); this->accelerationFront_ = acceleration; }
            /**
            @brief Set the breaking acceleration produced by the Engine.
            @param acceleration The breaking acceleration produced by the engine. Must be non-negative.
            */
            inline void setAccelerationBrake(float acceleration)
                { OrxAssert(acceleration >= 0.0f, "The input acceleration must be non-negative."); this->accelerationBrake_ = acceleration; }
            /**
            @brief Set the backward acceleration produced by the Engine.
            @param acceleration The backward acceleration produced by the Engine.
            */
            inline void setAccelerationBack(float acceleration)
                { OrxAssert(acceleration >= 0.0f, "The input acceleration must be non-negative."); this->accelerationBack_ = acceleration; }
            /**
            @brief Set the left-right acceleration produced by the Engine.
            @param acceleration The left-right acceleration produced by the Engine.
            */
            inline void setAccelerationLeftRight(float acceleration)
                { OrxAssert(acceleration >= 0.0f, "The input acceleration must be non-negative."); this->accelerationLeftRight_ = acceleration; }
            /**
            @brief Set the up-down acceleration produced by the Engine.
            @param acceleration The
            */
            inline void setAccelerationUpDown(float acceleration)
                { OrxAssert(acceleration >= 0.0f, "The input acceleration must be non-negative."); this->accelerationUpDown_ = acceleration; }

            /**
            @brief Get the forward acceleration produced by the Engine.
            @return Returns the forward acceleration produced by the Engine. Is non-negative.
            */
            inline float getAccelerationFront() const
                { return this->accelerationFront_; }
            /**
            @brief Get the breaking acceleration produced by the Engine.
            @return Returns the breaking acceleration produced by the Engine. Is non-negative.
            */
            inline float getAccelerationBrake() const
                { return this->accelerationBrake_; }
            /**
            @brief Get the backward acceleration produced by the Engine.
            @return Returns the backward acceleration produced by the Engine. Is non-negative.
            */
            inline float getAccelerationBack() const
                { return this->accelerationBack_; }
            /**
            @brief Get the left-right acceleration produced by the Engine.
            @return Returns the left-right acceleration produced by the Engine. Is non-negative.
            */
            inline float getAccelerationLeftRight() const
                { return this->accelerationLeftRight_; }
            /**
            @brief Get the up-down acceleration produced by the Engine.
            @return Returns the up-down acceleration produced by the Engine. Is non-negative.
            */
            inline float getAccelerationUpDown() const
                { return this->accelerationUpDown_; }

            /**
            @brief Set the factor by which boosting increases the forward acceleration of the Engine.
            @param factor The boost factor. Needs to be positive.
            */
            inline void setBoostFactor(float factor)
                { OrxAssert(factor > 0.0f, "The input factor must be positive."); this->boostFactor_ = factor; }
            /**
            @brief Get the boost factor of the Engine.
            @return Returns the factor by which boosting increases the forward acceleration of the Engine. Is positive.
            */
            inline float getBoostFactor() const
                { return this->boostFactor_; }

            /**
            @brief Add to the additional forward speed factor.
            @param factor The speed that is added to the additional forward speed. Must be non-negative.
            */
            inline void addSpeedAdd(float speed)
                { this->speedAdd_ += speed; }
            /**
            @brief Add to the forward speed multiplication factor.
            @param factor The factor by which the forward speed multiplication factor is multiplied. Must be non-negative.
            */
            inline void addSpeedMultiply(float factor)
                { OrxAssert(factor >= 0.0f, "The factor must be non-negative."); this->speedMultiply_ *= factor; }

            /**
            @brief Get the additional forward speed.
            @return Returns the additional forward speed.
            */
            inline float getSpeedAdd(void)
                { return this->speedAdd_; }
            /**
            @brief Get the forward speed multiplication factor.
            @return Returns the forward speed multiplication factor.
            */
            inline float getSpeedMultiply(void)
                { return this->speedMultiply_; }

            /**
            @brief Set the engine template, that specifies the parameters for the Engine.
            @param temp The name of the engine template.
            */
            inline void setEngineTemplate(const std::string& temp)
                { this->engineTemplate_ = temp; this->loadEngineTemplate(); }
            /**
            @brief Get the engine template, that specifies the parameters for the Engine.
            @return Returns the name of the engine template.
            */
            inline const std::string& getEngineTemplate() const
                { return this->engineTemplate_; }

        protected:
            void loadEngineTemplate(); // Load the engine template.

            virtual const Vector3& getSteering() const; // Get the steering direction imposed upon the Engine.

        private:
            void registerVariables();
            void networkcallback_shipID();

            std::string engineTemplate_; //!< The template that specifies the Engine's parameters.

            SpaceShip* ship_; //!< A pointer to the SpaceShip the Engine is mounted on.
            unsigned int shipID_; //!< Object ID of the SpaceShip the Engine is mounted on.
            Vector3 relativePosition_; //!< The relative position of the Engine with respect to the SpaceShip it is mounted on.

            float boostFactor_; //!< The factor by which boosting increases the forward acceleration.

            float speedAdd_; //!< Additional forward speed. Is not bounded by the maximal forward speed.
            float speedMultiply_; //!< Forward speed multiplication factor. Is not bounded by the maximal forward speed.

            float maxSpeedFront_; //!< The maximal forward speed.
            float maxSpeedBack_; //!< The maximal backward speed.
            float maxSpeedLeftRight_; //!< The maximal left-right speed.
            float maxSpeedUpDown_; //!< The maximal up-down speed.

            float accelerationFront_; //!< Forward acceleration produced by the Engine.
            float accelerationBrake_; //!< Breaking acceleration produced by the Engine.
            float accelerationBack_; //!< Backward acceleration produced by the Engine.
            float accelerationLeftRight_; //!< Left-right acceleration produced by the Engine.
            float accelerationUpDown_; //!< Up-down acceleration produced by the Engine.

    };
}

#endif /* _Engine_H__ */
