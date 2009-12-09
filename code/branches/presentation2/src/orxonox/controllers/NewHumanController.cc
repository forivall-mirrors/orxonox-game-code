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

#include <cmath>
#include <OgreRay.h>
#include <OgreSceneQuery.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>

#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "infos/PlayerInfo.h"
#include "overlays/OrxonoxOverlay.h"
#include "graphics/Camera.h"
#include "sound/SoundManager.h"
#include "Scene.h"

namespace orxonox
{
    SetConsoleCommand(NewHumanController, changeMode,          false).keybindMode(KeybindMode::OnPress);
    SetConsoleCommand(NewHumanController, accelerate,          false).keybindMode(KeybindMode::OnPress);
    SetConsoleCommand(NewHumanController, decelerate,          false).keybindMode(KeybindMode::OnPress);
    SetConsoleCommand(NewHumanController, unfire,              true).keybindMode(KeybindMode::OnRelease);

    CreateUnloadableFactory(NewHumanController);

    NewHumanController* NewHumanController::localController_s = 0;

    NewHumanController::NewHumanController(BaseObject* creator)
        : HumanController(creator)
        , crossHairOverlay_(NULL)
        , centerOverlay_(NULL)
    {
        RegisterObject(NewHumanController);

        overlaySize_ = 0.08;
        arrowsSize_ = 0.4;
        controlMode_ = 0;
        acceleration_ = 0;
        accelerating_ = false;
        firemode_ = -1;
        showArrows_ = true;
        showOverlays_ = false;

        //currentPitch_ = 1;
        //currentYaw_ = 1;

        if (GameMode::showsGraphics())
        {
            crossHairOverlay_ = new OrxonoxOverlay(this);
            crossHairOverlay_->setBackgroundMaterial("Orxonox/Crosshair3");
            crossHairOverlay_->setSize(Vector2(overlaySize_, overlaySize_));
            crossHairOverlay_->hide();
            //crossHairOverlay_->setAspectCorrection(true); not working

            centerOverlay_ = new OrxonoxOverlay(this);
            centerOverlay_->setBackgroundMaterial("Orxonox/CenterOverlay");
            centerOverlay_->setSize(Vector2(overlaySize_ * 2.5, overlaySize_ * 2.5));
            centerOverlay_->setPosition(Vector2(0.5 - overlaySize_*2.5/2.0, 0.5 - overlaySize_*2.5/2.0));\
            centerOverlay_->hide();

            if (showArrows_)
            {
                arrowsOverlay1_ = new OrxonoxOverlay(this);
                arrowsOverlay1_->setBackgroundMaterial("Orxonox/CenterOverlay");
                arrowsOverlay1_->setSize(Vector2(0.02727, 0.36 * arrowsSize_));
                arrowsOverlay1_->setPickPoint(Vector2(0.5, 0.5));
                arrowsOverlay1_->setPosition(Vector2(0.5, 0.5));
                arrowsOverlay1_->hide();
    
                arrowsOverlay2_ = new OrxonoxOverlay(this);
                arrowsOverlay2_->setBackgroundMaterial("Orxonox/CenterOverlay");
                arrowsOverlay2_->setSize(Vector2(0.02727, 0.59 * arrowsSize_));
                arrowsOverlay2_->setPickPoint(Vector2(0.5, 0.5));
                arrowsOverlay2_->setPosition(Vector2(0.5, 0.5));
                arrowsOverlay2_->hide();
    
                arrowsOverlay3_ = new OrxonoxOverlay(this);
                arrowsOverlay3_->setBackgroundMaterial("Orxonox/CenterOverlay");
                arrowsOverlay3_->setSize(Vector2(0.02727, 0.77 * arrowsSize_));
                arrowsOverlay3_->setPickPoint(Vector2(0.5, 0.5));
                arrowsOverlay3_->setPosition(Vector2(0.5, 0.5));
                arrowsOverlay3_->hide();
    
                arrowsOverlay4_ = new OrxonoxOverlay(this);
                arrowsOverlay4_->setBackgroundMaterial("Orxonox/CenterOverlay");
                arrowsOverlay4_->setSize(Vector2(0.02727, arrowsSize_));
                arrowsOverlay4_->setPickPoint(Vector2(0.5, 0.5));
                arrowsOverlay4_->setPosition(Vector2(0.5, 0.5));
                arrowsOverlay4_->hide();
            }
        }

        // HACK: Define which objects are targetable when considering the creator of an orxonox::Model
        this->targetMask_.exclude(ClassByString("BaseObject"));
        this->targetMask_.include(ClassByString("WorldEntity"));
        this->targetMask_.exclude(ClassByString("Projectile"));

        NewHumanController::localController_s = this;

        controlPaused_ = false;

//HumanController::localController_s->getControllableEntity()->getCamera()->setDrag(true);
    }

