#ifndef _Fighter_H__
#define _Fighter_H__

#include <OIS/OIS.h>

#include "../OrxonoxPrereqs.h"

#include "Model.h"

class TiXmlElement; // Forward declaration

namespace orxonox
{
    class _OrxonoxExport Fighter : public Model, public OIS::MouseListener
    {
        public:
            Fighter();
            ~Fighter();
            void setConfigValues();
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
            bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);// { return true; }
            bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);// { return true; }


        private:
            bool bInvertMouse_;
            bool setMouseEventCallback_;

            ParticleInterface *w;
            ParticleInterface *tt;

            AmmunitionDump* ammoDump_;
            BarrelGun* mainWeapon_;

            bool leftButtonPressed_;
            bool rightButtonPressed_;

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
    ExportClass(Fighter, Orxonox);
}

#endif /* _Fighter_H__ */
