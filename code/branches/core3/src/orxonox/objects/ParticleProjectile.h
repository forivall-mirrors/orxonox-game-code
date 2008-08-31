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

#ifndef _ParticleProjectile_H__
#define _ParticleProjectile_H__

#include "OrxonoxPrereqs.h"

#include "BillboardProjectile.h"
#include "tools/ParticleInterface.h"
#include "util/Math.h"

namespace orxonox
{
    class TESTTESTTEST13
    {
        public:
            TESTTESTTEST13() { this->setMyValue(10); }

        private:
            void setMyValue(int value) { this->value1_ = value; }

            int value1_;
            int value2_;
            Identifier* identifier_;
    };

    class TESTTESTTEST23
    {
        public:
            TESTTESTTEST23() { this->setMyValue(10); }

        private:
            void setMyValue(int value) { this->value1_ = value; }

            int value1_;
            int value2_;
            double value3_;
            char value4_;
            bool value5_;
            Identifier* identifier1_;
            Identifier* identifier2_;
    };

    class TESTTESTTEST33 : virtual public TESTTESTTEST13
    {
        public:
            TESTTESTTEST33() { this->setMyOtherValue(10); }

        private:
            void setMyOtherValue(int value) { this->value3_ = value; }

            int value3_;
            TESTTESTTEST23* test_;
    };

    class _OrxonoxExport ParticleProjectile : public TESTTESTTEST33, public BillboardProjectile, virtual public TESTTESTTEST13
    {
        public:
            ParticleProjectile(SpaceShip* owner = 0);
            virtual ~ParticleProjectile();
            virtual void changedVisibility();
            virtual void testfunction();
            void setConfigValues();

        private:
            ParticleInterface* particles_;
    };
}

#endif /* _ParticleProjectile_H__ */
