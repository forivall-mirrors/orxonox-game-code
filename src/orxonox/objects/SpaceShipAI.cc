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

#include "OrxonoxStableHeaders.h"
#include "SpaceShipAI.h"

#include <OgreMath.h>
#include "Projectile.h"
#include "ParticleSpawner.h"
#include "core/CoreIncludes.h"
#include "core/Iterator.h"
#include "core/Executor.h"
#include "core/ConsoleCommand.h"
#include "core/XMLPort.h"
#include "tools/ParticleInterface.h"

#define ACTION_INTERVAL 1.0f

namespace orxonox
{
    SetConsoleCommand(SpaceShipAI, createEnemy, true).setDefaultValue(0, 1);
    SetConsoleCommand(SpaceShipAI, killEnemies, true).setDefaultValue(0, 0);

    CreateFactory(SpaceShipAI);

    SpaceShipAI::SpaceShipAI()
    {
        RegisterObject(SpaceShipAI);

        this->target_ = 0;
        this->bShooting_ = 0;
        this->bHasTargetPosition_ = false;

        this->setTeamNr((int)rnd(NUM_AI_TEAMS) % NUM_AI_TEAMS + 1);

        if (NUM_AI_TEAMS > 0)
            this->teamColours_[1] = ColourValue(1, 0, 0, 1);
        if (NUM_AI_TEAMS > 1)
            this->teamColours_[2] = ColourValue(0, 1, 0, 1);
        if (NUM_AI_TEAMS > 2)
            this->teamColours_[3] = ColourValue(0, 0, 1, 1);

        for (int i = 4; i <= NUM_AI_TEAMS; ++i)
            this->teamColours_[i] = ColourValue(rnd(), rnd(), rnd(), 1);
    }

    SpaceShipAI::~SpaceShipAI()
    {
        for (Iterator<SpaceShipAI> it = ObjectList<SpaceShipAI>::begin(); it; ++it)
            it->shipDied(this);
    }

    void SpaceShipAI::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SpaceShip::XMLPort(xmlelement, mode);

