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

/**
@file Scene.h
@brief Definition of Scene Class
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
    class _OrxonoxExport Scene : public BaseObject, public Synchronisable, public Tickable
    {
        public:
            /**
            @brief 
                This class holds a Scene which is a collection of all kinds of objects to be rendered in the same space,
                with the same physics and the same light properties. Objects can be anything from a light source, over non physical objects
                like Billboards to just plain Models with an attached Mesh
            */
            Scene(BaseObject* creator);
            virtual ~Scene();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            inline Ogre::SceneManager* getSceneManager() const
                { return this->sceneManager_; }
            inline Ogre::SceneNode* getRootSceneNode() const
                { return this->rootSceneNode_; }
            inline RenderQueueListener* getRenderQueueListener() const
                { return this->renderQueueListener_; }

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

            Ogre::SceneManager*      sceneManager_; //!< This is a pointer to the Ogre SceneManager we're using to render the Scene
            Ogre::SceneNode*         rootSceneNode_; //!< This is a pointer to the root node of the Scene tree
            RenderQueueListener*     renderQueueListener_; //!< this is a pointer to the RenderQueueListener we're using for this Scene

            std::string              skybox_; //!< This string holds information about the skybox we're using
            ColourValue              ambientLight_; //!< This variable holds the color value for the ambient light in our scene, usually black in space
            std::list<BaseObject*>   objects_; //!< This list holds all the objects created in our scene
            bool                     bShadows_; //!< Do we want shadows in our scene?
            float                    soundReferenceDistance_; //!< This holds a reference distance, which represents the distance between our scene and the listener
            Radar*                   radar_; //!< This is a pointer to a Radar object assigned with this scene


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
