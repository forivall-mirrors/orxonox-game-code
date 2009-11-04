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
 *      Michael Wirth
 *   Co-authors:
 *      ...
 *
 */

#include "NewHumanController.h"

#include <OgreRay.h>
#include <OgreSceneQuery.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>

#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "gametypes/Gametype.h"
#include "infos/PlayerInfo.h"
#include "overlays/Map.h"
#include "graphics/Camera.h"
#include "sound/SoundManager.h"
#include "Radar.h"
#include "Scene.h"

namespace orxonox
{

    CreateUnloadableFactory(NewHumanController);

    NewHumanController::NewHumanController(BaseObject* creator) : HumanController(creator)
    {
        RegisterObject(NewHumanController);

        overlaySize = 0.08;

        CrossHairOverlay = new OrxonoxOverlay(this);
        CrossHairOverlay->setBackgroundMaterial("Orxonox/Crosshair3");
        CrossHairOverlay->setSize(Vector2(overlaySize,overlaySize));
        CrossHairOverlay->show();
    }

    NewHumanController::~NewHumanController()
    {
        if( this->isInitialized() )
        {
        }
    }

    void NewHumanController::tick(float dt) {
        CrossHairOverlay->setPosition(Vector2(static_cast<float>(this->currentYaw_)/2*-1+.5-overlaySize/2, static_cast<float>(this->currentPitch_)/2*-1+.5-overlaySize/2));

        HumanController::tick(dt);
    }

    /*void NewHumanController::tick(float dt)
    {
        if (GameMode::playsSound() && NewHumanController::localController_s && NewHumanController::localController_s->controllableEntity_)
        {
            // Update sound listener
            Camera* camera = NewHumanController::localController_s->controllableEntity_->getCamera();
            if (camera)
            {
                SoundManager::getInstance().setListenerPosition(camera->getWorldPosition());
                SoundManager::getInstance().setListenerOrientation(camera->getWorldOrientation());
            }
            else
                COUT(3) << "NewHumanController, Warning: Using a ControllableEntity without Camera" << std::endl;
        }
    }*/
    
    void NewHumanController::doFire(unsigned int firemode)
    {
        //if (HumanController::localController_s && HumanController::localController_s->controllableEntity_) {

/*

 // Get results, create a node/entity on the position
 for ( itr = result.begin(); itr != result.end(); itr++ )
 {
     if (itr->movable && itr->movable->getName() == "Head")
     {
         soundMgr->StopSound( &jaguarSoundChannel );
         soundMgr->PlaySound( jaguarSound, headNode, &jaguarSoundChannel );
         break;
     } // if
 }
*/

            Ogre::RaySceneQuery * rsq = HumanController::localController_s->getControllableEntity()->getScene()->getSceneManager()->createRayQuery(Ogre::Ray());

std::cout << "X: " << static_cast<float>(this->currentYaw_)/2*-1+.5 << "  Y: " << static_cast<float>(this->currentPitch_)/2*-1+.5 << endl;

            Ogre::Ray mouseRay = HumanController::localController_s->getControllableEntity()->getCamera()->getCamera()->getCameraToViewportRay(static_cast<float>(this->currentYaw_)/2*-1+.5, static_cast<float>(this->currentPitch_)/2*-1+.5);

            rsq->setRay(mouseRay);
            rsq->setSortByDistance(true);


            Ogre::RaySceneQueryResult &result = rsq->execute();

            Ogre::RaySceneQueryResult::iterator itr;
for ( itr = result.begin(); itr != result.end(); itr++ )
 {
     std::cout << "distance: " << itr->distance << "  name: " << itr->movable->getName() << " type: " << itr->movable->getMovableType() << endl;
if (itr->movable->isInScene() && itr->movable->getMovableType() == "Entity") {
std::cout << " in scene" << endl;
	itr->movable->getParentSceneNode()->showBoundingBox(true);
}
 }

//if (result.front().movable->isInScene()) std::cout << "in scene" << endl;
// && result.front().movable->getParentSceneNode() != NULL) result.front().movable->getParentSceneNode()->showBoundingBox(true);
//result.front().movable->setVisible(false);

std::cout << endl;
/*
            if (!result.empty()) {
            	Ogre::RaySceneQueryResultEntry obj = result.front();
            	std::cout << "distance: " << obj.distance << "  name: " << obj.movable->getName() << endl;
            }
*/


            HumanController::localController_s->getControllableEntity()->fire(firemode);
        //}
//}
    }

    void NewHumanController::yaw(const Vector2& value)
    {
//         SUPER(NewHumanController, yaw, value);
        HumanController::yaw(value);
        
        this->currentYaw_ = value.x;
        //std::cout << "Y: " << static_cast<float>(this->currentPitch_) << " X: " << static_cast<float>(this->currentYaw_) << endl;
    }
    void NewHumanController::pitch(const Vector2& value)
    {
//         SUPER(NewHumanController, pitch, value);
        HumanController::pitch(value);
        
        this->currentPitch_ = value.x;
        //std::cout << "Y: " << static_cast<float>(this->currentPitch_) << " X: " << static_cast<float>(this->currentYaw_) << endl;
    }

}
