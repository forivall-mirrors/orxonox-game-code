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
 *      Cyrill Frei
 *   Co-authors:
 *      ...
 *
 */

#ifndef _FpsPlayer_H__
#define _FpsPlayer_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <LinearMath/btVector3.h>
#include "util/Math.h"
#include "tools/Mesh.h"
#include "Pawn.h"

namespace orxonox
{
    class _OrxonoxExport FpsPlayer : public Pawn
    {
        public:
            FpsPlayer(BaseObject* creator);
            virtual ~FpsPlayer();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            void registerVariables();
            void setConfigValues();

            virtual void moveFrontBack(const Vector2& value);
            virtual void moveRightLeft(const Vector2& value);
            virtual void moveUpDown(const Vector2& value);

            virtual void rotateYaw(const Vector2& value);
            virtual void rotatePitch(const Vector2& value);
            virtual void rotateRoll(const Vector2& value);


            inline void setMeshSource(const std::string& meshname)
                { this->meshSrc_ = meshname; this->changedMesh(); }
            inline const std::string& getMeshSource() const
                { return this->meshSrc_; }

            void boost(bool bBoost); //acctually jump

            virtual void fire();

            bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint);

            virtual void addedWeaponPack(WeaponPack* wPack);

        protected:
            virtual void setPlayer(PlayerInfo* player);
            virtual void startLocalHumanControl();
            bool bInvertYAxis_;

            bool bBoost_;
            bool bPermanentBoost_;
            Vector3 steering_;
            float primaryThrust_;
            float auxilaryThrust_;
            float rotationThrust_;
            std::string weaponMashName_;
            btVector3 localLinearAcceleration_;
            btVector3 localAngularAcceleration_;

        private:
            virtual bool isCollisionTypeLegal(WorldEntity::CollisionType type) const;
            float speed_;

            void changedMesh();
            Mesh mesh_;
            std::string meshSrc_;
            float yaw_;
            float pitch_;
            float roll_;
            Vector3 localVelocity_;
            bool isFloor_;
            bool thisTickBoost_;
            Quaternion savedOrientation_;
            Ogre::SceneNode* weaponNode_;
    };
}

#endif /* _FpsPlayer_H__ */