    NewHumanController::~NewHumanController()
    {
        if (this->isInitialized())
        {
            if (this->crossHairOverlay_)
                this->crossHairOverlay_->destroy();
            if (this->centerOverlay_)
                this->centerOverlay_->destroy();

            if (showArrows_)
            {
                if (this->arrowsOverlay1_)
                    this->arrowsOverlay1_->destroy();
                if (this->arrowsOverlay2_)
                    this->arrowsOverlay2_->destroy();
                if (this->arrowsOverlay3_)
                    this->arrowsOverlay3_->destroy();
                if (this->arrowsOverlay4_)
                    this->arrowsOverlay4_->destroy();
            }
        }
    }

    void NewHumanController::tick(float dt)
    {
        if (GameMode::showsGraphics())
        {

            if( this->controllableEntity_ && !this->controllableEntity_->isInMouseLook() )
            {
                if (this->getControllableEntity() && this->getControllableEntity()->getIdentifier()->getName() == "SpaceShip")
                    this->showOverlays();
                this->updateTarget();
                if ( !controlPaused_ ) {
                    this->crossHairOverlay_->setPosition(Vector2(static_cast<float>(this->currentYaw_)/2*-1+.5-overlaySize_/2, static_cast<float>(this->currentPitch_)/2*-1+.5-overlaySize_/2));

                    if ( this->controlMode_ == 0 || ( this->controlMode_ == 1 && this->firemode_ == 1 ) )
                    {
                        if ( this->showOverlays_ )
                            alignArrows();
                    }
                    else
                        hideArrows();
                }
            }
            else
                this->hideOverlays();

            if ( this->acceleration_ > 0 )
            {
/*
if (this->controllableEntity_ && this->controllableEntity_->getEngine()) {
    std::cout << this->controllableEntity_->getEngine()->getAccelerationFront() << endl;
}
*/
                if ( this->accelerating_ )
                    HumanController::moveFrontBack(Vector2(1, 0));
                else
                    HumanController::moveFrontBack(Vector2(this->acceleration_, 0)); 
                this->accelerating_ = false;
                //HumanController::moveFrontBack(Vector2(clamp(this->acceleration_ + this->currentAcceleration_, 0.0f, 1.0f), 0));
            }
        }

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

        this->firemode_ = firemode;

        if (firemode == 1 && this->controlMode_ == 1)
        {
            //unlocked steering, steer on right mouse click
            HumanController::yaw(Vector2(this->currentYaw_, 0));
            HumanController::pitch(Vector2(this->currentPitch_, 0));
        }
        else
            HumanController::localController_s->getControllableEntity()->fire(firemode);

    }

    void NewHumanController::unfire()
    {
        if (NewHumanController::localController_s)
            NewHumanController::localController_s->doUnfire();
    }

    void NewHumanController::doUnfire()
    {
        this->firemode_ = -1;
        hideArrows();
    }

    void NewHumanController::updateTarget()
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
        Pawn* pawn = orxonox_cast<Pawn*>(this->getControllableEntity());

        Ogre::RaySceneQueryResult::iterator itr;
        for (itr = result.begin(); itr != result.end(); ++itr)
        {
            if (itr->movable->isInScene() && itr->movable->getMovableType() == "Entity" && itr->distance > 500)
            {
                // Try to cast the user pointer
                WorldEntity* wePtr = dynamic_cast<WorldEntity*>(itr->movable->getUserObject());
                if (wePtr)
                {
                    // go through all parents of object and look wheter they are Sightable or not
                    bool isSightable = false;
                    WorldEntity* parent = wePtr->getParent();
                    while( parent )
                    {
                        if (this->targetMask_.isExcluded(parent->getIdentifier()))
                        {
                            parent = parent->getParent();
                            continue;
                        }
                        else
                        {
                            isSightable = true;
                            break;
                        }
                    }
                    if ( !isSightable )
                        continue;
                }

                if ( this->getControllableEntity() && this->getControllableEntity()->getTarget() != wePtr )
                {
                    this->getControllableEntity()->setTarget(wePtr);
                }

                if( pawn )
                {
                    pawn->setAimPosition( mouseRay.getOrigin() + mouseRay.getDirection() * itr->distance );
                }

                //itr->movable->getParentSceneNode()->showBoundingBox(true);
                //std::cout << itr->movable->getParentSceneNode()->_getDerivedPosition() << endl;
                //return mouseRay.getOrigin() + mouseRay.getDirection() * itr->distance; //or itr->movable->getParentSceneNode()->_getDerivedPosition()
                return;
            }

        }
        if ( pawn )
        {
            pawn->setAimPosition( mouseRay.getOrigin() + mouseRay.getDirection() * 1200 );
        }

        if( this->getControllableEntity() && this->getControllableEntity()->getTarget() != 0 )
            this->getControllableEntity()->setTarget( 0 );
    

