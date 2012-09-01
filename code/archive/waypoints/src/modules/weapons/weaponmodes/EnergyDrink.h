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

/**
    @file EnergyDrink.h
    @brief Definition of the EnergyDrink class.
*/

#ifndef _EnergyDrink_H__
#define _EnergyDrink_H__

#include "weapons/WeaponsPrereqs.h"

#include <string>
#include "tools/Timer.h"
#include "weaponsystem/WeaponMode.h"

namespace orxonox
{

    /**
    @brief
        Shoots a can.
    @author
        Hagen Seifert
    @ingroup WeaponsWeaponModes
    */
    class _WeaponsExport EnergyDrink : public WeaponMode
    {
        public:
            EnergyDrink(BaseObject* creator);
            virtual ~EnergyDrink() {}

            virtual void fire();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        private:
            /**
            @brief Set the material of the EnergyDrink.
            @param material The name of the material.
            */
            void setMaterial(const std::string& material)
                { this->material_ = material; }
            /**
            @brief Get the material of the EnergyDrink.
            @return Returns the material name.
            */
            inline const std::string& getMaterial() const
                { return this->material_; }

            void setDelay(float delay);
            /**
            @brief Get the firing delay.
            @return Returns the delay in seconds.
            */
            float getDelay() const
                { return this->delay_; }

            void shot();
            void muzzleflash();

            std::string material_; //!< The material.
            float speed_; //!< The speed of the EnergyDrink.
            float delay_; //!< The firing delay.
            Timer delayTimer_; //!< The timer to delay the firing.
    };
}

#endif /* _EnergyDrink_H__ */
