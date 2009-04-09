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

#ifndef _MAP_H__
#define _MAP_H__

#include <string>
#include "OrxonoxPrereqs.h"
#include <OgrePrerequisites.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>

#include <OgreBorderPanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>

#include "overlays/OrxonoxOverlay.h"
#include "objects/Tickable.h"



namespace orxonox
{
    class _OrxonoxExport Map : public OrxonoxOverlay, public Tickable
    {

    public: // functions
        Map(BaseObject* creator);
        virtual ~Map() {}

        virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);
        virtual void tick(float dt);
        virtual void changedOwner();

        static Ogre::MaterialPtr createRenderCamera(Ogre::Camera * cam, std::string matName);
        
        static void openMap();

        static const int maxRange_s=1000;
        
    private: // functions
        void toggleVisibility();
        void updatePositions();


    private: // variables
        Ogre::SceneManager* sManager_;
        Ogre::SceneManager* mapSceneM_;
        Ogre::SceneNode* rootNode_;
        Ogre::OverlayManager * oManager_;
        Ogre::Camera* mReflectCam_;

        
        
        bool isVisible_;
    };
}

#endif /* _MAP_H__ */
