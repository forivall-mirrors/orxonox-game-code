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
 *      Martin Polak
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Munition_H__
#define _Munition_H__

#include "OrxonoxPrereqs.h"

#include "core/BaseObject.h"

#include "Weapon.h"


namespace orxonox
{
    class _OrxonoxExport Munition : public BaseObject
    {
        public:
            Munition(BaseObject* creator);
            virtual ~Munition();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void setMaxBullets(unsigned int amount);
            void setMaxMagazines(unsigned int amount);

            void fillBullets();
            void fillMagazines();

            bool bullets();
            bool magazines();

            void removeBullets(unsigned int k, Weapon * parentWeapon);
            void removeMagazines(unsigned int k);
            void addBullets(unsigned int k);
            void addMagazines(unsigned int k);

        private:

        protected:
            unsigned int bullets_;
            unsigned int magazines_;
            unsigned int maxBullets_;
            unsigned int maxMagazines_;
    };
}

#endif /* _Munition_H__ */
