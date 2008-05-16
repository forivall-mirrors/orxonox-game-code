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
#include "core/InputInterfaces.h"
#include "Camera.h"
#include "Model.h"
#include "../tools/BillboardSet.h"

namespace orxonox
{
    class _OrxonoxExport SpaceShip : public Model, public MouseHandler
    {
        public:
            SpaceShip();
            ~SpaceShip();
            bool create();
            void registerAllVariables();
            void init();
            void setConfigValues();
            virtual void loadParams(TiXmlElement* xmlElem);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

            void setCamera(const std::string& camera = "");
            void setMaxSpeed(float value);
            void setMaxSideAndBackSpeed(float value);
            void setMaxRotation(float value);
            void setTransAcc(float value);
            void setRotAcc(float value);
            void setTransDamp(float value);
            void setRotDamp(float value);

            void getFocus();
	    static SpaceShip* instance_s;
	    static Vector3 getSPosition();
	    static Quaternion getSOrientation();
	    static std::string whereAmI();
            static void setMaxSpeedTest(float value)
                { SpaceShip::instance_s->setMaxSpeed(value); }

            bool mouseButtonPressed (const MouseState& state, MouseButton::Enum id);
            bool mouseButtonReleased(const MouseState& state, MouseButton::Enum id);
            bool mouseButtonHeld    (const MouseState& state, MouseButton::Enum id) { return true; }
            bool mouseMoved         (const MouseState& state);
            bool mouseScrolled      (const MouseState& state) { return true; }


        private:
            void createCamera();

            Vector3 testvector_;
            bool bInvertYAxis_;
            bool setMouseEventCallback_;
            bool bLMousePressed_;
            bool bRMousePressed_;

            Ogre::SceneNode* camNode_;
            Camera* cam_;
            std::string camName_;


            ParticleInterface* tt_;

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

            float emitterRate_;
            bool server_;
    };
}

#endif /* _SpaceShip_H__ */
