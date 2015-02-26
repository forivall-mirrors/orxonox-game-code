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
#include <bullet/BulletCollision/NarrowPhaseCollision/btManifoldPoint.h>

#include "core/CoreIncludes.h"
#include "core/command/ConsoleCommand.h"
#include "core/input/KeyBinder.h"
#include "core/input/KeyBinderManager.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "infos/PlayerInfo.h"
#include "overlays/OrxonoxOverlay.h"
#include "graphics/Camera.h"
#include "sound/SoundManager.h"
#include "tools/BulletConversions.h"
#include "Scene.h"

namespace orxonox
{
    SetConsoleCommand("NewHumanController", "changeMode", &NewHumanController::changeMode).keybindMode(KeybindMode::OnPress);
    SetConsoleCommand("NewHumanController", "accelerate", &NewHumanController::accelerate).keybindMode(KeybindMode::OnPress);
    SetConsoleCommand("NewHumanController", "decelerate", &NewHumanController::decelerate).keybindMode(KeybindMode::OnPress);
    SetConsoleCommand("NewHumanController", "unfire",     &NewHumanController::unfire    ).keybindMode(KeybindMode::OnRelease).addShortcut();

    RegisterUnloadableClass(NewHumanController);

    NewHumanController* NewHumanController::localController_s = 0;

