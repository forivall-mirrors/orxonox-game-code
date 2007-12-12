#ifndef _WorldEntity_H__
#define _WorldEntity_H__

#include "BaseObject.h"
#include "Tickable.h"
#include "../core/CoreIncludes.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"

namespace orxonox
{
    class WorldEntity : public BaseObject, public Tickable
    {
        public:
            WorldEntity();
            ~WorldEntity();

            void tick(float dt);

            inline Ogre::SceneNode* getNode()
                { return this->node_; }

            inline void setPosition(const Vector3& pos)
                { this->node_->setPosition(pos); }
            inline void setPosition(Real x, Real y, Real z)
                { this->node_->setPosition(x, y, z); }
            inline const Vector3& getPosition() const
                { return this->node_->getPosition(); }

            inline void translate(const Vector3 &d, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(d, relativeTo); }
            inline void translate(Real x, Real y, Real z, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(x, y, z, relativeTo); }
            inline void translate(const Matrix3 &axes, const Vector3 &move, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(axes, move, relativeTo); }
            inline void translate(const Matrix3 &axes, Real x, Real y, Real z, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_PARENT)
                { this->node_->translate(axes, x, y, z, relativeTo); }

            inline void yaw(const Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
                { this->node_->yaw(angle, relativeTo); }
            inline void pitch(const Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
                { this->node_->pitch(angle, relativeTo); }
            inline void roll(const Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
                { this->node_->roll(angle, relativeTo); }

            inline void rotate(const Vector3 &axis, const Radian &angle, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL)
                { this->node_->rotate(axis, angle, relativeTo); }
            inline void setDirection(Real x, Real y, Real z, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL, const Vector3 &localDirectionVector=Vector3::NEGATIVE_UNIT_Z)
                { this->node_->setDirection(x, y, z, relativeTo, localDirectionVector); }
            inline void setDirection(const Vector3 &vec, Ogre::Node::TransformSpace relativeTo=Ogre::Node::TS_LOCAL, const Vector3 &localDirectionVector=Vector3::NEGATIVE_UNIT_Z)
                { this->node_->setDirection(vec, relativeTo, localDirectionVector); }
            inline void lookAt(const Vector3 &targetPoint, Ogre::Node::TransformSpace relativeTo, const Vector3 &localDirectionVector=Vector3::NEGATIVE_UNIT_Z)
                { this->node_->lookAt(targetPoint, relativeTo, localDirectionVector); }

            inline void attachObject(Ogre::MovableObject *obj)
                { this->node_->attachObject(obj); }
            inline void detachObject(Ogre::MovableObject *obj)
                { this->node_->detachObject(obj); }
            inline void detachAllObjects()
                { this->node_->detachAllObjects(); }

            static Ogre::SceneManager* sceneManager_s;
            static int num_s;

        private:
            Ogre::SceneNode* node_;
            static unsigned int worldEntityCounter_s;

            bool bStatic_;
            Vector3 velocity_;
    };
}

#endif
