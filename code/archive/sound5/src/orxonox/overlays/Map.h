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
 *      Si Sun
 *
 */

#ifndef _Map_H__
#define _Map_H__

#include "OrxonoxPrereqs.h"

#include <OgreMaterial.h>

#include "util/UtilPrereqs.h"
#include "tools/interfaces/Tickable.h"
#include "overlays/OrxonoxOverlay.h"

namespace orxonox
{
    class _OrxonoxExport Map : public OrxonoxOverlay, public Tickable
    {

    public: // functions
        Map(BaseObject* creator);
        virtual ~Map();

        virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);
        virtual void tick(float dt);
        virtual void changedOwner();

        static Ogre::MaterialPtr createRenderCamera(Ogre::Camera * cam, const std::string& matName);

        static void openMap();
        // HACK!
        static void hackDestroyMap();

//Not yet implemented
        static const int maxRange_s=1000;

        static void rotateYaw(const Vector2& value);
        static void rotatePitch(const Vector2& value);
        static void Zoom(const Vector2& value);
        // variables


        bool inline getVisibility()
            { return this->isVisible_; };

        static inline Ogre::SceneManager* getMapSceneManagerPtr()
        {
            return Map::singletonMap_s->mapSceneM_s;
        }
        static inline Map* getSingletonPtr()
        {
            return Map::singletonMap_s;
        }

        static inline Ogre::SceneManager* getMapSceneManager()
        {
            return Map::mapSceneM_s;
        }



    private: // functions

        void toggleVisibility();
        void updatePositions();
//        void changedPlayerNode();
        static inline void setMapSceneManager( Ogre::SceneManager * sm)
        {
            Map::mapSceneM_s = sm;
        }
        //static Ogre::MaterialPtr init();

    private: // variables
        static Map* singletonMap_s;

        Ogre::SceneManager* sManager_;
        Ogre::OverlayManager * oManager_;

        static Ogre::SceneManager* mapSceneM_s;
        static Ogre::SceneNode* CamNode_;
        static Ogre::Camera* Cam_;
        static Ogre::MaterialPtr OverlayMaterial_;
        static Ogre::Overlay* overlay_;
        //Ogre::SceneNode* CamNodeHelper_;
        Ogre::SceneNode* playerShipNode_;
        Ogre::SceneNode* planeNode_;
        Ogre::MovablePlane* movablePlane_;
        int mouseLookSpeed_;
        bool isVisible_;

    friend class RadarViewable;
    };
}

#endif /* _Map_H__ */