        //return this->controllableEntity_->getWorldPosition() + (this->controllableEntity_->getWorldOrientation() * Vector3::NEGATIVE_UNIT_Z * 2000);
        //return this->controllableEntity_->getWorldPosition() + (this->controllableEntity_->getCamera()->getOgreCamera()->getOrientation() * Vector3::NEGATIVE_UNIT_Z);
    }

    void NewHumanController::frontback(const Vector2& value)
    {
        this->accelerating_ = true;

        //if (this->acceleration_ == 0)
            HumanController::frontback(value);
    }

    void NewHumanController::yaw(const Vector2& value)
    {
//         SUPER(NewHumanController, yaw, value);
        if (this->controlMode_ == 0 || ( this->controllableEntity_ && this->controllableEntity_->isInMouseLook() ) )
            HumanController::yaw(value);

        this->currentYaw_ = value.x;
    }

    void NewHumanController::pitch(const Vector2& value)
    {
//         SUPER(NewHumanController, pitch, value);
        if (this->controlMode_ == 0 || ( this->controllableEntity_ && this->controllableEntity_->isInMouseLook() ) )
            HumanController::pitch(value);

        this->currentPitch_ = value.x;
    }

    void NewHumanController::changeMode()
    {
        if (NewHumanController::localController_s && NewHumanController::localController_s->controlMode_ == 0)
        {
                NewHumanController::localController_s->controlMode_ = 1;
                NewHumanController::localController_s->hideArrows();
        }
        else
            NewHumanController::localController_s->controlMode_ = 0;
    }

    void NewHumanController::changedControllableEntity()
    {
        this->controlMode_ = 0;
        this->currentYaw_ = 0;
        this->currentPitch_ = 0;
        if (this->getControllableEntity() && this->getControllableEntity()->getIdentifier()->getName() == "SpaceShip")
        {
            this->showOverlays_ = true;
            if( !this->controlPaused_ )
            {
                this->showOverlays();
                this->alignArrows();
            }
        }
        else
        {
            this->showOverlays_ = false;
            this->hideOverlays();
        }
    }

    void NewHumanController::accelerate()
    {
        if ( NewHumanController::localController_s )
        {
            NewHumanController::localController_s->acceleration_ = clamp(NewHumanController::localController_s->acceleration_ + 0.2f, 0.00f, 1.0f);
        }
    }

    void NewHumanController::decelerate()
    {
        if ( NewHumanController::localController_s )
        {
            NewHumanController::localController_s->acceleration_ = clamp(NewHumanController::localController_s->acceleration_ - 0.1f, 0.0f, 1.0f);
        }
    }

    void NewHumanController::doResumeControl() {
        this->controlPaused_ = false;
        if( this->showOverlays_ ) {
            this->showOverlays();
        }
    }

    void NewHumanController::doPauseControl() {
        this->controlPaused_ = true;
        
        this->hideOverlays();
    }

    void NewHumanController::alignArrows() {
        if (showArrows_) {
            hideArrows();
    
            float distance = sqrt(pow(static_cast<float>(this->currentYaw_)/2*-1,2) + pow(static_cast<float>(this->currentPitch_)/2*-1,2));
    
            if ( distance > 0.04 && distance <= 0.59 * arrowsSize_ / 2.0 ) {
                this->arrowsOverlay1_->setRotation(Degree(-90 + -1.0 * atan2(static_cast<float>(this->currentPitch_)/2*-1, static_cast<float>(this->currentYaw_)/2*-1) / (2.0f * Ogre::Math::PI) * 360.0f));
    
                this->arrowsOverlay1_->show();
            }
            else if ( distance > 0.59 * arrowsSize_ / 2.0 && distance <= 0.77 * arrowsSize_ / 2.0 ) {
                this->arrowsOverlay2_->setRotation(Degree(-90 + -1.0 * atan2(static_cast<float>(this->currentPitch_)/2*-1, static_cast<float>(this->currentYaw_)/2*-1) / (2.0f * Ogre::Math::PI) * 360.0f));
    
                this->arrowsOverlay2_->show();
            }
            else if ( distance > 0.77 * arrowsSize_ / 2.0 && distance <= arrowsSize_ / 2.0 ) {
                this->arrowsOverlay3_->setRotation(Degree(-90 + -1.0 * atan2(static_cast<float>(this->currentPitch_)/2*-1, static_cast<float>(this->currentYaw_)/2*-1) / (2.0f * Ogre::Math::PI) * 360.0f));
    
                this->arrowsOverlay3_->show();
            }
            else if ( distance > arrowsSize_ / 2.0 ) {
                this->arrowsOverlay4_->setRotation(Degree(-90 + -1.0 * atan2(static_cast<float>(this->currentPitch_)/2*-1, static_cast<float>(this->currentYaw_)/2*-1) / (2.0f * Ogre::Math::PI) * 360.0f));
    
                this->arrowsOverlay4_->show();
            }
        }
    }

    void NewHumanController::showOverlays() {
        this->crossHairOverlay_->show();
        this->centerOverlay_->show();
        
        if (showArrows_) {
            this->arrowsOverlay1_->show();
            this->arrowsOverlay2_->show();
            this->arrowsOverlay3_->show();
            this->arrowsOverlay4_->show();
        }
    }

    void NewHumanController::hideOverlays() {
        this->crossHairOverlay_->hide();
        this->centerOverlay_->hide();
        
        this->hideArrows();
    }

    void NewHumanController::hideArrows() {
        if (showArrows_) {
            this->arrowsOverlay1_->hide();
            this->arrowsOverlay2_->hide();
            this->arrowsOverlay3_->hide();
            this->arrowsOverlay4_->hide();
        }
    }
}
