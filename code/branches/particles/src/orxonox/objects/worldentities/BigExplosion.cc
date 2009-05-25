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
 *      Benjamin de Capitani
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "BigExplosion.h"
#include "MovableEntity.h"
#include "Model.h"

#include <OgreParticleSystem.h>
#include <OgreSceneNode.h>
#include <sstream>

#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "core/CommandExecutor.h"
#include "objects/Scene.h"
#include "tools/ParticleInterface.h"
#include "objects/worldentities/ParticleSpawner.h"
#include "util/Exception.h"

namespace orxonox
{
    CreateFactory(BigExplosion);

    BigExplosion::BigExplosion(BaseObject* creator) : MovableEntity(creator)
    {
        RegisterObject(BigExplosion);

        if ( Core::showsGraphics() && ( !this->getScene() || !this->getScene()->getSceneManager() ) )
            ThrowException(AbortLoading, "Can't create BigExplosion, no scene or no scene manager given.");
/*
        this->cps_ = 1;
        this->firstTick_ = true;
*/
        this->bStop_ = false;
        this->LOD_ = LODParticle::normal;

/*      this->stf_ = "setTimeFactor ";
        this->timeFactor_ =  1;
        std::ostringstream o;
        o << stf_ << this->timeFactor_;
        CommandExecutor::execute(o.str() ,false);
        this->timeFactor_ = 0.1;
*/

        if ( Core::showsGraphics() )
        {
            try
            {
                this->init();
            }
            catch (...)
            {
                COUT(1) << "Error: Couln't load particle effect in BigExplosion." << std::endl;
                this->initZero();
            }
        }
        else
        {
            this->initZero();
        }

        if (Core::isMaster())
        {
            Vector3 velocity(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1));
            velocity.normalise();
            velocity *= rnd(20, 30);
            this->setVelocity(velocity);

            this->destroyTimer_.setTimer(rnd(4, 7), false, this, createExecutor(createFunctor(&BigExplosion::stop)));
        }
        this->registerVariables();
    }

    void BigExplosion::init()
    {
        Identifier* idDE1 = Class(MovableEntity);
        BaseObject* oDE1 = idDE1->fabricate(this);
        this->debrisEntity1_ = dynamic_cast<MovableEntity*>(oDE1);

        Identifier* idDE2 = Class(MovableEntity);
        BaseObject* oDE2 = idDE2->fabricate(this);
        this->debrisEntity2_ = dynamic_cast<MovableEntity*>(oDE2);

        Identifier* idDE3 = Class(MovableEntity);
        BaseObject* oDE3 = idDE3 ->fabricate(this);
        this->debrisEntity3_ = dynamic_cast<MovableEntity*>(oDE3);

        Identifier* idDE4 = Class(MovableEntity);
        BaseObject* oDE4 = idDE4->fabricate(this);
        this->debrisEntity4_ = dynamic_cast<MovableEntity*>(oDE4);

        Identifier* idD1 = Class(Model);
        BaseObject* oD1 = idD1->fabricate(this);
        this->debris1_ = dynamic_cast<Model*>(oD1);

        Identifier* idD2 = Class(Model);
        BaseObject* oD2 = idD2->fabricate(this);
        this->debris2_ = dynamic_cast<Model*>(oD2);

        Identifier* idD3 = Class(Model);
        BaseObject* oD3 = idD3->fabricate(this);
        this->debris3_ = dynamic_cast<Model*>(oD3);

        Identifier* idD4 = Class(Model);
        BaseObject* oD4 = idD4->fabricate(this);
        this->debris4_ = dynamic_cast<Model*>(oD4);

        Identifier* id6 = Class(StaticEntity);
        BaseObject* object4 = id6->fabricate(this);
        this->explosion_ = dynamic_cast<StaticEntity*>(object4);

        this->debrisSmoke1_ = new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/smoke7", this->LOD_);
        this->debrisSmoke2_ =  new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/smoke7", this->LOD_);
        this->debrisSmoke3_ =  new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/smoke7", this->LOD_);
        this->debrisSmoke4_ =  new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/smoke7", this->LOD_);

        this->debrisFire1_ = new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/fire4", this->LOD_);
        this->debrisFire2_ = new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/fire4", this->LOD_);
        this->debrisFire3_ = new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/fire4", this->LOD_);
        this->debrisFire4_ = new ParticleInterface(this->getScene()->getSceneManager(), "Orxonox/fire4", this->LOD_);

        this->debris1_->attachOgreObject(this->debrisSmoke1_->getParticleSystem());
        this->debris1_->attachOgreObject(this->debrisFire1_->getParticleSystem());
        this->debris2_->attachOgreObject(this->debrisSmoke2_->getParticleSystem());
        this->debris2_->attachOgreObject(this->debrisFire2_->getParticleSystem());
        this->debris3_->attachOgreObject(this->debrisSmoke3_->getParticleSystem());
        this->debris3_->attachOgreObject(this->debrisFire3_->getParticleSystem());
        this->debris4_->attachOgreObject(this->debrisSmoke4_->getParticleSystem());
        this->debris4_->attachOgreObject(this->debrisFire4_->getParticleSystem());

        this->debris1_->setMeshSource("CockpitDebris.mesh");
        this->debris2_->setMeshSource("WingDebris1.mesh");
        this->debris3_->setMeshSource("BodyDebris1.mesh");
        this->debris4_->setMeshSource("WingDebris2.mesh");

        this->debrisEntity1_->setVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1))*rnd(50,100));
        this->debrisEntity1_->setAngularVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1)).normalisedCopy() * Degree(400).valueRadians());
        this->debrisEntity1_->setScale(4);

        this->debrisEntity2_->setVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1))*rnd(50,100));
        this->debrisEntity2_->setAngularVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1)).normalisedCopy() * Degree(400).valueRadians());
        this->debrisEntity2_->setScale(4);

        this->debrisEntity3_->setVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1))*rnd(50,100));
        this->debrisEntity3_->setAngularVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1)).normalisedCopy() * Degree(400).valueRadians());
        this->debrisEntity3_->setScale(4);

        this->debrisEntity4_->setVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1))*rnd(50,100));
        this->debrisEntity4_->setAngularVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1)).normalisedCopy() * Degree(400).valueRadians());
        this->debrisEntity4_->setScale(4);

        this->debrisEntity1_->attach(debris1_);
        this->debrisEntity2_->attach(debris2_);
        this->debrisEntity3_->attach(debris3_);
        this->debrisEntity4_->attach(debris4_);

        ParticleSpawner* effect = new ParticleSpawner(this->getCreator());
        effect->setDestroyAfterLife(true);
        effect->setSource("Orxonox/explosion2b");
        effect->setLifetime(4.0f);

        ParticleSpawner* effect2 = new ParticleSpawner(this->getCreator());
        effect2->setDestroyAfterLife(true);
        effect2->setSource("Orxonox/smoke6");
        effect2->setLifetime(4.0f);

        this->explosion_->attach(effect);
        this->explosion_->attach(effect2);

        this->attach(explosion_);
        this->attach(debrisEntity1_);
        this->attach(debrisEntity2_);
        this->attach(debrisEntity3_);
        this->attach(debrisEntity4_);


        for(int i=0;i<10;i++)
        {
            Identifier* idf1 = Class(Model);
            BaseObject* obj1 = idf1->fabricate(this);
            Model* part1 = dynamic_cast<Model*>(obj1);


            Identifier* idf2 = Class(Model);
            BaseObject* obj2 = idf2->fabricate(this);
            Model* part2 = dynamic_cast<Model*>(obj2);

            Identifier* idf3 = Class(MovableEntity);
            BaseObject* obj3 = idf3->fabricate(this);
            MovableEntity* partEntity1 = dynamic_cast<MovableEntity*>(obj3);

            Identifier* idf4 = Class(MovableEntity);
            BaseObject* obj4 = idf4->fabricate(this);
            MovableEntity* partEntity2 = dynamic_cast<MovableEntity*>(obj4);

            partEntity1->setVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1))*rnd(10,100));
            partEntity1->setAngularVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1)).normalisedCopy() * Degree(400).valueRadians());
            partEntity1->setScale(rnd(1, 3));

            partEntity2->setVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1))*rnd(10, 100));
            partEntity2->setAngularVelocity(Vector3(rnd(-1, 1), rnd(-1, 1), rnd(-1, 1)).normalisedCopy() * Degree(400).valueRadians());
            partEntity2->setScale(rnd(1, 3));

            part1->setMeshSource("SmallPart1.mesh");
            part2->setMeshSource("SmallPart2.mesh");


            partEntity1->attach(part1);
            partEntity2->attach(part2);

            this->attach(partEntity1);
            this->attach(partEntity2);
        }
    }

    void BigExplosion::initZero()
    {
        this->debrisFire1_ = 0;
        this->debrisFire2_ = 0;
        this->debrisFire3_ = 0;
        this->debrisFire4_ = 0;

        this->debrisSmoke1_ = 0;
        this->debrisSmoke2_ = 0;
        this->debrisSmoke3_ = 0;
        this->debrisSmoke4_ = 0;

        this->explosionSmoke_=0;
        this->explosionFire_=0;
    }

    BigExplosion::~BigExplosion()
    {
        CommandExecutor::execute("setTimeFactor 1", false);

        if (this->isInitialized())
        {
            if (this->debrisFire1_)
            {
                this->debris1_->detachOgreObject(this->debrisFire1_->getParticleSystem());
                delete this->debrisFire1_;
            }
            if (this->debrisSmoke1_)
            {
                this->debris1_->detachOgreObject(this->debrisSmoke1_->getParticleSystem());
                delete this->debrisSmoke1_;
            }

            if (this->debrisFire2_)
            {
                this->debris2_->detachOgreObject(this->debrisFire2_->getParticleSystem());
                delete this->debrisFire2_;
            }
            if (this->debrisSmoke2_)
            {
                this->debris2_->detachOgreObject(this->debrisSmoke2_->getParticleSystem());
                delete this->debrisSmoke2_;
            }

            if (this->debrisFire3_)
            {
                this->debris3_->detachOgreObject(this->debrisFire3_->getParticleSystem());
                delete this->debrisFire3_;
            }
            if (this->debrisSmoke3_)
            {
                this->debris3_->detachOgreObject(this->debrisSmoke3_->getParticleSystem());
                delete this->debrisSmoke3_;
            }

            if (this->debrisFire4_)
            {
                this->debris4_->detachOgreObject(this->debrisFire4_->getParticleSystem());
                delete this->debrisFire4_;
            }
            if (this->debrisSmoke4_)
            {
                this->debris4_->detachOgreObject(this->debrisSmoke4_->getParticleSystem());
                delete this->debrisSmoke4_;
            }
        }
    }

    void BigExplosion::registerVariables()
    {
        registerVariable((int&)(this->LOD_), variableDirection::toclient, new NetworkCallback<BigExplosion>(this, &BigExplosion::LODchanged));
        registerVariable(this->bStop_,       variableDirection::toclient, new NetworkCallback<BigExplosion>(this, &BigExplosion::checkStop));
    }

    void BigExplosion::LODchanged()
    {
        if (this->debrisFire1_)
            this->debrisFire1_->setDetailLevel(this->LOD_);
        if (this->debrisSmoke1_)
            this->debrisSmoke1_->setDetailLevel(this->LOD_);

        if (this->debrisFire2_)
            this->debrisFire2_->setDetailLevel(this->LOD_);
        if (this->debrisSmoke2_)
            this->debrisSmoke2_->setDetailLevel(this->LOD_);

        if (this->debrisFire3_)
            this->debrisFire3_->setDetailLevel(this->LOD_);
        if (this->debrisSmoke3_)
            this->debrisSmoke3_->setDetailLevel(this->LOD_);

        if (this->debrisFire4_)
            this->debrisFire4_->setDetailLevel(this->LOD_);
        if (this->debrisSmoke4_)
            this->debrisSmoke4_->setDetailLevel(this->LOD_);
    }

    void BigExplosion::checkStop()
    {
        if (this->bStop_)
            this->stop();
    }

    void BigExplosion::stop()
    {
        if (this->debrisFire1_)
            this->debrisFire1_->setEnabled(false);
        if (this->debrisSmoke1_)
            this->debrisSmoke1_->setEnabled(false);

        if (this->debrisFire2_)
            this->debrisFire2_->setEnabled(false);
        if (this->debrisSmoke2_)
            this->debrisSmoke2_->setEnabled(false);

        if (this->debrisFire3_)
            this->debrisFire3_->setEnabled(false);
        if (this->debrisSmoke3_)
            this->debrisSmoke3_->setEnabled(false);

        if (this->debrisFire4_)
            this->debrisFire4_->setEnabled(false);
        if (this->debrisSmoke4_)
            this->debrisSmoke4_->setEnabled(false);

        if (Core::isMaster())
        {
            this->bStop_ = true;
            this->destroyTimer_.setTimer(1.0f, false, this, createExecutor(createFunctor(&BigExplosion::destroy)));
        }
    }

    void BigExplosion::destroy()
    {
        delete this;
    }

/* TODO

    void BigExplosion::setDebrisMeshes()
    {

    }
    void BigExplosion::getDebrisMeshes()
    {

    }
*/

     void BigExplosion::tick(float dt)
    {
//        static const unsigned int CHANGES_PER_SECOND = 10;


/*        if (Core::isMaster() && rnd() < dt*(this->cps_))
        {

            if(this->timeFactor_ < 1 )
                this->timeFactor_ += 0.05;

            if(this->firstTick_)
                this->cps_ = 256;

            std::ostringstream o;
            o << this->stf_ << this->timeFactor_;
            CommandExecutor::execute(o.str() ,false);
            if(this->cps_>50)
                this->cps_/=2;
            this->firstTick_ = false;
            COUT(0) << timeFactor_ << std::endl;
        }
*/

        SUPER(BigExplosion, tick, dt);
    }
}
