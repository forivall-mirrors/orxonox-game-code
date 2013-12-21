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
    @file HsW01.h
    @brief Definition of the HsW01 class.
*/

#ifndef _HsW01_H__
#define _HsW01_H__

#include "weapons/WeaponsPrereqs.h"

#include "tools/Timer.h"
#include "weaponsystem/WeaponMode.h"

namespace orxonox
{

    /**
    @brief
        Shoots laser beams.
    @author
        Hagen Seifert
    @ingroup WeaponsWeaponModes
    */
    class _WeaponsExport HsW01 : public WeaponMode
    {
        public:
            HsW01(Context* context);
            virtual ~HsW01();

            virtual void fire();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        protected:
            /**
            @brief Set the mesh.
            @param mesh The mesh name.
            */
            void setMesh(const std::string& mesh)
                { this->mesh_ = mesh; }

            /**
            @brief Get the mesh.
            @return Returns the mesh name.
            */
            const std::string& getMesh() const
                { return this->mesh_; }

	    /**
            @brief Set the sound.
            @param mesh The Sound name.
            */
            void setSound(const std::string& sound)
                { this->sound_ = sound; }

	    /**
            @brief Get the sound.
            @return Returns the sound name.
            */
            const std::string& getSound() const
                { return this->sound_; }

            /**
            @brief Set the material.
            @param material The material name.
            */
            void setMaterial(const std::string& material)
                { this->material_ = material; }
            /**
            @brief Get the material.
            @return Returns the material name.
            */
            const std::string& getMaterial() const
                { return this->material_; }

            void setDelay(float delay);
            /**
            @brief Get the firing delay.
            @return Returns the firing delay in seconds.
            */
            float getDelay() const
                { return this->delay_; }

            virtual void shot();
            void muzzleflash();

            std::string material_; //!< The material.
            std::string mesh_; //!< The mesh.
            std::string sound_; //!< The sound.



            float speed_; //!< The speed of the fired projectile.
            float delay_; //!< The firing delay.
            Timer delayTimer_; //!< A timer to delay the firing.
    };
}

#endif /* _HsW01_H__ */
