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
 *      Felix Schulthess
 *
 */

/**
@file
@brief
*/

#ifndef _Radar_H__
#define _Radar_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include <string>

#include "core/ObjectListIterator.h"
#include "interfaces/RadarViewable.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox
{
    class _OrxonoxExport Radar : public Tickable
    {
    public:
        Radar();
        virtual ~Radar();

        virtual void tick(float dt);

        const RadarViewable* getFocus();
        RadarViewable::Shape addObjectDescription(const std::string& name);

        void listObjects() const;

        void releaseFocus();
        void cycleFocus();

    private:
        Radar(Radar& instance);

        void updateFocus();

        ObjectListIterator<RadarViewable> itFocus_;
        RadarViewable* focus_;
        std::map<std::string, RadarViewable::Shape> objectTypes_;
        int objectTypeCounter_;
    };
}
#endif /* _Radar_H__ */
