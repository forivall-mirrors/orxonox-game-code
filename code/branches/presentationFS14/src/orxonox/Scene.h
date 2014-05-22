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
 *      Reto Grieder (physics)
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Scene_H__
#define _Scene_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include <set>
#include <string>

#include "util/Math.h"
#include "util/OgreForwardRefs.h"
#include "core/BaseObject.h"
#include "network/synchronisable/Synchronisable.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox
{
    class _OrxonoxExport Scene : public BaseObject, public Synchronisable, public Tickable, public Context
    {
        public:
            Scene(Context* context);
            virtual ~Scene();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            inline Ogre::SceneManager* getSceneManager() const
                { return this->sceneManager_; }
            inline Ogre::SceneNode* getRootSceneNode() const
                { return this->rootSceneNode_; }

            void setSkybox(const std::string& skybox);
            inline const std::string& getSkybox() const
                { return this->skybox_; }

            void setAmbientLight(const ColourValue& colour);
            inline const ColourValue& getAmbientLight() const
                { return this->ambientLight_; }

            void setShadow(bool bShadow);
            inline bool getShadow() const
                { return this->bShadows_; }

            inline void setSoundReferenceDistance(float distance)
                { this->soundReferenceDistance_ = distance; }
            inline float getSoundReferenceDistance() const
                { return this->soundReferenceDistance_; }

            inline Radar* getRadar()
                { return this->radar_; }

            inline virtual uint32_t getSceneID() const { return this->getObjectID(); }

            virtual void tick(float dt);

        private:
            void addObject(BaseObject* object);
            BaseObject* getObject(unsigned int index) const;

            void networkcallback_applySkybox()
                { this->setSkybox(this->skybox_); }
            void networkcallback_applyAmbientLight()
                { this->setAmbientLight(this->ambientLight_); }
            void networkcallback_applyShadows()
                { this->setShadow(this->bShadows_); }

            Ogre::SceneManager*      sceneManager_;
            Ogre::SceneNode*         rootSceneNode_;

            std::string              skybox_;
            ColourValue              ambientLight_;
            std::list<BaseObject*>   objects_;
            bool                     bShadows_;
            float                    soundReferenceDistance_;
            Radar*                   radar_;


        /////////////
        // Physics //
        /////////////

        public:
            inline bool hasPhysics()
                { return this->physicalWorld_ != 0; }
            void setPhysicalWorld(bool wantsPhysics);

            void setNegativeWorldRange(const Vector3& range);
            inline const Vector3& getNegativeWorldRange() const
                { return this->negativeWorldRange_; }

            void setPositiveWorldRange(const Vector3& range);
            inline const Vector3& getPositiveWorldRange() const
                { return this->positiveWorldRange_; }

            void setGravity(const Vector3& gravity);
            inline const Vector3& getGravity() const
                { return this->gravity_; }

            void addPhysicalObject(WorldEntity* object);
            void removePhysicalObject(WorldEntity* object);

        private:
            inline void networkcallback_hasPhysics()
                { this->setPhysicalWorld(this->bHasPhysics_); }
            inline void networkcallback_negativeWorldRange()
                { this->setNegativeWorldRange(this->negativeWorldRange_); }
            inline void networkcallback_positiveWorldRange()
                { this->setPositiveWorldRange(this->positiveWorldRange_); }
            inline void networkcallback_gravity()
                { this->setGravity(this->gravity_); }

            // collision callback from bullet
            static bool collisionCallback(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0,
                                          int index0, const btCollisionObject* colObj1, int partId1, int index1);

            static bool customCollisionCallback(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0,
                                                      int index0, const btCollisionObject* colObj1, int partId1, int index1);

            // Bullet objects
            btDiscreteDynamicsWorld*             physicalWorld_;
            bt32BitAxisSweep3*                   broadphase_;
            btDefaultCollisionConfiguration*     collisionConfig_;
            btCollisionDispatcher*               dispatcher_;
            btSequentialImpulseConstraintSolver* solver_;

            std::set<WorldEntity*>               physicalObjectQueue_;
            std::set<WorldEntity*>               physicalObjects_;
            bool                                 bHasPhysics_;
            Vector3                              negativeWorldRange_;
            Vector3                              positiveWorldRange_;
            Vector3                              gravity_;
    };
}

#endif /* _Scene_H__ */
