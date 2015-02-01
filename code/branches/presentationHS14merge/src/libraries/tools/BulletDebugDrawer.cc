/**
 * Originally from http://www.ogre3d.org/tikiwiki/BulletDebugDrawer&structure=Cookbook
 * This source code is released into the Public Domain.
 *
 * Modified by Fabian 'x3n' Landau by using DebugDrawer and Orxonox specific utilities (e.g. output).
 */

#include "BulletDebugDrawer.h"
#include "OgreBulletUtils.h"

#include <OgreRoot.h>
#include <OgreManualObject.h>
#include <OgreSceneManager.h>

#include "util/Output.h"
#include "DebugDrawer.h"

namespace orxonox
{
    BulletDebugDrawer::BulletDebugDrawer(Ogre::SceneManager* sceneManager)
    {
        this->drawer_ = new DebugDrawer(sceneManager, 0.5f);
        this->bFill_ = true;

        mContactPoints = &mContactPoints1;

        mDebugMode = (DebugDrawModes) DBG_DrawWireframe;
        Ogre::Root::getSingleton().addFrameListener(this);
    }

    BulletDebugDrawer::~BulletDebugDrawer()
    {
        Ogre::Root::getSingleton().removeFrameListener(this);
        delete this->drawer_;
    }

    void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
    {
        this->drawer_->drawLine(vector3(from), vector3(to), colour(color, 1.0f));
    }

//    void BulletDebugDrawer::drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3& color, btScalar alpha)
//    {
//        // TODO
//    }

    void BulletDebugDrawer::drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
    {
        this->drawer_->drawSphere(vector3(p), Ogre::Quaternion::IDENTITY, radius, colour(color, 1.0f), this->bFill_);
    }

    void BulletDebugDrawer::drawSphere(btScalar radius, const btTransform& transform, const btVector3& color)
    {
        Ogre::Matrix4 matrix = matrix4(transform);
        this->drawer_->drawSphere(matrix.getTrans(), matrix.extractQuaternion(), radius, colour(color, 1.0f), this->bFill_);
    }

    void BulletDebugDrawer::drawBox(const btVector3& bbMin, const btVector3& bbMax, const btVector3& color)
    {
        Ogre::Vector3* corners = new Ogre::Vector3[8];
        corners[0]  = Ogre::Vector3(bbMin[0], bbMin[1], bbMin[2]);
        corners[1]  = Ogre::Vector3(bbMin[0], bbMax[1], bbMin[2]);
        corners[2]  = Ogre::Vector3(bbMax[0], bbMax[1], bbMin[2]);
        corners[3]  = Ogre::Vector3(bbMax[0], bbMin[1], bbMin[2]);
        corners[4]  = Ogre::Vector3(bbMax[0], bbMax[1], bbMax[2]);
        corners[5]  = Ogre::Vector3(bbMin[0], bbMax[1], bbMax[2]);
        corners[6]  = Ogre::Vector3(bbMin[0], bbMin[1], bbMax[2]);
        corners[7]  = Ogre::Vector3(bbMax[0], bbMin[1], bbMax[2]);
        this->drawer_->drawCuboid(corners, colour(color, 1.0f), this->bFill_);
    }

    void BulletDebugDrawer::drawBox(const btVector3& bbMin, const btVector3& bbMax, const btTransform& trans, const btVector3& color)
    {
        Ogre::Vector3* corners = new Ogre::Vector3[8];
        corners[0]  = Ogre::Vector3(trans * btVector3(bbMin[0], bbMin[1], bbMin[2]));
        corners[1]  = Ogre::Vector3(trans * btVector3(bbMin[0], bbMax[1], bbMin[2]));
        corners[2]  = Ogre::Vector3(trans * btVector3(bbMax[0], bbMax[1], bbMin[2]));
        corners[3]  = Ogre::Vector3(trans * btVector3(bbMax[0], bbMin[1], bbMin[2]));
        corners[4]  = Ogre::Vector3(trans * btVector3(bbMax[0], bbMax[1], bbMax[2]));
        corners[5]  = Ogre::Vector3(trans * btVector3(bbMin[0], bbMax[1], bbMax[2]));
        corners[6]  = Ogre::Vector3(trans * btVector3(bbMin[0], bbMin[1], bbMax[2]));
        corners[7]  = Ogre::Vector3(trans * btVector3(bbMax[0], bbMin[1], bbMax[2]));
        this->drawer_->drawCuboid(corners, colour(color, 1.0f), this->bFill_);
    }

    void BulletDebugDrawer::drawCylinder(btScalar radius, btScalar halfHeight, int upAxis, const btTransform& transform, const btVector3& color)
    {
        Ogre::Matrix4 matrix = matrix4(transform);
        this->drawer_->drawCylinder(matrix.getTrans(), matrix.extractQuaternion(), radius, halfHeight * 2, colour(color, 1.0f), this->bFill_);
    }

    void BulletDebugDrawer::drawCone(btScalar radius, btScalar height, int upAxis, const btTransform& transform, const btVector3& color)
    {
        Ogre::Matrix4 matrix = matrix4(transform);
        this->drawer_->drawCone(matrix.getTrans(), matrix.extractQuaternion(), radius, height, colour(color, 1.0f), this->bFill_);
    }

    void BulletDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
    {
        mContactPoints->resize(mContactPoints->size() + 1);
        ContactPoint p = mContactPoints->back();
        p.from = vector3(PointOnB);
        p.to = p.from + vector3(normalOnB) * distance;
        p.dieTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds() + lifeTime;
        p.color.r = color.x();
        p.color.g = color.y();
        p.color.b = color.z();
    }

    bool BulletDebugDrawer::frameStarted(const Ogre::FrameEvent& evt)
    {
        size_t now = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
        std::vector<ContactPoint>* newCP = mContactPoints == &mContactPoints1 ? &mContactPoints2 : &mContactPoints1;
        for (std::vector<ContactPoint>::iterator i = mContactPoints->begin(); i < mContactPoints->end(); i++ )
        {
            ContactPoint& cp = *i;
            this->drawer_->drawLine(cp.from, cp.to, cp.color);
            if (now <= cp.dieTime)
                newCP->push_back(cp);
        }
        mContactPoints->clear();
        mContactPoints = newCP;

        // Right before the frame is rendered, call DebugDrawer::build().
        this->drawer_->build();

        return true;
    }

    bool BulletDebugDrawer::frameEnded(const Ogre::FrameEvent& evt)
    {
        // After the frame is rendered, call DebugDrawer::clear()
        this->drawer_->clear();

        return true;
    }

    void BulletDebugDrawer::reportErrorWarning(const char* warningString)
    {
        orxout(internal_error) << warningString << endl;
        Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(warningString);
    }

    void BulletDebugDrawer::draw3dText(const btVector3& location, const char* textString)
    {

    }

    void BulletDebugDrawer::setDebugMode(int debugMode)
    {
        mDebugMode = (DebugDrawModes) debugMode;
    }

    int BulletDebugDrawer::getDebugMode() const
    {
        return mDebugMode;
    }

    void BulletDebugDrawer::configure(bool bFill, float fillAlpha)
    {
        this->bFill_ = bFill;
        this->drawer_->setFillAlpha(fillAlpha);
    }
}
