#ifndef _WorldEntity_H__
#define _WorldEntity_H__

#include "BaseObject.h"
#include "Tickable.h"
#include "../core/CoreIncludes.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "Mesh.h"
#include "network/Synchronisable.h"
#include "tinyxml/tinyxml.h"

namespace orxonox
{
  class WorldEntity : public BaseObject, public Tickable, public network::Synchronisable
  {
    public:
      WorldEntity();
      ~WorldEntity();

      virtual void tick(float dt);
      virtual void loadParams(TiXmlElement* xmlElem);

      inline Ogre::SceneNode* getNode()
          { return this->node_; }

      inline void setNode(Ogre::SceneNode* node)
          { this->node_ = node; }

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
      inline void setOrientation(const Ogre::Quaternion quat)
        { this->node_->setOrientation(quat); }
      inline void lookAt(const Vector3 &targetPoint, Ogre::Node::TransformSpace relativeTo, const Vector3 &localDirectionVector=Vector3::NEGATIVE_UNIT_Z)
        { this->node_->lookAt(targetPoint, relativeTo, localDirectionVector); }

      inline void setScale(const Vector3 &scale)
        { this->node_->setScale(scale); }
      inline void setScale(Real x, Real y, Real z)
        { this->node_->setScale(x, y, z); }
      inline void setScale(Real scale)
        { this->node_->setScale(scale, scale, scale); }
      inline const Vector3& getScale(void) const
        { return this->node_->getScale(); }
      inline void scale(const Vector3 &scale)
        { this->node_->scale(scale); }
      inline void scale(Real x, Real y, Real z)
        { this->node_->scale(x, y, z); }
      inline void scale(Real scale)
        { this->node_->scale(scale, scale, scale); }

      inline void attachObject(Ogre::MovableObject *obj)
        { std::cout << "gux_1" << this->node_ << "\n"; this->node_->attachObject(obj); std::cout << "gux_2\n"; }
      inline void attachObject(Mesh &mesh)
        { std::cout << "gux_3" << this->node_ << "\n"; this->node_->attachObject((Ogre::MovableObject*)(mesh.getEntity())); std::cout << "gux_4\n"; }
      inline void detachObject(Ogre::MovableObject *obj)
        { this->node_->detachObject(obj); }
      inline void detachAllObjects()
        { this->node_->detachAllObjects(); }

      inline void setVelocity(const Vector3& velocity)
          { this->velocity_ = velocity; }
      inline void setVelocity(Real x, Real y, Real z)
          { this->velocity_.x = x; this->velocity_.y = y; this->velocity_.z = z; }
      inline const Vector3& getVelocity() const
          { return this->velocity_; }

      inline void setAcceleration(const Vector3& acceleration)
          { this->acceleration_ = acceleration; }
      inline void setAcceleration(Real x, Real y, Real z)
          { this->acceleration_.x = x; this->acceleration_.y = y; this->acceleration_.z = z; }
      inline const Vector3& getAcceleration() const
          { return this->acceleration_; }

      inline void setRotationAxis(const Vector3& axis)
          { this->rotationAxis_ = axis; }
      inline void setRotationAxis(Real x, Real y, Real z)
          { this->rotationAxis_.x = x; this->rotationAxis_.y = y; this->rotationAxis_.z = z; }
      inline const Vector3& getRotationAxis() const
          { return this->rotationAxis_; }

      inline void setRotationRate(const Radian& angle)
          { this->rotationRate_ = angle; }
      inline void setRotationRate(const Degree& angle)
          { this->rotationRate_ = angle; }
      inline const Radian& getRotationRate() const
          { return this->rotationRate_; }

      inline void setMomentum(const Radian& angle)
          { this->momentum_ = angle; }
      inline void setMomentum(const Degree& angle)
          { this->momentum_ = angle; }
      inline const Radian& getMomentum() const
          { return this->momentum_; }
      inline const Ogre::Quaternion& getOrientation()
          { return this->node_->getOrientation(); }

    protected:
      void registerAllVariables();

    private:
      Ogre::SceneNode* node_;
      static unsigned int worldEntityCounter_s;

      bool bStatic_;
      Vector3 velocity_;
      Vector3 acceleration_;
      Vector3 rotationAxis_;
      Radian rotationRate_;
      Radian momentum_;
  };
}

#endif
