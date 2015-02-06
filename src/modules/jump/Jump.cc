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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Jump.cc
    @brief This is the gametype for the awesome minigame. Here the level is randomly created, the camera is shifted, ...
*/

#include "Jump.h"
#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/command/Executor.h"
#include "core/config/ConfigValueIncludes.h"
#include "gamestates/GSLevel.h"
#include "chat/ChatManager.h"
#include "JumpCenterpoint.h"
#include "JumpPlatform.h"
#include "JumpPlatformStatic.h"
#include "JumpPlatformHMove.h"
#include "JumpPlatformVMove.h"
#include "JumpPlatformDisappear.h"
#include "JumpPlatformTimer.h"
#include "JumpPlatformFake.h"
#include "JumpProjectile.h"
#include "JumpEnemy.h"
#include "JumpFigure.h"
#include "JumpItem.h"
#include "JumpSpring.h"
#include "JumpRocket.h"
#include "JumpPropeller.h"
#include "JumpBoots.h"
#include "JumpShield.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{
    RegisterUnloadableClass(Jump);

    Jump::Jump(Context* context) : Deathmatch(context)
    {
        RegisterObject(Jump);

        center_ = 0;
        figure_ = 0;
        camera = 0;
        setHUDTemplate("JumpHUD");

        setConfigValues();
    }

    Jump::~Jump()
    {
        if (isInitialized())
        {
            cleanup();
        }
    }

    void Jump::tick(float dt)
    {
        SUPER(Jump, tick, dt);

        if (figure_ != NULL)
        {
            Vector3 figurePosition = figure_->getPosition();
            Vector3 figureVelocity = figure_->getVelocity();

            float boundary = totalScreenShift_+center_->getCameraOffset();

            if (figurePosition.z > boundary)
            {
                screenShiftSinceLastUpdate_ += figurePosition.z - boundary;
                totalScreenShift_ = figurePosition.z - center_->getCameraOffset();

                // Create new platforms if needed
                if (screenShiftSinceLastUpdate_ > center_->getSectionLength())
                {
                    if (sectionNumber_ > 2 && sectionNumber_%4 == 0 && rand()%2 == 0 && figure_->propellerActive_ == false && figure_->rocketActive_ == false && addAdventure(adventureNumber_) == true)
                    {
                        screenShiftSinceLastUpdate_ -= 2*center_->getSectionLength();
                        ++ adventureNumber_;
                    }
                    else
                    {
                        screenShiftSinceLastUpdate_ -= center_->getSectionLength();
                        addSection();
                    }
                }
            }

            if (figurePosition.z < totalScreenShift_ - center_->getFieldDimension().y + platformHeight_ && figureVelocity.z < 0)
            {
                figure_->dead_ = true;
            }

            if (figure_->fireSignal_ == true)
            {
                if (figure_->dead_ == true)
                {
                    end();
                }
                else
                {
                    figure_->fireSignal_ = false;
                    addProjectile(figurePosition.x, figurePosition.z + figure_->getPropellerPos());
                }
            }


            if (camera != NULL)
            {
                Vector3 cameraPosition = Vector3(0, totalScreenShift_, 0);
                camera->setPosition(cameraPosition);
            }
            else
            {
                orxout() << "No camera found." << endl;
            }
        }

        ObjectList<JumpPlatform>::iterator beginPlatform = ObjectList<JumpPlatform>::begin();
        ObjectList<JumpPlatform>::iterator endPlatform = ObjectList<JumpPlatform>::end();
        ObjectList<JumpPlatform>::iterator itPlatform = beginPlatform;
        Vector3 platformPosition;

        while (itPlatform != endPlatform)
        {
            platformPosition = itPlatform->getPosition();
            if (platformPosition.z < totalScreenShift_ - center_->getFieldDimension().y)
            {
                ObjectList<JumpPlatform>::iterator temp = itPlatform;
                ++ itPlatform;
                center_->detach(*temp);
                temp->destroy();
            }
            else
            {
                ++ itPlatform;
            }
        }

        // Deleted deactivated platforms
        ObjectList<JumpPlatformDisappear>::iterator beginDisappear = ObjectList<JumpPlatformDisappear>::begin();
        ObjectList<JumpPlatformDisappear>::iterator endDisappear = ObjectList<JumpPlatformDisappear>::end();
        ObjectList<JumpPlatformDisappear>::iterator itDisappear = beginDisappear;

        while (itDisappear != endDisappear)
        {
            if (!itDisappear->isActive())
            {
                ObjectList<JumpPlatformDisappear>::iterator temp = itDisappear;
                ++ itDisappear;
                center_->detach(*temp);
                temp->destroy();
            }
            else
            {
                ++ itDisappear;
            }
        }

        ObjectList<JumpPlatformTimer>::iterator beginTimer = ObjectList<JumpPlatformTimer>::begin();
        ObjectList<JumpPlatformTimer>::iterator endTimer = ObjectList<JumpPlatformTimer>::end();
        ObjectList<JumpPlatformTimer>::iterator itTimer = beginTimer;

        while (itTimer != endTimer)
        {
            if (!itTimer->isActive())
            {
                ObjectList<JumpPlatformTimer>::iterator temp = itTimer;
                ++ itTimer;
                center_->detach(*temp);
                temp->destroy();
            }
            else
            {
                ++ itTimer;
            }
        }

        ObjectList<JumpProjectile>::iterator beginProjectile = ObjectList<JumpProjectile>::begin();
        ObjectList<JumpProjectile>::iterator endProjectile = ObjectList<JumpProjectile>::end();
        ObjectList<JumpProjectile>::iterator itProjectile = beginProjectile;
        Vector3 projectilePosition;

        while (itProjectile != endProjectile)
        {
            projectilePosition = itProjectile->getPosition();
            if (projectilePosition.z > totalScreenShift_ + 5*center_->getFieldDimension().y)
            {
                ObjectList<JumpProjectile>::iterator temp = itProjectile;
                ++ itProjectile;
                center_->detach(*temp);
                temp->destroy();
            }
            else
            {
                ++ itProjectile;
            }
        }

        ObjectList<JumpEnemy>::iterator beginEnemy = ObjectList<JumpEnemy>::begin();
        ObjectList<JumpEnemy>::iterator endEnemy = ObjectList<JumpEnemy>::end();
        ObjectList<JumpEnemy>::iterator itEnemy = beginEnemy;
        Vector3 enemyPosition;

        while (itEnemy != endEnemy)
        {
            enemyPosition = itEnemy->getPosition();
            if (enemyPosition.z < totalScreenShift_ - center_->getFieldDimension().y || itEnemy->dead_ == true)
            {
                ObjectList<JumpEnemy>::iterator temp = itEnemy;
                ++ itEnemy;
                center_->detach(*temp);
                temp->destroy();
            }
            else
            {
                ++ itEnemy;
            }
        }

        ObjectList<JumpItem>::iterator beginItem = ObjectList<JumpItem>::begin();
        ObjectList<JumpItem>::iterator endItem = ObjectList<JumpItem>::end();
        ObjectList<JumpItem>::iterator itItem = beginItem;
        Vector3 itemPosition;

        while (itItem != endItem)
        {
            itemPosition = itItem->getPosition();

            WorldEntity* parent = itItem->getParent();

            if (itItem->attachedToFigure_ == false && itemPosition.z < totalScreenShift_ - center_->getFieldDimension().y && parent == center_)
            {
                ObjectList<JumpItem>::iterator temp = itItem;
                ++ itItem;
                center_->detach(*temp);
                temp->destroy();
            }
            else
            {
                ++ itItem;
            }
        }
    }

    void Jump::cleanup()
    {
        camera = 0;
    }

    void Jump::start()
    {
        if (center_ != NULL) // There needs to be a JumpCenterpoint, i.e. the area the game takes place.
        {
            if (figure_ == NULL)
            {
                figure_ = new JumpFigure(center_->getContext());
                figure_->addTemplate(center_->getFigureTemplate());
                figure_->InitializeAnimation(center_->getContext());
            }

            center_->attach(figure_);
            figure_->setPosition(0, 0, 0);
            figure_->setFieldDimension(center_->getFieldDimension());
        }
        else // If no centerpoint was specified, an error is thrown and the level is exited.
        {
            orxout(internal_error) << "Jump: No Centerpoint specified." << endl;
            GSLevel::startMainMenu();
            return;
        }

        // Call start for the parent class.
        Deathmatch::start();

        if (figure_ != NULL)
        {
            camera = figure_->getCamera();
        }

        totalScreenShift_ = 0.0;
        screenShiftSinceLastUpdate_ = 0.0;
        sectionNumber_ = 0;
        adventureNumber_ = 0;

        addStartSection();
        addSection();
        addSection();
    }

    void Jump::end()
    {
        cleanup();
        GSLevel::startMainMenu();

        Deathmatch::end();
    }

    void Jump::spawnPlayer(PlayerInfo* player)
    {
        assert(player);

        if (figure_->getPlayer() == NULL)
        {
            player->startControl(figure_);
            players_[player].state_ = PlayerState::Alive;
        }
    }

    PlayerInfo* Jump::getPlayer() const
    {
        if (this->figure_ != NULL)
        {
            return this->figure_->getPlayer();
        }
        else
        {
            return 0;
        }
    }

    void Jump::addPlatform(JumpPlatform* newPlatform, std::string platformTemplate, float xPosition, float zPosition)
    {
        if (newPlatform != NULL && center_ != NULL)
        {
            newPlatform->addTemplate(platformTemplate);
            newPlatform->setPosition(Vector3(xPosition, 0.0, zPosition));
            newPlatform->setFigure(this->figure_);
            center_->attach(newPlatform);
        }
    }

    JumpPlatformStatic* Jump::addPlatformStatic(float xPosition, float zPosition)
    {
        JumpPlatformStatic* newPlatform = new JumpPlatformStatic(center_->getContext());
        addPlatform(newPlatform, center_->getPlatformStaticTemplate(), xPosition, zPosition);

        return newPlatform;
    }

    JumpPlatformHMove* Jump::addPlatformHMove(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float speed)
    {
        JumpPlatformHMove* newPlatform = new JumpPlatformHMove(center_->getContext());
        newPlatform->setProperties(leftBoundary, rightBoundary, speed);
        addPlatform(newPlatform, center_->getPlatformHMoveTemplate(), xPosition, zPosition);

        return newPlatform;
    }

    JumpPlatformVMove* Jump::addPlatformVMove(float xPosition, float zPosition, float lowerBoundary, float upperBoundary, float speed)
    {
        JumpPlatformVMove* newPlatform = new JumpPlatformVMove(center_->getContext());
        newPlatform->setProperties(lowerBoundary, upperBoundary, speed);
        addPlatform(newPlatform, center_->getPlatformVMoveTemplate(), xPosition, zPosition);

        return newPlatform;
    }

    JumpPlatformDisappear* Jump::addPlatformDisappear(float xPosition, float zPosition)
    {
        JumpPlatformDisappear* newPlatform = new JumpPlatformDisappear(center_->getContext());
        newPlatform->setProperties(true);
        addPlatform(newPlatform, center_->getPlatformDisappearTemplate(), xPosition, zPosition);

        return newPlatform;
    }

    JumpPlatformTimer* Jump::addPlatformTimer(float xPosition, float zPosition, float time, float variance)
    {
        float additionalTime = (float)(rand()%100)/100.0f*variance - variance/2.0f;

        JumpPlatformTimer* newPlatform = new JumpPlatformTimer(center_->getContext());
        newPlatform->setProperties(time + additionalTime);
        addPlatform(newPlatform, center_->getPlatformTimerTemplate(), xPosition, zPosition);

        return newPlatform;
    }

    JumpPlatformFake* Jump::addPlatformFake(float xPosition, float zPosition)
    {
        JumpPlatformFake* newPlatform = new JumpPlatformFake(center_->getContext());
        addPlatform(newPlatform, center_->getPlatformFakeTemplate(), xPosition, zPosition);
        newPlatform->setAngularVelocity(Vector3(0, 0, 2.0));

        return newPlatform;
    }


    void Jump::addProjectile(float xPosition, float zPosition)
    {
        JumpProjectile* newProjectile = new JumpProjectile(center_->getContext());
        if (newProjectile != NULL && center_ != NULL)
        {
            newProjectile->addTemplate(center_->getProjectileTemplate());
            newProjectile->setPosition(Vector3(xPosition, 0.0, zPosition));
            newProjectile->setFieldDimension(center_->getFieldDimension());
            newProjectile->setFigure(this->figure_);
            center_->attach(newProjectile);
        }
    }

    void Jump::addSpring(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity)
    {
        JumpSpring* newSpring = new JumpSpring(center_->getContext());
        if (newSpring != NULL && center_ != NULL)
        {
            newSpring->addTemplate(center_->getSpringTemplate());
            newSpring->setPosition(Vector3(xPosition, 0.0, zPosition));
            newSpring->setProperties(leftBoundary, rightBoundary, lowerBoundary, upperBoundary, xVelocity, zVelocity);
            newSpring->setFigure(figure_);
            center_->attach(newSpring);
        }
    }

    void Jump::addSpring(JumpPlatform* platform)
    {
        JumpSpring* newSpring = new JumpSpring(center_->getContext());
        if (newSpring != NULL && center_ != NULL)
        {
            newSpring->addTemplate(center_->getSpringTemplate());
            newSpring->setPosition(Vector3(0.0, 0.0, 0.0));
            newSpring->setProperties(-10.0, 10.0, -10.0, 10.0, 0.0, 0.0);
            newSpring->setFigure(figure_);
            platform->attach(newSpring);
        }
    }

    void Jump::addRocket(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity)
    {
        JumpRocket* newRocket = new JumpRocket(center_->getContext());
        if (newRocket != NULL && center_ != NULL)
        {
            newRocket->addTemplate(center_->getRocketTemplate());
            newRocket->setPosition(Vector3(xPosition, 0.0, zPosition));
            newRocket->setProperties(leftBoundary, rightBoundary, lowerBoundary, upperBoundary, xVelocity, zVelocity);
            newRocket->setFigure(figure_);
            center_->attach(newRocket);
        }
    }

    void Jump::addRocket(JumpPlatform* platform)
    {
        JumpRocket* newRocket = new JumpRocket(center_->getContext());
        if (newRocket != NULL && center_ != NULL)
        {
            newRocket->addTemplate(center_->getRocketTemplate());
            newRocket->setPosition(Vector3(0.0, 0.0, 0.0));
            newRocket->setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            newRocket->setFigure(figure_);
            platform->attach(newRocket);
        }
    }

    void Jump::addPropeller(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity)
    {
        JumpPropeller* newPropeller = new JumpPropeller(center_->getContext());
        if (newPropeller != NULL && center_ != NULL)
        {
            newPropeller->addTemplate(center_->getPropellerTemplate());
            newPropeller->setPosition(Vector3(xPosition, 0.0, zPosition));
            newPropeller->setProperties(leftBoundary, rightBoundary, lowerBoundary, upperBoundary, xVelocity, zVelocity);
            newPropeller->setFigure(figure_);
            center_->attach(newPropeller);
        }
    }

    void Jump::addPropeller(JumpPlatform* platform)
    {
        JumpPropeller* newPropeller = new JumpPropeller(center_->getContext());
        if (newPropeller != NULL && center_ != NULL)
        {
            newPropeller->addTemplate(center_->getPropellerTemplate());
            newPropeller->setPosition(Vector3(0.0, 0.0, 0.0));
            newPropeller->setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            newPropeller->setFigure(figure_);
            platform->attach(newPropeller);
        }
    }

    void Jump::addBoots(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity)
    {
        JumpBoots* newBoots = new JumpBoots(center_->getContext());
        if (newBoots != NULL && center_ != NULL)
        {
            newBoots->addTemplate(center_->getBootsTemplate());
            newBoots->setPosition(Vector3(xPosition, 0.0, zPosition));
            newBoots->setProperties(leftBoundary, rightBoundary, lowerBoundary, upperBoundary, xVelocity, zVelocity);
            newBoots->setFigure(figure_);
            center_->attach(newBoots);
        }
    }

    void Jump::addBoots(JumpPlatform* platform)
    {
        JumpBoots* newBoots = new JumpBoots(center_->getContext());
        if (newBoots != NULL && center_ != NULL)
        {
            newBoots->addTemplate(center_->getBootsTemplate());
            newBoots->setPosition(Vector3(0.0, 0.0, 0.0));
            newBoots->setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            newBoots->setFigure(figure_);
            platform->attach(newBoots);
        }
    }

    void Jump::addShield(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity)
    {
        JumpShield* newShield = new JumpShield(center_->getContext());
        if (newShield != NULL && center_ != NULL)
        {
            newShield->addTemplate(center_->getShieldTemplate());
            newShield->setPosition(Vector3(xPosition, 0.0, zPosition));
            newShield->setProperties(leftBoundary, rightBoundary, lowerBoundary, upperBoundary, xVelocity, zVelocity);
            newShield->setFigure(figure_);
            center_->attach(newShield);
        }
    }

    void Jump::addShield(JumpPlatform* platform)
    {
        JumpShield* newShield = new JumpShield(center_->getContext());
        if (newShield != NULL && center_ != NULL)
        {
            newShield->addTemplate(center_->getShieldTemplate());
            newShield->setPosition(Vector3(0.0, 0.0, 0.0));
            newShield->setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            newShield->setFigure(figure_);
            platform->attach(newShield);
        }
    }

    void Jump::addEnemy(int type, float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity)
    {
        JumpEnemy* newEnemy = new JumpEnemy(center_->getContext());
        if (newEnemy != NULL && center_ != NULL)
        {
            switch (type)
            {
            case 1:
                newEnemy->addTemplate(center_->getEnemy1Template());
                break;
            case 2:
                newEnemy->addTemplate(center_->getEnemy2Template());
                break;
            case 3:
                newEnemy->addTemplate(center_->getEnemy3Template());
                break;
            case 4:
                newEnemy->addTemplate(center_->getEnemy4Template());
                break;
            default:
                return;
            }

            newEnemy->setPosition(Vector3(xPosition, 0.0, zPosition));
            newEnemy->setProperties(leftBoundary, rightBoundary, lowerBoundary, upperBoundary, xVelocity, zVelocity);
            newEnemy->setFieldDimension(center_->getFieldDimension());
            newEnemy->setFigure(this->figure_);
            center_->attach(newEnemy);
        }
    }

    void Jump::addStartSection()
    {
        JumpPlatform* newPlatform;

        float sectionLength = center_->getSectionLength();

        newPlatform = new JumpPlatformStatic(center_->getContext());
        addPlatform(newPlatform, center_->getPlatformStaticTemplate(), 0.0, -0.05f*sectionLength);

        platformWidth_ = newPlatform->getWidth();
        platformHeight_ = newPlatform->getHeight();

        for (float xPosition = platformWidth_; xPosition <= center_->getFieldDimension().x; xPosition += platformWidth_)
        {
            newPlatform = new JumpPlatformStatic(center_->getContext());
            addPlatform(newPlatform, center_->getPlatformStaticTemplate(), xPosition, -0.05f*sectionLength);
            newPlatform = new JumpPlatformStatic(center_->getContext());
            addPlatform(newPlatform, center_->getPlatformStaticTemplate(), -xPosition, -0.05f*sectionLength);
        }
    }

    void Jump::addSection()
    {
        float fieldWidth = center_->getFieldDimension().x;
        float sectionLength = center_->getSectionLength();
        float sectionBegin = sectionNumber_ * sectionLength;
        float sectionEnd = (1 + sectionNumber_) * sectionLength;

        const int numI = 6;
        const int numJ = 4;

        enum PlatformType
        {
            PLATFORM_EMPTY, PLATFORM_STATIC, PLATFORM_HMOVE, PLATFORM_VMOVE, PLATFORM_DISAPPEAR, PLATFORM_TIMER, PLATFORM_FAKE
        };

        enum ItemType
        {
            ITEM_NOTHING, ITEM_SPRING, ITEM_PROPELLER, ITEM_ROCKET, ITEM_BOOTS, ITEM_SHIELD
        };

        struct PlatformMatrix
        {
            PlatformType type;
            bool done;
        } matrix[numI][numJ];


        for (int i = 0; i < numI; ++i)
        {
            for (int j = 0; j < numJ; ++j)
            {
                matrix[i][j].type = PLATFORM_EMPTY;
                matrix[i][j].done = false;
            }
        }
        PlatformType platformtype1;
        PlatformType platformtype2;
        ItemType itemType = ITEM_NOTHING;

        if (rand()%2 == 0)
        {
            itemType = ITEM_SPRING;
        }
        else if (rand()%2 == 0 && sectionNumber_ > 3)
        {
            switch(rand()%4)
            {
            case 0:
                itemType = ITEM_PROPELLER;
                break;
            case 1:
                itemType = ITEM_ROCKET;
                break;
            case 2:
                itemType = ITEM_BOOTS;
                break;
            case 3:
                itemType = ITEM_SHIELD;
                break;
            default:
                break;
            }
        }

        switch((sectionNumber_ > 28) ? rand()%29 : rand()%(sectionNumber_+1))
        {
        case 0:
            platformtype1 = PLATFORM_STATIC;
            platformtype2 = PLATFORM_STATIC;
            break;
        case 1:
            platformtype1 = PLATFORM_STATIC;
            platformtype2 = PLATFORM_STATIC;
            break;
        case 2:
            platformtype1 = PLATFORM_STATIC;
            platformtype2 = PLATFORM_HMOVE;
            break;
        case 3:
            platformtype1 = PLATFORM_STATIC;
            platformtype2 = PLATFORM_DISAPPEAR;
            break;
        case 4:
            platformtype1 = PLATFORM_STATIC;
            platformtype2 = PLATFORM_VMOVE;
            break;
        case 5:
            platformtype1 = PLATFORM_STATIC;
            platformtype2 = PLATFORM_TIMER;
            break;
        case 6:
            platformtype1 = PLATFORM_HMOVE;
            platformtype2 = PLATFORM_STATIC;
            break;
        case 7:
            platformtype1 = PLATFORM_HMOVE;
            platformtype2 = PLATFORM_HMOVE;
            break;
        case 8:
            platformtype1 = PLATFORM_HMOVE;
            platformtype2 = PLATFORM_HMOVE;
            break;
        case 9:
            platformtype1 = PLATFORM_HMOVE;
            platformtype2 = PLATFORM_DISAPPEAR;
            break;
        case 10:
            platformtype1 = PLATFORM_HMOVE;
            platformtype2 = PLATFORM_VMOVE;
            break;
        case 11:
            platformtype1 = PLATFORM_HMOVE;
            platformtype2 = PLATFORM_TIMER;
            break;
        case 12:
            platformtype1 = PLATFORM_DISAPPEAR;
            platformtype2 = PLATFORM_STATIC;
            break;
        case 13:
            platformtype1 = PLATFORM_DISAPPEAR;
            platformtype2 = PLATFORM_HMOVE;
            break;
        case 14:
            platformtype1 = PLATFORM_DISAPPEAR;
            platformtype2 = PLATFORM_DISAPPEAR;
            break;
        case 15:
            platformtype1 = PLATFORM_DISAPPEAR;
            platformtype2 = PLATFORM_DISAPPEAR;
            break;
        case 16:
            platformtype1 = PLATFORM_DISAPPEAR;
            platformtype2 = PLATFORM_VMOVE;
            break;
        case 17:
            platformtype1 = PLATFORM_DISAPPEAR;
            platformtype2 = PLATFORM_TIMER;
            break;
        case 18:
            platformtype1 = PLATFORM_VMOVE;
            platformtype2 = PLATFORM_STATIC;
            break;
        case 19:
            platformtype1 = PLATFORM_VMOVE;
            platformtype2 = PLATFORM_HMOVE;
            break;
        case 20:
            platformtype1 = PLATFORM_VMOVE;
            platformtype2 = PLATFORM_DISAPPEAR;
            break;
        case 21:
            platformtype1 = PLATFORM_VMOVE;
            platformtype2 = PLATFORM_VMOVE;
            break;
        case 22:
            platformtype1 = PLATFORM_VMOVE;
            platformtype2 = PLATFORM_VMOVE;
            break;
        case 23:
            platformtype1 = PLATFORM_VMOVE;
            platformtype2 = PLATFORM_TIMER;
            break;
        case 24:
            platformtype1 = PLATFORM_TIMER;
            platformtype2 = PLATFORM_STATIC;
            break;
        case 25:
            platformtype1 = PLATFORM_TIMER;
            platformtype2 = PLATFORM_HMOVE;
            break;
        case 26:
            platformtype1 = PLATFORM_TIMER;
            platformtype2 = PLATFORM_DISAPPEAR;
            break;
        case 27:
            platformtype1 = PLATFORM_TIMER;
            platformtype2 = PLATFORM_VMOVE;
            break;
        case 28:
            platformtype1 = PLATFORM_TIMER;
            platformtype2 = PLATFORM_TIMER;
            break;
        default:
            platformtype1 = PLATFORM_TIMER;
            platformtype2 = PLATFORM_TIMER;
            break;
        }

        // Fill matrix with selected platform types
        for (int i = 0; i < numI; ++ i)
        {
            for (int j = 0; j < numJ; ++ j)
            {
                if (rand()%3 == 0)
                {
                    matrix[i][j].type = platformtype1;
                }
                else
                {
                    matrix[i][j].type = platformtype2;
                }
                matrix[i][j].done = false;
            }
        }

        if (platformtype1 == platformtype2 && sectionNumber_ > 10 && rand()%2 == 0)
        {
            matrix[rand()%numI][rand()%numJ].type = PLATFORM_EMPTY;
            matrix[rand()%numI][rand()%numJ].type = PLATFORM_EMPTY;
        }

        // Delete some planned platforms or replace them with fake platforms
        if (sectionNumber_ > 5)
        {
            if (rand()%2 == 0)
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_EMPTY;
            }
            else
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_FAKE;
            }
        }
        else if (sectionNumber_ > 10)
        {
            if (rand()%2 == 0)
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_EMPTY;
            }
            else
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_FAKE;
            }
            if (rand()%2 == 0)
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_EMPTY;
            }
            else
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_FAKE;
            }
        }
        else if (sectionNumber_ > 15)
        {
            if (rand()%2 == 0)
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_EMPTY;
            }
            else
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_FAKE;
            }
            if (rand()%2 == 0)
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_EMPTY;
            }
            else
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_FAKE;
            }
            if (rand()%2 == 0)
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_EMPTY;
            }
            else
            {
                matrix[rand()%numI][rand()%numJ].type = PLATFORM_FAKE;
            }
        }

        std::vector<JumpPlatform*> platformList;

        for (int i = 0; i < numI; ++ i)
        {
            for (int j = 0; j < numJ; ++ j)
            {
                if (matrix[i][j].done == false)
                {
                    float xPosition = 0.0;
                    float zPosition = 0.0;
                    float leftBoundary = 0.0;
                    float rightBoundary = 0.0;
                    float lowerBoundary = 0.0;
                    float upperBoundary = 0.0;
                    float xVelocity = 0.0;
                    float zVelocity = 0.0;

                    switch(matrix[i][j].type)
                    {
                    case PLATFORM_EMPTY:
                        matrix[i][j].done = true;
                        break;
                    case PLATFORM_STATIC:
                        xPosition = randomXPosition(numJ, j);
                        zPosition = sectionBegin + i*sectionLength/numI;
                        platformList.push_back(addPlatformStatic(xPosition, zPosition));
                        matrix[i][j].done = true;
                        break;
                    case PLATFORM_FAKE:
                        xPosition = randomXPosition(numJ, j);
                        zPosition = sectionBegin + i*sectionLength/numI;
                        platformList.push_back(addPlatformFake(xPosition, zPosition));
                        matrix[i][j].done = true;
                        break;
                    case PLATFORM_TIMER:
                        xPosition = randomXPosition(numJ, j);
                        zPosition = sectionBegin + i*sectionLength/numI;
                        platformList.push_back(addPlatformTimer(xPosition, zPosition, 12.0, 1.5));
                        matrix[i][j].done = true;
                        break;
                    case PLATFORM_DISAPPEAR:
                        xPosition = randomXPosition(numJ, j);
                        zPosition = sectionBegin + i*sectionLength/numI;
                        platformList.push_back(addPlatformDisappear(xPosition, zPosition));
                        matrix[i][j].done = true;
                        break;
                    case PLATFORM_HMOVE:
                        xVelocity = randomSpeed();
                        if (j <= numJ-3 && matrix[i][j+1].type == PLATFORM_HMOVE && matrix[i][j+2].type == PLATFORM_HMOVE && rand()%2 == 0)
                        {
                            leftBoundary = randomXPositionLeft(numJ, j);
                            rightBoundary = randomXPositionRight(numJ, j+2);
                            xPosition = randomPosition(leftBoundary, rightBoundary);
                            zPosition = sectionBegin + i*sectionLength/numI;
                            platformList.push_back(addPlatformHMove(xPosition, zPosition, leftBoundary, rightBoundary, xVelocity));
                            matrix[i][j].done = true;
                            matrix[i][j+1].done = true;
                            matrix[i][j+2].done = true;
                        }
                        else if (j <= numJ-2 && matrix[i][j+1].type == PLATFORM_HMOVE && rand()%2 == 0)
                        {
                            leftBoundary = randomXPositionLeft(numJ, j);
                            rightBoundary = randomXPositionRight(numJ, j+1);
                            xPosition = randomPosition(leftBoundary, rightBoundary);
                            zPosition = sectionBegin + i*sectionLength/numI;
                            platformList.push_back(addPlatformHMove(xPosition, zPosition, leftBoundary, rightBoundary, xVelocity));
                            matrix[i][j].done = true;
                            matrix[i][j+1].done = true;
                        }
                        else
                        {
                            leftBoundary = randomXPositionLeft(numJ, j);
                            rightBoundary = randomXPositionRight(numJ, j);
                            xPosition = randomPosition(leftBoundary, rightBoundary);
                            zPosition = sectionBegin + i*sectionLength/numI;
                            platformList.push_back(addPlatformHMove(xPosition, zPosition, leftBoundary, rightBoundary, xVelocity));
                            matrix[i][j].done = true;
                        }
                        break;
                    case PLATFORM_VMOVE:
                        zVelocity = randomSpeed();
                        if (i <= numI-3 && matrix[i+1][j].type == PLATFORM_VMOVE && matrix[i+2][j].type == PLATFORM_VMOVE && rand()%2 == 0)
                        {
                            lowerBoundary = randomZPositionLower(numI, i, sectionBegin, sectionEnd);
                            upperBoundary = randomZPositionUpper(numI, i+2, sectionBegin, sectionEnd);
                            zPosition = randomPosition(lowerBoundary, upperBoundary);
                            xPosition = randomXPosition(numJ, j);
                            platformList.push_back(addPlatformVMove(xPosition, zPosition, lowerBoundary, upperBoundary, zVelocity));
                            matrix[i][j].done = true;
                            matrix[i+1][j].done = true;
                            matrix[i+2][j].done = true;
                        }
                        else if (i <= numI-2 && matrix[i+1][j].type == PLATFORM_VMOVE && rand()%2 == 0)
                        {
                            lowerBoundary = randomZPositionLower(numI, i, sectionBegin, sectionEnd);
                            upperBoundary = randomZPositionUpper(numI, i+1, sectionBegin, sectionEnd);
                            zPosition = randomPosition(lowerBoundary, upperBoundary);
                            xPosition = randomXPosition(numJ, j);
                            platformList.push_back(addPlatformVMove(xPosition, zPosition, lowerBoundary, upperBoundary, zVelocity));
                            matrix[i][j].done = true;
                            matrix[i+1][j].done = true;
                        }
                        else
                        {
                            lowerBoundary = randomZPositionLower(numI, i, sectionBegin, sectionEnd);
                            upperBoundary = randomZPositionUpper(numI, i, sectionBegin, sectionEnd);
                            zPosition = randomPosition(lowerBoundary, upperBoundary);
                            xPosition = randomXPosition(numJ, j);
                            platformList.push_back(addPlatformVMove(xPosition, zPosition, lowerBoundary, upperBoundary, zVelocity));
                            matrix[i][j].done = true;
                        }
                        break;
                    default:
                        //ERROR
                        break;

                    }
                }
            }
        }

        //Add items
        int numNewPlatforms = platformList.size();

        if (rand()%4 == 0)
        {
            if (rand()%2 == 0)
            {
                switch (itemType)
                {
                case ITEM_ROCKET:
                    addRocket(0.0, randomPosition(sectionBegin, sectionBegin + sectionLength), -fieldWidth/2, fieldWidth/2, 0.0, 0.0, randomSpeed(), 0.0);
                    break;
                case ITEM_PROPELLER:
                    addPropeller(0.0, randomPosition(sectionBegin, sectionBegin + sectionLength), -fieldWidth/2, fieldWidth/2, 0.0, 0.0, randomSpeed(), 0.0);
                    break;
                case ITEM_BOOTS:
                    addBoots(0.0, randomPosition(sectionBegin, sectionBegin + sectionLength), -fieldWidth/2, fieldWidth/2, 0.0, 0.0, randomSpeed(), 0.0);
                    break;
                case ITEM_SHIELD:
                    addShield(0.0, randomPosition(sectionBegin, sectionBegin + sectionLength), -fieldWidth/2, fieldWidth/2, 0.0, 0.0, randomSpeed(), 0.0);
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch (itemType)
                {
                case ITEM_ROCKET:
                    addRocket(randomPosition(-fieldWidth/2, fieldWidth/2), sectionBegin + sectionLength/2, 0.0, 0.0, sectionBegin, sectionEnd, 0.0, randomSpeed());
                    break;
                case ITEM_PROPELLER:
                    addPropeller(randomPosition(-fieldWidth/2, fieldWidth/2), sectionBegin + sectionLength/2, 0.0, 0.0, sectionBegin, sectionEnd, 0.0, randomSpeed());
                    break;
                case ITEM_BOOTS:
                    addBoots(randomPosition(-fieldWidth/2, fieldWidth/2), sectionBegin + sectionLength/2, 0.0, 0.0, sectionBegin, sectionEnd, 0.0, randomSpeed());
                    break;
                case ITEM_SHIELD:
                    addShield(randomPosition(-fieldWidth/2, fieldWidth/2), sectionBegin + sectionLength/2, 0.0, 0.0, sectionBegin, sectionEnd, 0.0, randomSpeed());
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            if (numNewPlatforms > 0)
            {
                JumpPlatform* itemPlatform = platformList[rand()%numNewPlatforms];

                switch (itemType)
                {
                case ITEM_SPRING:
                    addSpring(itemPlatform);
                    break;
                case ITEM_ROCKET:
                    addRocket(itemPlatform);
                    break;
                case ITEM_PROPELLER:
                    addPropeller(itemPlatform);
                    break;
                case ITEM_BOOTS:
                    addBoots(itemPlatform);
                    break;
                case ITEM_SHIELD:
                    addShield(itemPlatform);
                    break;
                default:
                    break;
                }
            }
        }

        if (sectionNumber_ >= 5 && rand()%3 == 0)
        {
            switch(rand()%4)
            {
            case 0:
                addEnemy(1, randomXPosition(), randomPosition(sectionBegin, sectionBegin + sectionLength), -fieldWidth/3*2, fieldWidth/3*2, sectionBegin, sectionBegin + sectionLength, randomSpeed(), 0.0);
                break;
            case 1:
                addEnemy(2, randomXPosition(), randomPosition(sectionBegin, sectionBegin + sectionLength), -fieldWidth/3*2, fieldWidth/3*2, sectionBegin, sectionBegin + sectionLength, randomSpeed(), 0.0);
                break;
            case 2:
                addEnemy(3, randomXPosition(), randomPosition(sectionBegin, sectionBegin + sectionLength), -fieldWidth/3*2, fieldWidth/3*2, sectionBegin, sectionBegin + sectionLength, randomSpeed(), 0.0);
                break;
            case 3:
                addEnemy(4, randomXPosition(), randomPosition(sectionBegin, sectionBegin + sectionLength), -fieldWidth/3*2, fieldWidth/3*2, sectionBegin, sectionBegin + sectionLength, randomSpeed(), 0.0);
                break;
            }
        }
        ++ sectionNumber_;
    }

    bool Jump::addAdventure(int number)
    {
        float fieldWidth = center_->getFieldDimension().x;
        float sectionLength = center_->getSectionLength();
        float sectionBegin = sectionNumber_ * sectionLength;
        sectionLength *= 2;

        switch(number)
        {
        case 0:
        {
            int numI = 10;
            if (rand()%2 == 0)
            {
                for (int i = 0; i < numI; ++ i)
                {
                    addPlatformStatic((2*fieldWidth-platformWidth_)*i/numI-fieldWidth+platformWidth_/2, sectionBegin+i*sectionLength/numI);
                }
            }
            else
            {
                for (int i = 0; i < numI; ++ i)
                {
                    addPlatformStatic((2*fieldWidth-platformWidth_)*i/numI-fieldWidth+platformWidth_/2, sectionBegin+(numI-i)*sectionLength/numI);
                }
            }
            break;
        }
        case 1:
        {
            int numI = 7;

            addPlatformStatic(0.0, sectionBegin);
            if (rand()%2 == 0)
            {
                for (int i = 1; i < numI; ++ i)
                {
                    addPlatformStatic((fieldWidth-platformWidth_/2)*i/numI, sectionBegin+i*sectionLength/numI);
                    addPlatformStatic(-(fieldWidth-platformWidth_/2)*i/numI, sectionBegin+i*sectionLength/numI);
                }
            }
            else
            {
                for (int i = 1; i < numI; ++ i)
                {
                    addPlatformStatic((fieldWidth-platformWidth_/2)*i/numI, sectionBegin+(numI-i)*sectionLength/numI);
                    addPlatformStatic(-(fieldWidth-platformWidth_/2)*i/numI, sectionBegin+(numI-i)*sectionLength/numI);
                }
            }
            break;
        }
        case 2:
        {
            int numI = 5;
            for (int i = 0; i <= numI; ++ i)
            {
                addPlatformStatic((2*fieldWidth-platformWidth_)*i/numI-fieldWidth, sectionBegin);
                addPlatformStatic((2*fieldWidth-platformWidth_)*i/numI-fieldWidth, sectionBegin+sectionLength/5);
                addPlatformStatic((2*fieldWidth-platformWidth_)*i/numI-fieldWidth, sectionBegin+sectionLength*2/5);
                addPlatformStatic((2*fieldWidth-platformWidth_)*i/numI-fieldWidth, sectionBegin+sectionLength*3/5);
                addPlatformStatic((2*fieldWidth-platformWidth_)*i/numI-fieldWidth, sectionBegin+sectionLength*4/5);
                addEnemy(4, (2*fieldWidth-platformWidth_)*i/numI-fieldWidth+platformWidth_/2, sectionBegin+sectionLength/2, -fieldWidth, fieldWidth, sectionBegin, sectionBegin + sectionLength, 0.0, 0.0);
            }
            break;
        }
        case 3:
        {
            addRocket(addPlatformStatic(0.0, sectionBegin));
            addEnemy(2, 0.0, sectionBegin+sectionLength/5, 0.0,0.0, 0.0, 0.0, 0.0, 0.0);
            break;
        }
        case 4:
        {
            addPropeller(addPlatformStatic(0.0, sectionBegin));
            addPropeller(addPlatformStatic(-fieldWidth/2, sectionBegin));
            addPropeller(addPlatformStatic(fieldWidth/2, sectionBegin));
            break;
        }
        default:
            return false;
        }
        sectionNumber_ +=2;
        return true;
    }

    float Jump::randomXPosition()
    {
        float fieldWidth = center_->getFieldDimension().x;

        return (float)(rand()%(2*(int)fieldWidth)) - fieldWidth;
    }

    float Jump::randomXPosition(int totalColumns, int culomn)
    {
        float fieldWidth = center_->getFieldDimension().x;

        float halfWidth = fieldWidth/totalColumns;
        float leftBound = culomn*halfWidth*2-fieldWidth;
        float rightBound = leftBound + 2*halfWidth;

        return randomPosition(leftBound+platformWidth_/2, rightBound-platformWidth_/2);
    }

    float Jump::randomXPositionLeft(int totalColumns, int culomn)
    {
        float fieldWidth = center_->getFieldDimension().x;

        float halfWidth = fieldWidth/totalColumns;
        float leftBound = culomn*halfWidth*2-fieldWidth;
        float rightBound = leftBound + 2*halfWidth/3;

        return randomPosition(leftBound+platformWidth_/2, rightBound-platformWidth_/2);
    }

    float Jump::randomXPositionRight(int totalColumns, int culomn)
    {
        float fieldWidth = center_->getFieldDimension().x;

        float halfWidth = fieldWidth/totalColumns;
        float rightBound = (culomn+1)*halfWidth*2-fieldWidth;
        float leftBound = rightBound - 2*halfWidth/3;
        return randomPosition(leftBound+platformWidth_/2, rightBound-platformWidth_/2);
    }

    float Jump::randomZPosition(int totalRows, int row, float sectionBegin, float SectionEnd)
    {
        float fieldHeight = SectionEnd - sectionBegin;
        float halfHeight = fieldHeight/totalRows;
        float lowerBound = row*halfHeight*2+sectionBegin;
        float upperBound = lowerBound + 2*halfHeight;

        return randomPosition(lowerBound+platformHeight_/2, upperBound-platformHeight_/2);
    }

    float Jump::randomZPositionLower(int totalRows, int row, float sectionBegin, float SectionEnd)
    {
        float fieldHeight = SectionEnd - sectionBegin;
        float rowHeight = fieldHeight/totalRows;
        float lowerBound = row*rowHeight+sectionBegin;
        float upperBound = lowerBound + rowHeight/3;

        return randomPosition(lowerBound+platformHeight_/2, upperBound-platformHeight_/2);
    }

    float Jump::randomZPositionUpper(int totalRows, int row, float sectionBegin, float SectionEnd)
    {
        float fieldHeight = SectionEnd - sectionBegin;
        float rowHeight = fieldHeight/totalRows;
        float lowerBound = (row+1)*rowHeight+sectionBegin;
        float upperBound = lowerBound - rowHeight/3;

        return randomPosition(lowerBound+platformHeight_/2, upperBound-platformHeight_/2);
    }

    float Jump::randomPosition(float lowerBoundary, float upperBoundary)
    {
        if (lowerBoundary >= upperBoundary)
        {
            return (lowerBoundary + upperBoundary)/2;
        }

        return (float)(rand()%(int)(100*(upperBoundary - lowerBoundary)))/100 + lowerBoundary;
    }

    float Jump::randomSpeed()
    {
        float platformSpeed = center_->getPlatformSpeed();
        return randomPosition(0.5f*platformSpeed, 1.5f*platformSpeed);
    }

    int Jump::getScore(PlayerInfo* player) const
    {
        return sectionNumber_ - 2;
    }

    float Jump::getFuel() const
    {
        if (this->figure_ != NULL)
        {
            if (this->figure_->rocketActive_ != NULL)
            {
                return this->figure_->rocketActive_->getFuelState();
            }
            else if (this->figure_->propellerActive_ != NULL)
            {
                return this->figure_->propellerActive_->getFuelState();
            }
            else if (this->figure_->shieldActive_ != NULL)
            {
                return this->figure_->shieldActive_->getFuelState();
            }
            else if (this->figure_->bootsActive_ != NULL)
            {
                return this->figure_->bootsActive_->getFuelState();
            }
        }

        return 0.0;
    }

    bool Jump::getDead(PlayerInfo* player) const
    {
        return figure_->dead_;
    }
}
