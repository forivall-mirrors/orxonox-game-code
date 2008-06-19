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
 *      Benjamin Knecht
 *
 */

#ifndef _SpaceShip_H__
#define _SpaceShip_H__
#include <string>
#include "OrxonoxPrereqs.h"
#include <OgrePrerequisites.h>

#include "util/Math.h"
#include "Camera.h"
#include "Model.h"
#include "RadarViewable.h"
#include "tools/BillboardSet.h"

namespace orxonox
{
    class _OrxonoxExport SpaceShip : public Model, public RadarViewable
    {
        public:
            static SpaceShip *getLocalShip();

            SpaceShip();
            ~SpaceShip();
            virtual bool create();
            void registerAllVariables();
            void init();
            void setConfigValues();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            virtual void changedVisibility();
            virtual void changedActivity();

            void setCamera(const std::string& camera = "");
            void setMaxSpeed(float value);
            void setMaxSideAndBackSpeed(float value);
            void setMaxRotation(float value);
            void setTransAcc(float value);
            void setRotAcc(float value);
            void setTransDamp(float value);
            void setRotDamp(float value);
            void getFocus();

            inline float getMaxSpeed() const
                { return this->maxSpeed_; }
            inline float getMaxSideAndBackSpeed() const
                { return this->maxSideAndBackSpeed_; }
            inline float getMaxRotation() const
                { return this->maxRotation_; }
            inline float getTransAcc() const
                { return this->translationAcceleration_; }
            inline float getRotAcc() const
                { return this->rotationAcceleration_; }
            inline float getTransDamp() const
                { return this->translationDamping_; }
            inline float getRotDamp() const
                { return this->rotationDamping_; }

            static std::string whereAmI();
            static void setMaxSpeedTest(float value)
                { SpaceShip::instance_s->setMaxSpeed(value); }

            static void movePitch(float value);
            static void moveYaw(float value);
            static void moveRoll(float value);
            static void moveLongitudinal(float value);
            static void moveLateral(float value);
            static void fire();
            void setMovePitch(float value);
            void setMoveYaw(float value);
            void setMoveRoll(float value);
            void setMoveLongitudinal(float value);
            void setMoveLateral(float value);
            void doFire();

            inline const Vector3& getDir() const
                { return this->currentDir_; }
            inline const Vector3& getInitialDir() const
                { return this->initialDir_; }
            inline const Vector3& getOrth() const
                { return this->currentOrth_; }
            inline const Vector3& getInitialOrth() const
                { return this->initialOrth_; }

            Camera* getCamera();

            int getTeamNr() const
                { return this->teamNr_; }
            float getHealth() const
                { return this->health_; }

            bool getMyShip(){return myShip_;}

        protected:
            void setTeamNr(int teamNr)
                { this->teamNr_ = teamNr; }

        private:
            void createCamera();
            virtual ColourValue getProjectileColour() const
                { return ColourValue(1.0, 1.0, 0.5); }

            Vector3 testvector_;
            Vector3 initialDir_;
            Vector3 currentDir_;
            Vector3 initialOrth_;
            Vector3 currentOrth_;
            bool bInvertYAxis_;
            bool setMouseEventCallback_;
            bool bLMousePressed_;
            bool bRMousePressed_;

            Ogre::SceneNode* camNode_;
            Camera* cam_;
            std::string camName_;

            ParticleInterface* tt1_;
            ParticleInterface* tt2_;

            BillboardSet redBillboard_;
            BillboardSet greenBillboard_;
            Ogre::SceneNode* redNode_;
            Ogre::SceneNode* greenNode_;
            float blinkTime_;

            BillboardSet crosshairNear_;
            BillboardSet crosshairFar_;
            Ogre::SceneNode* chNearNode_;
            Ogre::SceneNode* chFarNode_;

            float timeToReload_;
            float reloadTime_;

            float maxSideAndBackSpeed_;
            float maxSpeed_;
            float maxRotation_;
            float translationAcceleration_;
            float rotationAcceleration_;
            float translationDamping_;
            float rotationDamping_;

            Radian maxRotationRadian_;
            Radian rotationAccelerationRadian_;
            Radian rotationDampingRadian_;
            Radian zeroRadian_;
            Radian mouseXRotation_;
            Radian mouseYRotation_;

            float mouseX_;
            float mouseY_;

        protected:
            bool myShip_;

            int teamNr_;
            float health_;

            static SpaceShip* instance_s;
    };
}

#endif /* _SpaceShip_H__ */
