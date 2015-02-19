/**
 * Originally from http://www.ogre3d.org/tikiwiki/BulletDebugDrawer&structure=Cookbook
 * This source code is released into the Public Domain.
 *
 * Modified by Fabian 'x3n' Landau by using DebugDrawer and Orxonox specific utilities (e.g. output).
 */

#ifndef _BulletDebugDrawer_H__
#define _BulletDebugDrawer_H__

#include "tools/ToolsPrereqs.h"

#include <btBulletCollisionCommon.h>
#include <OgreFrameListener.h>

namespace orxonox
{
    class _ToolsExport BulletDebugDrawer : public btIDebugDraw, public Ogre::FrameListener
    {
        public:
            BulletDebugDrawer(Ogre::SceneManager* sceneManager);
            ~BulletDebugDrawer();
            virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
//            virtual void drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3& color, btScalar);
            virtual void drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
            virtual void drawSphere(btScalar radius, const btTransform& transform, const btVector3& color);
            virtual void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color);
            virtual void drawBox(const btVector3& bbMin, const btVector3& bbMax, const btTransform& trans, const btVector3& color);
            virtual void drawCylinder(btScalar radius, btScalar halfHeight, int upAxis, const btTransform& transform, const btVector3& color);
            virtual void drawCone(btScalar radius, btScalar height, int upAxis, const btTransform& transform, const btVector3& color);
//            virtual void drawPlane(const btVector3& planeNormal, btScalar planeConst, const btTransform& transform, const btVector3& color);

            virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

            virtual void reportErrorWarning(const char* warningString);
            virtual void draw3dText(const btVector3& location, const char* textString);

            virtual void setDebugMode(int debugMode);
            virtual int getDebugMode() const;

            void configure(bool bFill, float fillAlpha);

        protected:
            bool frameStarted(const Ogre::FrameEvent& evt);
            bool frameEnded(const Ogre::FrameEvent& evt);

        private:
            struct ContactPoint
            {
                Ogre::Vector3 from;
                Ogre::Vector3 to;
                Ogre::ColourValue color;
                size_t dieTime;
            };

            bool bFill_;
            DebugDrawer* drawer_;

            DebugDrawModes mDebugMode;
            std::vector<ContactPoint>* mContactPoints;
            std::vector<ContactPoint> mContactPoints1;
            std::vector<ContactPoint> mContactPoints2;
    };
}

#endif /* _BulletDebugDrawer_H__ */