        this->myShip_=true;
        this->actionTimer_.setTimer(ACTION_INTERVAL, true, this, createExecutor(createFunctor(&SpaceShipAI::action)));
    }

    void SpaceShipAI::createEnemy(int num)
    {
        for (int i = 0; i < num; ++i)
        {
            SpaceShipAI* newenemy = new SpaceShipAI();
            newenemy->setMesh("assff.mesh");
//            newenemy->setPosition(0, 0, 0);
            newenemy->setPosition(Vector3(rnd(-3000, 3000), rnd(-3000, 3000), rnd(-3000, 3000)));
            newenemy->setScale(10);
            newenemy->setMaxSpeed(500);
            newenemy->setMaxSideAndBackSpeed(50);
            newenemy->setMaxRotation(1.0);
            newenemy->setTransAcc(200);
            newenemy->setRotAcc(3.0);
            newenemy->setTransDamp(75);
            newenemy->setRotDamp(1.0);
            Element xmlelement;
            newenemy->XMLPort(xmlelement, XMLPort::LoadObject);

            ParticleSpawner* spawneffect = new ParticleSpawner("Orxonox/fairytwirl", 2.0, 0.0, newenemy->getOrth());
            spawneffect->setPosition(newenemy->getPosition() - newenemy->getOrth() * 50);
            spawneffect->create();
        }
    }

    void SpaceShipAI::killEnemies(int num)
    {
        int i = 0;
        for (Iterator<SpaceShipAI> it = ObjectList<SpaceShipAI>::begin(); it; )
        {
            (it++)->kill();
            if (num && i >= num)
                break;
        }
    }

    ColourValue SpaceShipAI::getProjectileColour() const
    {
        return this->teamColours_[this->getTeamNr()];
    }

    void SpaceShipAI::action()
    {
        float random;
        float maxrand = 100.0f / ACTION_INTERVAL;

        // search enemy
        random = rnd(maxrand);
        if (random < 20 && (!this->target_))
            this->searchNewTarget();

        // forget enemy
        random = rnd(maxrand);
        if (random < 5 && (this->target_))
            this->forgetTarget();

        // next enemy
        random = rnd(maxrand);
        if (random < 10 && (this->target_))
            this->searchNewTarget();

        // fly somewhere
        random = rnd(maxrand);
        if (random < 40 && (!this->bHasTargetPosition_ && !this->target_))
            this->searchNewTargetPosition();

        // stop flying
        random = rnd(maxrand);
        if (random < 10 && (this->bHasTargetPosition_ && !this->target_))
            this->bHasTargetPosition_ = false;

        // fly somewhere else
        random = rnd(maxrand);
        if (random < 30 && (this->bHasTargetPosition_ && !this->target_))
            this->searchNewTargetPosition();

        // shoot
        random = rnd(maxrand);
        if (random < 75 && (this->target_ && !this->bShooting_))
            this->bShooting_ = true;

        // stop shooting
        random = rnd(maxrand);
        if (random < 25 && (this->bShooting_))
            this->bShooting_ = false;
    }

    void SpaceShipAI::damage(float damage)
    {
        this->health_ -= damage;
        if (this->health_ <= 0)
        {
            this->kill();
            SpaceShipAI::createEnemy(1);
        }
    }

    void SpaceShipAI::kill()
    {
        ParticleSpawner* explosion = new ParticleSpawner("Orxonox/BigExplosion1part1", 3.0);
        explosion->setPosition(this->getPosition());
        explosion->getParticleInterface()->setKeepParticlesInLocalSpace(true);
        explosion->setScale(4);
        explosion->create();

        explosion = new ParticleSpawner("Orxonox/BigExplosion1part2", 3.0);
        explosion->setPosition(this->getPosition());
        explosion->getParticleInterface()->setKeepParticlesInLocalSpace(true);
        explosion->setScale(4);
        explosion->create();

        Vector3 ringdirection = Vector3(rnd(), rnd(), rnd());
        ringdirection.normalise();
        explosion = new ParticleSpawner("Orxonox/BigExplosion1part3", 3.0, 0.5, ringdirection);
        explosion->setPosition(this->getPosition());
        explosion->getParticleInterface()->setKeepParticlesInLocalSpace(true);
        explosion->setScale(4);
        explosion->create();

        delete this;
    }

    void SpaceShipAI::tick(float dt)
    {
        if (this->target_)
            this->aimAtTarget();

        if (this->bHasTargetPosition_)
            this->moveToTargetPosition(dt);

        if (this->bShooting_ && this->isCloseAtTarget(2500) && this->isLookingAtTarget(Ogre::Math::PI / 10))
            this->doFire();

        SpaceShip::tick(dt);
    }

    void SpaceShipAI::moveToTargetPosition(float dt)
    {
        Vector3 proj = Ogre::Plane(this->getDir(), this->getPosition()).projectVector(this->targetPosition_ - this->getPosition());
        float angle = acos((this->getOrth().dotProduct(proj)) / (this->getOrth().length()*proj.length()));

        if ((this->getDir().crossProduct(this->getOrth())).dotProduct(this->targetPosition_ - this->getPosition()) > 0)
            this->setMoveYaw(sgn(sin(angle)));
        else
            this->setMoveYaw(-sgn(sin(angle)));
        this->setMovePitch(sgn(cos(angle)));

        if ((this->targetPosition_ - this->getPosition()).length() > 300)
            this->setMoveLongitudinal(1);

        if (this->isCloseAtTarget(300) && this->target_)
        {
            if (this->getVelocity().length() > this->target_->getVelocity().length())
                this->setMoveLongitudinal(-1);
        }
    }

    void SpaceShipAI::searchNewTargetPosition()
    {
        this->targetPosition_ = Vector3(rnd(-5000,5000), rnd(-5000,5000), rnd(-5000,5000));
        this->bHasTargetPosition_ = true;
    }

    void SpaceShipAI::searchNewTarget()
    {
        this->targetPosition_ = this->getPosition();
        this->forgetTarget();

        for (Iterator<SpaceShip> it = ObjectList<SpaceShip>::begin(); it; ++it)
        {
            if (it->getTeamNr() != this->getTeamNr())
            {
                float speed = this->getVelocity().length();
                Vector3 distanceCurrent = this->targetPosition_ - this->getPosition();
                Vector3 distanceNew = it->getPosition() - this->getPosition();
                if (!this->target_ || it->getPosition().squaredDistance(this->getPosition()) * (1.5f + acos((this->getOrientation() * Ogre::Vector3::UNIT_X).dotProduct(distanceNew) / speed / distanceNew.length()) / (2 * Ogre::Math::PI))
                        < this->targetPosition_.squaredDistance(this->getPosition()) * (1.5f + acos((this->getOrientation() * Ogre::Vector3::UNIT_X).dotProduct(distanceCurrent) / speed / distanceCurrent.length()) / (2 * Ogre::Math::PI)) + rnd(-250, 250))
                {
                    this->target_ = (*it);
                    this->targetPosition_ = it->getPosition();
                }
            }
        }
    }

    void SpaceShipAI::forgetTarget()
    {
        this->target_ = 0;
        this->bShooting_ = false;
    }

    void SpaceShipAI::aimAtTarget()
    {
        if (!this->target_)
            return;

        Vector3 enemymovement = this->target_->getVelocity();
        Vector3 distance_normalised = this->target_->getPosition() - this->getPosition();
        distance_normalised.normalise();

        float scalarprod = enemymovement.dotProduct(distance_normalised);
        float aimoffset = scalarprod*scalarprod + Projectile::getSpeed() * Projectile::getSpeed() - this->target_->getVelocity().squaredLength();

        if (aimoffset < 0)
        {
            this->bHasTargetPosition_ = false;
            return;
        }
        aimoffset = -scalarprod + sqrt(aimoffset);
        this->targetPosition_ = this->getPosition() + enemymovement + distance_normalised * aimoffset;
        this->bHasTargetPosition_ = true;
    }

    bool SpaceShipAI::isCloseAtTarget(float distance)
    {
        if (!this->target_)
            return (this->getPosition().squaredDistance(this->targetPosition_) < distance*distance);
        else
            return (this->getPosition().squaredDistance(this->target_->getPosition()) < distance*distance);
    }

    bool SpaceShipAI::isLookingAtTarget(float angle)
    {
        Vector3 dist = this->targetPosition_ - this->getPosition();
        return (acos((this->getDir().dotProduct(dist)) / (dist.length() * this->getDir().length())) < angle);
    }

    void SpaceShipAI::shipDied(SpaceShipAI* ship)
    {
        this->forgetTarget();
        this->searchNewTargetPosition();
    }
}
