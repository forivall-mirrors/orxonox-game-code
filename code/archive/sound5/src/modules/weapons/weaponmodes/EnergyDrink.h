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
 *      Hagen Seifert
 *   Co-authors:
 *      ...
 *
 */

#ifndef _EnergyDrink_H__
#define _EnergyDrink_H__

#include "weapons/WeaponsPrereqs.h"

#include <string>
#include "tools/Timer.h"
#include "weaponsystem/WeaponMode.h"

namespace orxonox
{
    class _WeaponsExport EnergyDrink : public WeaponMode
    {
        public:
            EnergyDrink(BaseObject* creator);
            virtual ~EnergyDrink() {}

            virtual void fire();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        private:
            void setMaterial(const std::string& material);
            inline const std::string& getMaterial()
                { return this->material_; }
            void setDelay(float d);
            float getDelay() const;
            void shot();
            void muendungsfeuer();

            std::string material_;
            float speed_;
            float delay_;
            Timer delayTimer_;
    };
}

#endif /* _EnergyDrink_H__ */
