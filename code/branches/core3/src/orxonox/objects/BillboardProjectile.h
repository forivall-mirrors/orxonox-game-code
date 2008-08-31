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

#ifndef _BillboardProjectile_H__
#define _BillboardProjectile_H__

#include "OrxonoxPrereqs.h"

#include "Projectile.h"
#include "tools/BillboardSet.h"
#include "util/Math.h"

namespace orxonox
{
    class TESTTESTTEST1
    {
        public:
            TESTTESTTEST1() { this->setMyValue(10); }

        private:
            void setMyValue(int value) { this->value1_ = value; }

            int value1_;
            int value2_;
            Identifier* identifier_;
    };

    class TESTTESTTEST2
    {
        public:
            TESTTESTTEST2() { this->setMyValue(10); }

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

    class TESTTESTTEST3 : virtual public TESTTESTTEST1
    {
        public:
            TESTTESTTEST3() { this->setMyOtherValue(10); }

        private:
            void setMyOtherValue(int value) { this->value3_ = value; }

            int value3_;
            TESTTESTTEST2* test_;
    };

    class _OrxonoxExport BillboardProjectile : public Projectile, public TESTTESTTEST3, public TESTTESTTEST2, virtual public TESTTESTTEST1
    {
        public:
            BillboardProjectile(SpaceShip* owner = 0);
            virtual ~BillboardProjectile();

            virtual void setColour(const ColourValue& colour);
            virtual void changedVisibility();
            virtual void testfunction();

        private:
            BillboardSet billboard_;
    };
}

#endif /* _BillboardProjectile_H__ */
