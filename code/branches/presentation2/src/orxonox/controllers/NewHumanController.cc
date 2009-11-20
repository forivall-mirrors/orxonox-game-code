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
#include "infos/PlayerInfo.h"
#include "overlays/OrxonoxOverlay.h"
#include "graphics/Camera.h"
#include "sound/SoundManager.h"
#include "Scene.h"

namespace orxonox
{
    SetConsoleCommand(NewHumanController, changeMode,          false).keybindMode(KeybindMode::OnPress);

    CreateUnloadableFactory(NewHumanController);

    NewHumanController* NewHumanController::localController_s = 0;

    NewHumanController::NewHumanController(BaseObject* creator) : HumanController(creator)
    {
        RegisterObject(NewHumanController);

        overlaySize_ = 0.08;
        controlMode_ = 0;

        crossHairOverlay_ = new OrxonoxOverlay(this);
        crossHairOverlay_->setBackgroundMaterial("Orxonox/Crosshair3");
        crossHairOverlay_->setSize(Vector2(overlaySize_, overlaySize_));
        crossHairOverlay_->show();

        // HACK: Define which objects are targetable when considering the creator of an orxonox::Model
        this->targetMask_.exclude(ClassByString("BaseObject"));
        this->targetMask_.include(ClassByString("WorldEntity"));
        this->targetMask_.exclude(ClassByString("Projectile"));

        NewHumanController::localController_s = this;
    }

    NewHumanController::~NewHumanController()
    {
        if (this->isInitialized())
        {
        }
    }

    void NewHumanController::tick(float dt)
    {
        crossHairOverlay_->setPosition(Vector2(static_cast<float>(this->currentYaw_)/2*-1+.5-overlaySize_/2, static_cast<float>(this->currentPitch_)/2*-1+.5-overlaySize_/2));

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

        HumanController::localController_s->getControllableEntity()->fire(firemode);
    }

    Vector3 NewHumanController::getTarget()
    {
        Ogre::RaySceneQuery * rsq = HumanController::localController_s->getControllableEntity()->getScene()->getSceneManager()->createRayQuery(Ogre::Ray());

        Ogre::Ray mouseRay = HumanController::localController_s->getControllableEntity()->getCamera()->getOgreCamera()->getCameraToViewportRay(static_cast<float>(this->currentYaw_)/2*-1+.5, static_cast<float>(this->currentPitch_)/2*-1+.5);

        rsq->setRay(mouseRay);
        rsq->setSortByDistance(true);

        /*
        Distance of objects:
        ignore everything under 200 maybe even take 1000 as min distance to shoot at

        shots are regularly traced and are entities!!!!!!!!! this is the biggest problem
        they vanish only after a distance of 10'000
        */


        Ogre::RaySceneQueryResult& result = rsq->execute();

        Ogre::RaySceneQueryResult::iterator itr;
        for (itr = result.begin(); itr != result.end(); ++itr)
        {
            if (itr->movable->isInScene() && itr->movable->getMovableType() == "Entity" && itr->distance > 500)
            {
                // Try to cast the user pointer
                WorldEntity* wePtr = dynamic_cast<WorldEntity*>(itr->movable->getUserObject());
                if (wePtr)
                {
                    BaseObject* creator = wePtr->getCreator();
                    if (this->targetMask_.isExcluded(creator->getIdentifier()))
                        continue;
                }

                itr->movable->getParentSceneNode()->showBoundingBox(true);
                //std::cout << itr->movable->getParentSceneNode()->_getDerivedPosition() << endl;
                return mouseRay.getOrigin() + mouseRay.getDirection() * itr->distance; //or itr->movable->getParentSceneNode()->_getDerivedPosition()
            }

        }

	return mouseRay.getOrigin() + mouseRay.getDirection() * 1200;

        //return this->controllableEntity_->getWorldPosition() + (this->controllableEntity_->getWorldOrientation() * Vector3::NEGATIVE_UNIT_Z * 2000);
        //return this->controllableEntity_->getWorldPosition() + (this->controllableEntity_->getCamera()->getOgreCamera()->getOrientation() * Vector3::NEGATIVE_UNIT_Z);
    }

    void NewHumanController::yaw(const Vector2& value)
    {
//         SUPER(NewHumanController, yaw, value);
        if (this->controlMode_ == 0)
            HumanController::yaw(value);
        
        this->currentYaw_ = value.x;
        //std::cout << "Y: " << static_cast<float>(this->currentPitch_) << " X: " << static_cast<float>(this->currentYaw_) << endl;
    }

    void NewHumanController::pitch(const Vector2& value)
    {
//         SUPER(NewHumanController, pitch, value);
        if (this->controlMode_ == 0)
            HumanController::pitch(value);

        this->currentPitch_ = value.x;
        //std::cout << "Y: " << static_cast<float>(this->currentPitch_) << " X: " << static_cast<float>(this->currentYaw_) << endl;
    }

    void NewHumanController::changeMode() {
        if (NewHumanController::localController_s->controlMode_ == 0)
            NewHumanController::localController_s->controlMode_ = 1;
        else
            NewHumanController::localController_s->controlMode_ = 0;
    }
}
