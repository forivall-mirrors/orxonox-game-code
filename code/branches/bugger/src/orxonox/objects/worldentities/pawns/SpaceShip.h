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
            void registerVariables();
            void setConfigValues();

            virtual void moveFrontBack(const Vector2& value);
            virtual void moveRightLeft(const Vector2& value);
            virtual void moveUpDown(const Vector2& value);

            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);

            virtual void fire();
            virtual void boost();

            void setEngine(Engine* engine);
            inline Engine* getEngine() const
                { return this->engine_; }

            void setMaxRotation(const Degree& value)
                { this->maxRotation_ = value; }
            void setRotAcc(const Degree& value)
                { this->rotationAcceleration_ = value; }
            void setTransDamp(float value)
                { this->translationDamping_ = value; }

            inline float getMaxRotation() const
                { return this->maxRotation_.valueDegrees(); }
            inline float getRotAcc() const
                { return this->rotationAcceleration_.valueDegrees(); }
            inline float getTransDamp() const
                { return this->translationDamping_; }

            inline void setSteeringDirection(const Vector3& direction)
                { this->steering_ = direction; }
            inline const Vector3& getSteeringDirection() const
                { return this->steering_; }

            inline void setBoost(bool bBoost)
                { this->bBoost_ = bBoost; }
            inline bool getBoost() const
                { return this->bBoost_; }

            inline void setEngineTemplate(const std::string& temp)
                { this->enginetemplate_ = temp; this->loadEngineTemplate(); }
            inline const std::string& getEngineTemplate() const
                { return this->enginetemplate_; }

        protected:
            bool bInvertYAxis_;

            bool bBoost_;
            Vector3 steering_;
            float translationDamping_;

            Degree maxRotation_;
            Degree rotationAcceleration_;

            Degree zeroDegree_;
            Degree pitchRotation_;
            Degree yawRotation_;
            Degree rollRotation_;

        private:
            void loadEngineTemplate();

            std::string enginetemplate_;
            Engine* engine_;
    };
}

#endif /* _SpaceShip_H__ */
