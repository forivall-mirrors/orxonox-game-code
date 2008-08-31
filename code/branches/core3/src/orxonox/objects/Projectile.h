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

#ifndef _Projectile_H__
#define _Projectile_H__

#include "OrxonoxPrereqs.h"

#include "core/Super.h"
#include "WorldEntity.h"
#include "tools/Timer.h"

namespace orxonox
{
    class TESTTESTTEST12
    {
        public:
            TESTTESTTEST12() { this->setMyValue(10); }

        private:
            void setMyValue(int value) { this->value1_ = value; }

            int value1_;
            int value2_;
            Identifier* identifier_;
    };

    class TESTTESTTEST22
    {
        public:
            TESTTESTTEST22() { this->setMyValue(10); }

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

    class TESTTESTTEST32 : virtual public TESTTESTTEST12
    {
        public:
            TESTTESTTEST32() { this->setMyOtherValue(10); }

        private:
            void setMyOtherValue(int value) { this->value3_ = value; }

            int value3_;
            TESTTESTTEST22* test_;
    };

    class _OrxonoxExport Projectile : public TESTTESTTEST22, public TESTTESTTEST32, virtual public TESTTESTTEST12, public WorldEntity
    {
        public:
            virtual ~Projectile();
            void setConfigValues();
            void speedChanged();
            void destroyObject();
            virtual void tick(float dt);
            virtual void testfunction();

            static float getSpeed()
                { return Projectile::speed_s; }

        protected:
            Projectile(SpaceShip* owner = 0);
            SpaceShip* owner_;

        private:
            std::string explosionTemplateName_;
            std::string smokeTemplateName_;
        protected:
            static float speed_s;
            float speed_;
        private:
            float lifetime_;
            float damage_;
            Timer<Projectile> destroyTimer_;
    };

    SUPER_FUNCTION(0, Projectile, testfunction, false);
}

#endif /* _Projectile_H__ */
