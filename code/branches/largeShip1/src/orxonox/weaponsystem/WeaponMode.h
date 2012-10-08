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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _WeaponMode_H__
#define _WeaponMode_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "util/Math.h"
#include "core/BaseObject.h"
#include "core/SubclassIdentifier.h"
#include "tools/Timer.h"

namespace orxonox
{
    class _OrxonoxExport WeaponMode : public BaseObject
    {
        public:
            WeaponMode(BaseObject* creator);
            virtual ~WeaponMode();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            bool fire(float* reloadTime);
            bool reload();

            // Interacting with the default Firing sound
            void setDefaultSound(const std::string& soundPath);
            const std::string& getDefaultSound();
            void setDefaultSoundWithVolume(const std::string& soundPath, const float soundVolume);

            // Munition
            inline Munition* getMunition() const
                { return this->munition_; }

            void setMunitionType(Identifier* identifier);
            inline Identifier* getMunitionType() const
                { return this->munitiontype_; }

            void setMunitionName(const std::string& munitionname);
            inline const std::string& getMunitionName() const
                { return this->munitionname_; }

            inline void setInitialMunition(unsigned int amount)
                { this->initialMunition_ = amount; }
            inline unsigned int getInitialMunition() const
                { return this->initialMunition_; }

            inline void setInitialMagazines(unsigned int amount)
                { this->initialMagazines_ = amount; }
            inline unsigned int getInitialMagazines() const
                { return this->initialMagazines_; }

            inline void setMunitionPerShot(unsigned int amount)
                { this->munitionPerShot_ = amount; }
            inline unsigned int getMunitionPerShot() const
                { return this->munitionPerShot_; }


            // Reloading
            inline void setReloadTime(float time)
                { this->reloadTime_ = time; }
            inline float getReloadTime() const
                { return this->reloadTime_; }

            inline void setAutoReload(bool autoreload)
                { this->bAutoReload_ = autoreload; }
            inline bool getAutoReload() const
                { return this->bAutoReload_; }

            inline void setParallelReload(bool parallelreload)
                { this->bParallelReload_ = parallelreload; }
            inline bool getParallelReload() const
                { return this->bParallelReload_; }


            // Fire
            inline void setDamage(float damage)
                { this->damage_ = damage;}
            inline float getDamage() const
                { return this->damage_; }
            inline void setHealthDamage(float healthdamage)
                { this->healthdamage_ = healthdamage; }
            inline float getHealthDamage() const
                { return this->healthdamage_; }

            inline void setShieldDamage(float shielddamage)
                { this->shielddamage_ = shielddamage;}
            inline float getShieldDamage() const
                { return this->shielddamage_; }

            inline void setMuzzleOffset(const Vector3& offset)
                { this->muzzleOffset_ = offset; }
            inline const Vector3& getMuzzleOffset() const
                { return this->muzzleOffset_; }

            void computeMuzzleParameters(const Vector3& target);
            const Vector3& getMuzzlePosition() const
                { return this->muzzlePosition_; }
            const Quaternion& getMuzzleOrientation() const
                { return this->muzzleOrientation_; }
            Vector3 getMuzzleDirection() const;


            // Weapon
            inline void setWeapon(Weapon* weapon)
                { this->weapon_ = weapon; this->updateMunition(); }
            inline Weapon* getWeapon() const
                { return this->weapon_; }

            inline void setMode(unsigned int mode)
                { this->mode_ = mode; }
            inline unsigned int getMode() const
                { return this->mode_; }

            Vector3 getTarget();

        protected:
            virtual void fire() = 0;

            unsigned int initialMunition_;
            unsigned int initialMagazines_;
            unsigned int munitionPerShot_;

            float reloadTime_;
            bool bAutoReload_;
            bool bParallelReload_;

            float damage_;
            float healthdamage_;
            float shielddamage_;
            Vector3 muzzleOffset_;

        private:
            void updateMunition();
            void reloaded();

            Weapon* weapon_;
            unsigned int mode_;

            Munition* munition_;
            SubclassIdentifier<Munition> munitiontype_;
            std::string munitionname_;

            Timer reloadTimer_;
            bool bReloading_;

            Vector3 muzzlePosition_;
            Quaternion muzzleOrientation_;

            WorldSound* defSndWpnFire_;
            bool        bSoundAttached_;
    };
}

#endif /* _WeaponMode_H__ */
