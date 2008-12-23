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

#ifndef _WorldEntity_H__
#define _WorldEntity_H__

#include "OrxonoxPrereqs.h"

#define OGRE_FORCE_ANGLE_TYPES

#include <OgreSceneNode.h>

#include "network/Synchronisable.h"
#include "core/BaseObject.h"
#include "util/Math.h"

namespace orxonox
{
    class _OrxonoxExport WorldEntity : public BaseObject, public Synchronisable
    {
        public:
            WorldEntity(BaseObject* creator);
            virtual ~WorldEntity();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            inline Ogre::SceneNode* getNode() const
                { return this->node_; }

            static const Vector3 FRONT;
            static const Vector3 BACK;
            static const Vector3 LEFT;
            static const Vector3 RIGHT;
            static const Vector3 DOWN;
            static const Vector3 UP;

            virtual void setPosition(const Vector3& position) = 0;
            inline void setPosition(float x, float y, float z)
                { this->setPosition(Vector3(x, y, z)); }
            inline const Vector3& getPosition() const
                { return this->node_->getPosition(); }
            inline const Vector3& getWorldPosition() const
                { return this->node_->getWorldPosition(); }

            virtual void translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL) = 0;
            inline void translate(float x, float y, float z, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->translate(Vector3(x, y, z), relativeTo); }

            virtual inline const Vector3& getVelocity() const
                { return Vector3::ZERO; }

            virtual void setOrientation(const Quaternion& orientation) = 0;
            inline void setOrientation(float w, float x, float y, float z)
                { this->setOrientation(Quaternion(w, x, y, z)); }
            inline void setOrientation(const Vector3& axis, const Radian& angle)
                { this->setOrientation(Quaternion(angle, axis)); }
            inline void setOrientation(const Vector3& axis, const Degree& angle)
                { this->setOrientation(Quaternion(angle, axis)); }
            inline const Quaternion& getOrientation() const
                { return this->node_->getOrientation(); }
            inline const Quaternion& getWorldOrientation() const
                { return this->node_->getWorldOrientation(); }

            virtual void rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL) = 0;
            inline void rotate(const Vector3& axis, const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->rotate(Quaternion(angle, axis), relativeTo); }
            inline void rotate(const Vector3& axis, const Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->rotate(Quaternion(angle, axis), relativeTo); }

            virtual void yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL) = 0;
            inline void yaw(const Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->yaw(Degree(angle), relativeTo); }
            virtual void pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL) = 0;
            inline void pitch(const Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->pitch(Degree(angle), relativeTo); }
            virtual void roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL) = 0;
            inline void roll(const Radian& angle, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL)
                { this->roll(Degree(angle), relativeTo); }

            virtual void lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z) = 0;
            virtual void setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z) = 0;
            inline void setDirection(float x, float y, float z, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_LOCAL, const Vector3& localDirectionVector = Vector3::NEGATIVE_UNIT_Z)
                { this->setDirection(Vector3(x, y, z), relativeTo, localDirectionVector); }

            inline void setScale3D(const Vector3& scale)
                { this->node_->setScale(scale); this->changedScale(); }
            inline void setScale3D(float x, float y, float z)
                { this->node_->setScale(x, y, z); this->changedScale(); }
            inline const Vector3& getScale3D() const
                { return this->node_->getScale(); }
            inline const Vector3& getWorldScale3D() const
                { return this->node_->_getDerivedScale(); }

            inline void setScale(float scale)
                { this->node_->setScale(scale, scale, scale); this->changedScale(); }
            inline float getScale() const
                { Vector3 scale = this->getScale3D(); return (scale.x == scale.y && scale.x == scale.z) ? scale.x : 1; }
            inline float getWorldScale() const
                { Vector3 scale = this->getWorldScale3D(); return (scale.x == scale.y && scale.x == scale.z) ? scale.x : 1; }

            inline void scale3D(const Vector3& scale)
                { this->node_->scale(scale); this->changedScale(); }
            inline void scale3D(float x, float y, float z)
                { this->node_->scale(x, y, z); this->changedScale(); }
            inline void scale(float scale)
                { this->node_->scale(scale, scale, scale); }

            virtual void changedScale() {}

            void attach(WorldEntity* object);
            void detach(WorldEntity* object);
            WorldEntity* getAttachedObject(unsigned int index) const;
            inline const std::set<WorldEntity*>& getAttachedObjects() const
                { return this->children_; }

            inline void attachToParent(WorldEntity* parent)
                { parent->attach(this); }
            inline void detachFromParent()
                { if (this->parent_) { this->parent_->detach(this); } }
            inline WorldEntity* getParent() const
                { return this->parent_; }

            void attachNode(Ogre::SceneNode* node);
            void detachNode(Ogre::SceneNode* node);
            void attachToNode(Ogre::SceneNode* node);
            void detachFromNode(Ogre::SceneNode* node);

        protected:
            Ogre::SceneNode* node_;

        private:
            void updateParent();

            inline void lookAt_xmlport(const Vector3& target)
                { this->lookAt(target); }
            inline void setDirection_xmlport(const Vector3& direction)
                { this->setDirection(direction); }
            inline void yaw_xmlport(const Degree& angle)
                { this->yaw(angle); }
            inline void pitch_xmlport(const Degree& angle)
                { this->pitch(angle); }
            inline void roll_xmlport(const Degree& angle)
                { this->roll(angle); }

            WorldEntity* parent_;
            unsigned int parentID_;
            std::set<WorldEntity*> children_;
    };

    SUPER_FUNCTION(5, WorldEntity, changedScale, false);
}

#endif /* _WorldEntity_H__ */
