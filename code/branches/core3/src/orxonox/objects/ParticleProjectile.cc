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
#include "ParticleProjectile.h"

#include "SpaceShip.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
//#include "util/FastDelegate.h"
//using namespace fastdelegate;

namespace orxonox
{
    CreateFactory(ParticleProjectile);

    struct FunctionPointerViewer
    {
        void* ptr1_;
        void* ptr2_;

        void view()
        {
            std::cout << ptr1_ << "." << ptr2_ << std::endl;
        }
    };

    union FunctionPointerViewer1
    {
        FunctionPointerViewer viewer_;
        void (Projectile::*function_) ();
    };

    union FunctionPointerViewer2
    {
        FunctionPointerViewer viewer_;
        void (BillboardProjectile::*function_) ();
    };

    union FunctionPointerViewer3
    {
        FunctionPointerViewer viewer_;
        void (ParticleProjectile::*function_) ();
    };

    ParticleProjectile::ParticleProjectile(SpaceShip* owner) : BillboardProjectile(owner)
    {
        RegisterObject(ParticleProjectile);

        if (this->owner_)
        {
            this->particles_ = new ParticleInterface("Orxonox/shot2", LODParticle::normal);
            this->particles_->addToSceneNode(this->getNode());
            this->particles_->getAllEmitters()->setDirection(-this->owner_->getInitialDir());
            this->particles_->setKeepParticlesInLocalSpace(true);
        }
        else
        {
            this->particles_ = 0;
        }

        this->setConfigValues();
/*
        FunctionPointerViewer1 fpw1;
        fpw1.function_ = &Projectile::testfunction;
        FunctionPointerViewer2 fpw2;
        fpw2.function_ = &BillboardProjectile::testfunction;
        FunctionPointerViewer3 fpw3;
        fpw3.function_ = &ParticleProjectile::testfunction;

        std::cout << sizeof(void (Projectile::*) ()) << std::endl;
        fpw1.viewer_.view();
        fpw2.viewer_.view();
        fpw3.viewer_.view();

        {
            std::cout << "1:" << std::endl;
            FastDelegate0<> delegate1(this, &ParticleProjectile::testfunction);
            delegate1();
            FastDelegate0<> delegate2((BillboardProjectile*)this, &BillboardProjectile::testfunction);
            delegate2();
            FastDelegate0<> delegate3(this, &Projectile::testfunction);
            delegate3();
        }
        {
            std::cout << "2:" << std::endl;
            BillboardProjectile temp;
//            FastDelegate0<> delegate1(&temp, &ParticleProjectile::testfunction);
//            delegate1();
            FastDelegate0<> delegate2(&temp, &BillboardProjectile::testfunction);
            delegate2();
            FastDelegate0<> delegate3(&temp, &Projectile::testfunction);
            delegate3();
        }
        std::cout << "done" << std::endl;

        std::cout << "0:" << std::endl;
        this->Projectile::testfunction();
        this->BillboardProjectile::testfunction();
        this->ParticleProjectile::testfunction();
        this->testfunction();

        std::cout << "1:" << std::endl;
        (this->*fpw1.function_)();
        std::cout << "2:" << std::endl;
        (this->*fpw2.function_)();
        std::cout << "3:" << std::endl;
        (this->*fpw3.function_)();
        std::cout << "done" << std::endl;
*/
        std::cout << "c:\n";
        SUPER(ParticleProjectile, testfunction);
        std::cout << "d:\n";

        std::cout << "e:\n";
        this->testfunction();
        std::cout << "f:\n";

//        (*((ClassIdentifier<SuperDummy>*)this->getIdentifier())->superFunctionCaller_testfunction_)(this);
    }

    ParticleProjectile::~ParticleProjectile()
    {
        if (this->isInitialized() && this->particles_)
            delete this->particles_;
    }

    void ParticleProjectile::setConfigValues()
    {
        SetConfigValue(speed_, 5000.0).description("The speed of a projectile in units per second").callback((Projectile*)this, &ParticleProjectile::speedChanged);
    }

    void ParticleProjectile::changedVisibility()
    {
        BillboardProjectile::changedVisibility();
        this->particles_->setEnabled(this->isVisible());
    }

    void ParticleProjectile::testfunction() { SUPER(ParticleProjectile, testfunction); std::cout << "3 -> " << std::endl; }
}
