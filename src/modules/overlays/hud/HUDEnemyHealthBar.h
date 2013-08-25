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
 *      Matthias Spalinger
 *   Co-authors:
 *      Fabian 'x3n' Landau
 *
 */

#ifndef _HUDEnemyHealthBar_H__
#define _HUDEnemyHealthBar_H__

#include "HUDHealthBar.h"

namespace orxonox
{
    class _OverlaysExport HUDEnemyHealthBar : public HUDHealthBar
    {
        public:
            HUDEnemyHealthBar(Context* context);
            virtual ~HUDEnemyHealthBar();

            void setConfigValues();
            virtual void tick(float dt);

            void changedOwner();

        private:
            void updateTarget();

            ControllableEntity* owner_;
            bool useEnemyBar_;
    };
}
#endif /* _HUDEnemyHealthBar_H__ */
