#ifndef _SpaceShip_H__
#define _SpaceShip_H__

#include <OIS/OIS.h>

#include "Model.h"
#include "BillboardSet.h"
#include "OgreSceneNode.h"

#include "../../tinyxml/tinyxml.h"
#include "../particle/ParticleInterface.h"

namespace orxonox
{
    class SpaceShip : public Model, public OIS::MouseListener
    {
        public:
            SpaceShip();
            ~SpaceShip();
            virtual void loadParams(TiXmlElement* xmlElem);
            void setMaxSpeedValues(float maxSpeedForward, float maxSpeedRotateUpDown, float maxSpeedRotateRightLeft, float maxSpeedLoopRightLeft);
            virtual void tick(float dt);
            void moveForward(float moveForward);
            void rotateUp(float rotateUp);
            void rotateDown(float rotateDown);
            void rotateRight(float rotateRight);
            void rotateLeft(float rotateLeft);
            void loopRight(float loopRight);
            void loopLeft(float loopLeft);
            void brakeForward(float brakeForward);
            void brakeRotate(float brakeRotate);
            void brakeLoop(float brakeLoop);
            void maxSpeedForward(float maxSpeedForward);
            void maxSpeedRotateUpDown(float maxSpeedRotateUpDown);
            void maxSpeedRotateRightLeft(float maxSpeedRotateRightLeft);
            void maxSpeedLoopRightLeft(float maxSpeedLoopRightLeft);
            bool mouseMoved(const OIS::MouseEvent &e);
            bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return true; }
            bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) { return true; }


        private:
            bool bInvertMouse_;
            bool setMouseEventCallback_;

            particle::ParticleInterface *tt_;

            BillboardSet redBillboard_;
            BillboardSet greenBillboard_;
            Ogre::SceneNode* redNode_;
            Ogre::SceneNode* greenNode_;
            float blinkTime_;

            float moveForward_;
            float rotateUp_;
            float rotateDown_;
            float rotateRight_;
            float rotateLeft_;
            float loopRight_;
            float loopLeft_;
            float brakeForward_;
            float brakeRotate_;
            float brakeLoop_;

            float speedForward_;
            float speedRotateUpDown_;
            float speedRotateRightLeft_;
            float speedLoopRightLeft_;

            float maxSpeedForward_;
            float maxSpeedRotateUpDown_;
            float maxSpeedRotateRightLeft_;
            float maxSpeedLoopRightLeft_;

            float accelerationForward_;
            float accelerationRotateUpDown_;
            float accelerationRotateRightLeft_;
            float accelerationLoopRightLeft_;

            float speed;
            float rotate;
            float loop;
            float mouseY;
            float mouseX;
            float maxMouseX;
            float minMouseX;
            bool moved;

            int emitterRate_;
    };
}

#endif
