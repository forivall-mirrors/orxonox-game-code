#ifndef _SpaceShip_H__
#define _SpaceShip_H__

#include <OgrePrerequisites.h>
#include <OIS/OISMouse.h>

#include "../OrxonoxPrereqs.h"

#include "Model.h"
#include "../tools/BillboardSet.h"

class TiXmlElement;          // Forward declaration

namespace orxonox
{
    class ParticleInterface; // Forward declaration

    class _OrxonoxExport SpaceShip : public Model, public OIS::MouseListener
    {
        public:
            SpaceShip();
            ~SpaceShip();
            void init();
            void setConfigValues();
            virtual void loadParams(TiXmlElement* xmlElem);
            virtual void XMLPort(Element& xmlelement, bool loading);
            virtual void tick(float dt);

            void setCamera(const std::string& camera = "");
            void setMaxSpeed(float value);
            void setMaxSideAndBackSpeed(float value);
            void setMaxRotation(float value);
            void setTransAcc(float value);
            void setRotAcc(float value);
            void setTransDamp(float value);
            void setRotDamp(float value);

            bool mouseMoved(const OIS::MouseEvent &e);
            bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
            bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);


        private:
            Vector3 testvector_;
            bool bInvertYAxis_;
            bool setMouseEventCallback_;
            bool bLMousePressed_;
            bool bRMousePressed_;

            Ogre::SceneNode* camNode_;

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
    };
}

#endif /* _SpaceShip_H__ */