    NewHumanController::NewHumanController(Context* context)
        : HumanController(context)
        , crossHairOverlay_(NULL)
        , centerOverlay_(NULL)
        , damageOverlayTop_(NULL)
        , damageOverlayRight_(NULL)
        , damageOverlayBottom_(NULL)
        , damageOverlayLeft_(NULL)
        , damageOverlayTT_(0)
        , damageOverlayTR_(0)
        , damageOverlayTB_(0)
        , damageOverlayTL_(0)
        , arrowsOverlay1_(NULL)
        , arrowsOverlay2_(NULL)
        , arrowsOverlay3_(NULL)
        , arrowsOverlay4_(NULL)
    {
        RegisterObject(NewHumanController);

        overlaySize_ = 0.08f;
        arrowsSize_ = 0.4f;

        damageOverlayTime_ = 0.6f;

        controlMode_ = 0;
        acceleration_ = 0;
        accelerating_ = false;
        firemode_ = -1;

        showArrows_ = true;
        showOverlays_ = false;
        showDamageOverlay_ = true;

        //currentPitch_ = 1;
        //currentYaw_ = 1;

        if (GameMode::showsGraphics())
        {
            crossHairOverlay_ = new OrxonoxOverlay(this->getContext());
            crossHairOverlay_->setBackgroundMaterial("Orxonox/Crosshair3");
            crossHairOverlay_->setSize(Vector2(overlaySize_, overlaySize_));
            crossHairOverlay_->hide();
            //crossHairOverlay_->setAspectCorrection(true); not working

            centerOverlay_ = new OrxonoxOverlay(this->getContext());
            centerOverlay_->setBackgroundMaterial("Orxonox/CenterOverlay");
            centerOverlay_->setSize(Vector2(overlaySize_ * 2.5f, overlaySize_ * 2.5f));
            centerOverlay_->setPosition(Vector2(0.5f - overlaySize_*2.5f/2.0f, 0.5f - overlaySize_*2.5f/2.0f));
            centerOverlay_->hide();

            if (showDamageOverlay_)
            {
                damageOverlayTop_ = new OrxonoxOverlay(this->getContext());
                damageOverlayTop_->setBackgroundMaterial("Orxonox/DamageOverlayTop");
                damageOverlayTop_->setSize(Vector2(overlaySize_ * 2.5f, overlaySize_ * 2.5f));
                damageOverlayTop_->setPosition(Vector2(0.5f - overlaySize_*2.5f/2.0f, 0.5f - overlaySize_*2.5f/2.0f));
                damageOverlayTop_->hide();

                damageOverlayRight_ = new OrxonoxOverlay(this->getContext());
                damageOverlayRight_->setBackgroundMaterial("Orxonox/DamageOverlayRight");
                damageOverlayRight_->setSize(Vector2(overlaySize_ * 2.5f, overlaySize_ * 2.5f));
                damageOverlayRight_->setPosition(Vector2(0.5f - overlaySize_*2.5f/2.0f, 0.5f - overlaySize_*2.5f/2.0f));
                damageOverlayRight_->hide();

                damageOverlayBottom_ = new OrxonoxOverlay(this->getContext());
                damageOverlayBottom_->setBackgroundMaterial("Orxonox/DamageOverlayBottom");
                damageOverlayBottom_->setSize(Vector2(overlaySize_ * 2.5f, overlaySize_ * 2.5f));
                damageOverlayBottom_->setPosition(Vector2(0.5f - overlaySize_*2.5f/2.0f, 0.5f - overlaySize_*2.5f/2.0f));
                damageOverlayBottom_->hide();

                damageOverlayLeft_ = new OrxonoxOverlay(this->getContext());
                damageOverlayLeft_->setBackgroundMaterial("Orxonox/DamageOverlayLeft");
                damageOverlayLeft_->setSize(Vector2(overlaySize_ * 2.5f, overlaySize_ * 2.5f));
                damageOverlayLeft_->setPosition(Vector2(0.5f - overlaySize_*2.5f/2.0f, 0.5f - overlaySize_*2.5f/2.0f));
                damageOverlayLeft_->hide();
            }

            if (showArrows_)
            {
                arrowsOverlay1_ = new OrxonoxOverlay(this->getContext());
                arrowsOverlay1_->setBackgroundMaterial("Orxonox/DirectionArrows1");
                arrowsOverlay1_->setSize(Vector2(0.02727f, 0.36f * arrowsSize_));
                arrowsOverlay1_->setPickPoint(Vector2(0.5f, 0.5f));
                arrowsOverlay1_->setPosition(Vector2(0.5f, 0.5f));
                arrowsOverlay1_->hide();

                arrowsOverlay2_ = new OrxonoxOverlay(this->getContext());
                arrowsOverlay2_->setBackgroundMaterial("Orxonox/DirectionArrows2");
                arrowsOverlay2_->setSize(Vector2(0.02727f, 0.59f * arrowsSize_));
                arrowsOverlay2_->setPickPoint(Vector2(0.5f, 0.5f));
                arrowsOverlay2_->setPosition(Vector2(0.5f, 0.5f));
                arrowsOverlay2_->hide();

                arrowsOverlay3_ = new OrxonoxOverlay(this->getContext());
                arrowsOverlay3_->setBackgroundMaterial("Orxonox/DirectionArrows3");
                arrowsOverlay3_->setSize(Vector2(0.02727f, 0.77f * arrowsSize_));
                arrowsOverlay3_->setPickPoint(Vector2(0.5f, 0.5f));
                arrowsOverlay3_->setPosition(Vector2(0.5f, 0.5f));
                arrowsOverlay3_->hide();

                arrowsOverlay4_ = new OrxonoxOverlay(this->getContext());
                arrowsOverlay4_->setBackgroundMaterial("Orxonox/DirectionArrows4");
                arrowsOverlay4_->setSize(Vector2(0.02727f, arrowsSize_));
                arrowsOverlay4_->setPickPoint(Vector2(0.5f, 0.5f));
                arrowsOverlay4_->setPosition(Vector2(0.5f, 0.5f));
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

            if (this->arrowsOverlay1_)
                this->arrowsOverlay1_->destroy();
            if (this->arrowsOverlay2_)
                this->arrowsOverlay2_->destroy();
            if (this->arrowsOverlay3_)
                this->arrowsOverlay3_->destroy();
            if (this->arrowsOverlay4_)
                this->arrowsOverlay4_->destroy();
            if (this->damageOverlayTop_)
                this->damageOverlayTop_->destroy();
            if (this->damageOverlayLeft_)
                this->damageOverlayLeft_->destroy();
            if (this->damageOverlayRight_)
                this->damageOverlayRight_->destroy();
            if (this->damageOverlayBottom_)
                this->damageOverlayBottom_->destroy();
        }
    }

    void NewHumanController::tick(float dt)
    {
        if (GameMode::showsGraphics())
        {

            if (this->controllableEntity_ && !this->controllableEntity_->isInMouseLook())
            {
                this->updateTarget();

                if (!controlPaused_ )
                {
                    if (this->getControllableEntity() && ((this->getControllableEntity()->isExactlyA(ClassByString("SpaceShip")) || (this->getControllableEntity()->isExactlyA(ClassByString("ModularSpaceShip")))) || this->getControllableEntity()->isExactlyA(ClassByString("Rocket"))))
                        this->showOverlays();
                    else if (this->getControllableEntity() &&  this->getControllableEntity()->isExactlyA(ClassByString("FpsPlayer")))
                    {
                        this->showOverlays();
                        this->hideArrows();
                    }

                    this->crossHairOverlay_->setPosition(Vector2(static_cast<float>(this->currentYaw_)/2*-1+.5f-overlaySize_/2, static_cast<float>(this->currentPitch_)/2*-1+.5f-overlaySize_/2));

                    if (this->controlMode_ == 0 || (this->controlMode_ == 1 && this->firemode_ == 1))
                    {
                        if (this->showOverlays_ && this->showArrows_)
                            alignArrows();
                    }
                    else
                        hideArrows();

                    if (this->showDamageOverlay_ && (this->damageOverlayTT_ > 0 || this->damageOverlayTR_ > 0 || this->damageOverlayTB_ > 0 || this->damageOverlayTL_ > 0))
                    {
                        this->damageOverlayTT_ -= dt;
                        this->damageOverlayTR_ -= dt;
                        this->damageOverlayTB_ -= dt;
                        this->damageOverlayTL_ -= dt;
                        if (this->damageOverlayTT_ <= 0)
                            this->damageOverlayTop_->hide();
                        if (this->damageOverlayTR_ <= 0)
                            this->damageOverlayRight_->hide();
                        if (this->damageOverlayTB_ <= 0)
                            this->damageOverlayBottom_->hide();
                        if (this->damageOverlayTL_ <= 0)
                            this->damageOverlayLeft_->hide();
                    }
                }
            }
            else
                this->hideOverlays();

            if (this->acceleration_ > 0)
            {
                if (this->accelerating_)
                    HumanController::moveFrontBack(Vector2(1, 0));
                else
                    HumanController::moveFrontBack(Vector2(this->acceleration_, 0));
                this->accelerating_ = false;
                //HumanController::moveFrontBack(Vector2(clamp(this->acceleration_ + this->currentAcceleration_, 0.0f, 1.0f), 0));
            }
        }

        // Reset pitch and yaw rates
        // TODO: Reactivate this to fix the game pad problem with 0 input
        //this->currentPitch_ = 0;
        //this->currentYaw_ = 0;

        HumanController::tick(dt);
    }

    void NewHumanController::doFire(unsigned int firemode)
    {
        if (!this->controllableEntity_)
            return;

        this->firemode_ = firemode;

        if (firemode == 1 && this->controlMode_ == 1)
        {
            //unlocked steering, steer on right mouse click
            HumanController::yaw(Vector2(this->currentYaw_, 0));
            HumanController::pitch(Vector2(this->currentPitch_, 0));
        }
        else
            HumanController::doFire(firemode); //call for formationflight
    }

    void NewHumanController::hit(Pawn* originator, btManifoldPoint& contactpoint, float damage)
    {
        //Used in HumanController for formationFlight
        HumanController::hit(originator,contactpoint,damage);

        if (this->showDamageOverlay_ && !this->controlPaused_ && this->controllableEntity_ && !this->controllableEntity_->isInMouseLook())
        {
            Vector3 posA;
            if (originator)
                posA = originator->getWorldPosition();
            else
                posA = multi_cast<Vector3>(contactpoint.getPositionWorldOnA());
            //Vector3 posB = multi_cast<Vector3>(contactpoint.getPositionWorldOnB());
            //posA and posB are almost identical

            Vector3 relativeHit = this->getControllableEntity()->getWorldOrientation().UnitInverse() * (this->getControllableEntity()->getWorldPosition() - posA);

            //back is z positive
            //x is left positive
            //y is down positive
            relativeHit.normalise();

            float threshold = 0.3f;
            if (relativeHit.x > threshold) // Left
            {
                this->damageOverlayLeft_->show();
                this->damageOverlayTL_ = this->damageOverlayTime_;
                //this->damageOverlayLeft_->setBackgroundAlpha(0.3);
            }
            if (relativeHit.x < -threshold) //Right
            {
                this->damageOverlayRight_->show();
                this->damageOverlayTR_ = this->damageOverlayTime_;
                //this->damageOverlayRight_->setBackgroundAlpha(0.3);
            }
            if (relativeHit.y > threshold) //Top
            {
                this->damageOverlayBottom_->show();
                this->damageOverlayTB_ = this->damageOverlayTime_;
                //this->damageOverlayTop_->setBackgroundAlpha(0.3);
            }
            if (relativeHit.y < -threshold) //Bottom
            {
                this->damageOverlayTop_->show();
                this->damageOverlayTT_ = this->damageOverlayTime_;
                //this->damageOverlayBottom_->setBackgroundAlpha(0.3);
            }
        }
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

    void NewHumanController::centerCursor()
    {
        this->currentYaw_ = 0;
        this->currentPitch_ = 0;

        if( KeyBinderManager::exists() )
            KeyBinderManager::getInstance().getCurrent()->resetMouseAxes();
    }

    void NewHumanController::updateTarget()
    {
        Ogre::RaySceneQuery * rsq = HumanController::localController_s->getControllableEntity()->getScene()->getSceneManager()->createRayQuery(Ogre::Ray());

        Ogre::Ray mouseRay = HumanController::localController_s->getControllableEntity()->getCamera()->getOgreCamera()->getCameraToViewportRay(static_cast<float>(this->currentYaw_)/2*-1+.5f, static_cast<float>(this->currentPitch_)/2*-1+.5f);

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
        WorldEntity* myWe = static_cast<WorldEntity*>(this->getControllableEntity());

        Ogre::RaySceneQueryResult::iterator itr;
        for (itr = result.begin(); itr != result.end(); ++itr)
        {
//             orxout() << "testing object as target" << endl;
            if (itr->movable->isInScene() && itr->movable->getMovableType() == "Entity" && itr->distance > 200)
            {
                // Try to cast the user pointer
                WorldEntity* wePtr;
                try
                {
                    wePtr = orxonox_cast<WorldEntity*>(Ogre::any_cast<OrxonoxClass*>(itr->movable->getUserAny()));
                }
                catch (...)
                {
                    continue;
                }

                // make sure we don't shoot ourselves
                if( wePtr==myWe )
                    continue;

                if (wePtr)
                {
                    // go through all parents of object and look whether they are sightable or not
                    bool isSightable = false;
                    WorldEntity* parent = wePtr->getParent();
                    while (parent)
                    {
                        if (this->targetMask_.isExcluded(parent->getIdentifier()) || parent==myWe)
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
                    if (!isSightable)
                        continue;
                }

                if (this->getControllableEntity() && this->getControllableEntity()->getTarget() != wePtr)
                    this->getControllableEntity()->setTarget(wePtr);

                if (pawn)
                    pawn->setAimPosition( mouseRay.getOrigin() + mouseRay.getDirection() * itr->distance );

                //itr->movable->getParentSceneNode()->showBoundingBox(true);
                //return mouseRay.getOrigin() + mouseRay.getDirection() * itr->distance; //or itr->movable->getParentSceneNode()->_getDerivedPosition()
                return;
            }
        }

        if (pawn)
            pawn->setAimPosition( mouseRay.getOrigin() + mouseRay.getDirection() * 3000 );

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
        //SUPER(NewHumanController, yaw, value);
        if (this->controlMode_ == 0 || (this->controllableEntity_ && this->controllableEntity_->isInMouseLook()))
            HumanController::yaw(value);

        if (this->getControllableEntity() && !this->getControllableEntity()->isExactlyA(ClassByString("FpsPlayer")))
            this->currentYaw_ = value.x;
    }

    void NewHumanController::pitch(const Vector2& value)
    {
        //SUPER(NewHumanController, pitch, value);
        if (this->controlMode_ == 0 || (this->controllableEntity_ && this->controllableEntity_->isInMouseLook()))
            HumanController::pitch(value);

        if (this->getControllableEntity() && !this->getControllableEntity()->isExactlyA(ClassByString("FpsPlayer")))
            this->currentPitch_ = value.x;
    }

    void NewHumanController::changeMode()
    {
        if (NewHumanController::localController_s)
        {
            if (NewHumanController::localController_s->controlMode_ == 0)
            {
                NewHumanController::localController_s->controlMode_ = 1;
                NewHumanController::localController_s->hideArrows();
            }
            else
                NewHumanController::localController_s->controlMode_ = 0;
        }
    }

    void NewHumanController::changedControllableEntity()
    {
        this->controlMode_ = 0;
        this->centerCursor();
        if (this->getControllableEntity() && ((this->getControllableEntity()->isExactlyA(ClassByString("SpaceShip")) || (this->getControllableEntity()->isExactlyA(ClassByString("ModularSpaceShip")))) || this->getControllableEntity()->isExactlyA(ClassByString("Rocket"))))
        {
            this->showOverlays_ = true;
            if (!this->controlPaused_)
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
        if (NewHumanController::localController_s)
            NewHumanController::localController_s->acceleration_ = clamp(NewHumanController::localController_s->acceleration_ + 0.2f, 0.00f, 1.0f);
    }

    void NewHumanController::decelerate()
    {
        if (NewHumanController::localController_s)
            NewHumanController::localController_s->acceleration_ = clamp(NewHumanController::localController_s->acceleration_ - 0.1f, 0.0f, 1.0f);
    }

    void NewHumanController::doResumeControl()
    {
        this->controlPaused_ = false;
        if (this->showOverlays_)
            this->showOverlays();
    }

    void NewHumanController::doPauseControl()
    {
        this->controlPaused_ = true;
        this->hideOverlays();
    }

    void NewHumanController::alignArrows()
    {
        if (showArrows_)
        {
            hideArrows();

            float distance = sqrt(pow(static_cast<float>(this->currentYaw_)/2*-1,2) + pow(static_cast<float>(this->currentPitch_)/2*-1,2));

            if (distance > 0.04f && distance <= 0.59f * arrowsSize_ / 2.0f )
            {
                this->arrowsOverlay1_->setRotation(Degree(-90 + -1.0f * atan2(static_cast<float>(this->currentPitch_)/2*-1, static_cast<float>(this->currentYaw_)/2*-1) / math::twoPi * 360.0f));
                this->arrowsOverlay1_->show();
            }
            else if (distance > 0.59f * arrowsSize_ / 2.0f && distance <= 0.77f * arrowsSize_ / 2.0f )
            {
                this->arrowsOverlay2_->setRotation(Degree(-90 + -1.0f * atan2(static_cast<float>(this->currentPitch_)/2*-1, static_cast<float>(this->currentYaw_)/2*-1) / math::twoPi * 360.0f));
                this->arrowsOverlay2_->show();
            }
            else if (distance > 0.77f * arrowsSize_ / 2.0f && distance <= arrowsSize_ / 2.0f)
            {
                this->arrowsOverlay3_->setRotation(Degree(-90 + -1.0f * atan2(static_cast<float>(this->currentPitch_)/2*-1, static_cast<float>(this->currentYaw_)/2*-1) / math::twoPi * 360.0f));
                this->arrowsOverlay3_->show();
            }
            else if (distance > arrowsSize_ / 2.0f)
            {
                this->arrowsOverlay4_->setRotation(Degree(-90 + -1.0f * atan2(static_cast<float>(this->currentPitch_)/2*-1, static_cast<float>(this->currentYaw_)/2*-1) / math::twoPi * 360.0f));
                this->arrowsOverlay4_->show();
            }
        }
    }

    void NewHumanController::showOverlays()
    {
        if (!GameMode::showsGraphics())
            return;
        this->crossHairOverlay_->show();
        this->centerOverlay_->show();

        if (showArrows_)
        {
            this->arrowsOverlay1_->show();
            this->arrowsOverlay2_->show();
            this->arrowsOverlay3_->show();
            this->arrowsOverlay4_->show();
        }
    }

    void NewHumanController::hideOverlays()
    {
        if (!GameMode::showsGraphics())
            return;
        this->crossHairOverlay_->hide();
        this->centerOverlay_->hide();

        if (showDamageOverlay_)
        {
            this->damageOverlayTop_->hide();
            this->damageOverlayRight_->hide();
            this->damageOverlayBottom_->hide();
            this->damageOverlayLeft_->hide();
        }

        this->hideArrows();
    }

    void NewHumanController::hideArrows()
    {
        if(!GameMode::showsGraphics())
            return;
        if (showArrows_)
        {
            this->arrowsOverlay1_->hide();
            this->arrowsOverlay2_->hide();
            this->arrowsOverlay3_->hide();
            this->arrowsOverlay4_->hide();
        }
    }





}
