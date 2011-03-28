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
 *   Authors:
 *      Martin Polak
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Munition_H__
#define _Munition_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include "core/BaseObject.h"
#include "tools/Timer.h"

namespace orxonox
{
    class _OrxonoxExport Munition : public BaseObject
    {
        struct Magazine
        {
            public:
                Magazine(Munition* munition, bool bUseReloadTime = true);
                virtual ~Magazine() {}

                unsigned int munition_;
                Timer loadTimer_;
                bool bLoaded_;

            private:
                void loaded(Munition* munition);
        };

        public:
            Munition(BaseObject* creator);
            virtual ~Munition();

            unsigned int getNumMunition(WeaponMode* user) const;
            unsigned int getNumMunitionInCurrentMagazine(WeaponMode* user) const;
            unsigned int getNumMagazines() const;

            unsigned int getMaxMunition() const;
            inline unsigned int getMaxMagazines() const
                { return this->maxMagazines_; }
            inline unsigned int getMaxMunitionPerMagazine() const
                { return this->maxMunitionPerMagazine_; }

            bool canTakeMunition(unsigned int amount, WeaponMode* user) const;
            bool takeMunition(unsigned int amount, WeaponMode* user);

            bool canReload() const;
            bool needReload(WeaponMode* user) const;
            bool reload(WeaponMode* user, bool bUseReloadTime = true);
            inline float getReloadTime() const
                { return this->reloadTime_; }

            bool canAddMunition(unsigned int amount) const;
            bool addMunition(unsigned int amount);

            bool canAddMagazines(unsigned int amount) const;
            bool addMagazines(unsigned int amount);

            bool canRemoveMagazines(unsigned int amount) const;
            bool removeMagazines(unsigned int amount);

            bool dropMagazine(WeaponMode* user);

        protected:
            unsigned int maxMunitionPerMagazine_;
            unsigned int maxMagazines_;
            unsigned int magazines_;
            std::map<WeaponMode*, Magazine*> currentMagazines_;

            bool bUseSeparateMagazines_;
            bool bStackMunition_;
            bool bAllowMunitionRefilling_;
            bool bAllowMultiMunitionRemovementUnderflow_;

            float reloadTime_;

        private:
            Magazine* getMagazine(WeaponMode* user) const;
    };
}

#endif /* _Munition_H__ */
