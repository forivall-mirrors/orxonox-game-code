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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _WorldSound_H__
#define _WorldSound_H__

#include "OrxonoxPrereqs.h"

#include "tools/interfaces/Tickable.h"
#include "sound/BaseSound.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    /**
    @brief
        The WorldSound class is to be used for sounds with position and orientation.
    */
    class _OrxonoxExport WorldSound : public StaticEntity, public BaseSound, public Tickable
    {
    public:
        WorldSound(Context* context);

        void XMLPort(Element& xmlelement, XMLPort::Mode mode);
        void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);
        void changedActivity();

        void tick(float dt);

    protected:
        ~WorldSound() {}

    private:
        void registerVariables();
        void initialiseSource();
        float getRealVolume();
    };
}

#endif /* _WorldSound_H__ */
