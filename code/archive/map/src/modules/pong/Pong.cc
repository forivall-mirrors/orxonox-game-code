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
 *   Co-authors:
 *      ...
 *
 */

#include "Pong.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/Executor.h"
#include "PongCenterpoint.h"
#include "PongBall.h"
#include "PongBat.h"
#include "PongBot.h"
#include "PongAI.h"

namespace orxonox
{
    CreateEventName(PongCenterpoint, right);
    CreateEventName(PongCenterpoint, left);

    CreateUnloadableFactory(Pong);

    Pong::Pong(BaseObject* creator) : Deathmatch(creator)
    {
        RegisterObject(Pong);

        this->center_ = 0;
        this->ball_ = 0;
        this->bat_[0] = 0;
        this->bat_[1] = 0;

        this->setHUDTemplate("PongHUD");

        this->starttimer_.setTimer(1.0, false, createExecutor(createFunctor(&Pong::startBall, this)));
        this->starttimer_.stopTimer();

        this->botclass_ = Class(PongBot);
    }

    void Pong::start()
    {
        if (this->center_)
        {
            if (!this->ball_)
            {
                this->ball_ = new PongBall(this->center_);
                this->ball_->addTemplate(this->center_->getBalltemplate());
            }

            this->center_->attach(this->ball_);
            this->ball_->setPosition(0, 0, 0);
            this->ball_->setFieldDimension(this->center_->getFieldDimension());
            this->ball_->setSpeed(0);
            this->ball_->setAccelerationFactor(this->center_->getBallAccelerationFactor());
            this->ball_->setBatLength(this->center_->getBatLength());

            if (!this->bat_[0])
            {
                this->bat_[0] = new PongBat(this->center_);
                this->bat_[0]->addTemplate(this->center_->getBattemplate());
            }
            if (!this->bat_[1])
            {
                this->bat_[1] = new PongBat(this->center_);
                this->bat_[1]->addTemplate(this->center_->getBattemplate());
            }

            this->center_->attach(this->bat_[0]);
            this->center_->attach(this->bat_[1]);
            this->bat_[0]->setPosition(-this->center_->getFieldDimension().x / 2, 0, 0);
            this->bat_[1]->setPosition( this->center_->getFieldDimension().x / 2, 0, 0);
            this->bat_[0]->yaw(Degree(-90));
            this->bat_[1]->yaw(Degree(90));
            this->bat_[0]->setSpeed(this->center_->getBatSpeed());
            this->bat_[1]->setSpeed(this->center_->getBatSpeed());
            this->bat_[0]->setFieldHeight(this->center_->getFieldDimension().y);
            this->bat_[1]->setFieldHeight(this->center_->getFieldDimension().y);
            this->bat_[0]->setLength(this->center_->getBatLength());
            this->bat_[1]->setLength(this->center_->getBatLength());

            this->ball_->setBats(this->bat_);
        }
        else
        {
            COUT(1) << "Error: No Centerpoint specified." << std::endl;
        }

        this->starttimer_.startTimer();


        bool temp = this->bForceSpawn_;
        this->bForceSpawn_ = true;

        Deathmatch::start();

        this->bForceSpawn_ = temp;
    }

    void Pong::end()
    {
        if (this->ball_)
        {
            this->ball_->destroy();
            this->ball_ = 0;
        }

        Deathmatch::end();
    }

    void Pong::spawnPlayer(PlayerInfo* player)
    {
        if (!this->bat_[0]->getPlayer())
        {
            player->startControl(this->bat_[0]);
            this->players_[player].state_ = PlayerState::Alive;
        }
        else if (!this->bat_[1]->getPlayer())
        {
            player->startControl(this->bat_[1]);
            this->players_[player].state_ = PlayerState::Alive;
        }
        else
            return;

        if (player && player->getController() && player->getController()->isA(Class(PongAI)))
        {
            PongAI* ai = orxonox_cast<PongAI*>(player->getController());
            ai->setPongBall(this->ball_);
        }
    }

    void Pong::playerScored(PlayerInfo* player)
    {
        Deathmatch::playerScored(player);

        if (this->center_)
        {
            if (player == this->getRightPlayer())
                this->center_->fireEvent(FireEventName(PongCenterpoint, right));
            else if (player == this->getLeftPlayer())
                this->center_->fireEvent(FireEventName(PongCenterpoint, left));

            if (player)
                this->gtinfo_->sendAnnounceMessage(player->getName() + " scored");
        }

        if (this->ball_)
        {
            this->ball_->setPosition(Vector3::ZERO);
            this->ball_->setVelocity(Vector3::ZERO);
            this->ball_->setAcceleration(Vector3::ZERO);
            this->ball_->setSpeed(0);
        }

        if (this->bat_[0] && this->bat_[1])
        {
            this->bat_[0]->setPosition(-this->center_->getFieldDimension().x / 2, 0, 0);
            this->bat_[1]->setPosition( this->center_->getFieldDimension().x / 2, 0, 0);
        }

        this->starttimer_.startTimer();
    }

    void Pong::startBall()
    {
        if (this->ball_ && this->center_)
            this->ball_->setSpeed(this->center_->getBallSpeed());
    }

    PlayerInfo* Pong::getLeftPlayer() const
    {
        if (this->bat_ && this->bat_[0])
            return this->bat_[0]->getPlayer();
        else
            return 0;
    }

    PlayerInfo* Pong::getRightPlayer() const
    {
        if (this->bat_ && this->bat_[1])
            return this->bat_[1]->getPlayer();
        else
            return 0;
    }
}
