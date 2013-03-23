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
#include <set>
#include <string>

#include "core/object/ObjectListIterator.h"
#include "interfaces/RadarViewable.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox
{
    class _OrxonoxExport Radar : public Tickable
    {
        friend class RadarViewable;
    public:
        Radar();
        virtual ~Radar();

        virtual void tick(float dt);

        const RadarViewable* getFocus();
        RadarViewable::Shape addObjectDescription(const std::string& name);

        void listObjects() const;
        const std::set<RadarViewable*>& getRadarObjects() const
            { return this->radarObjects_; }

        void releaseFocus();
        void cycleFocus();

    private:
        Radar(Radar& instance);

        void updateFocus();
        void addRadarObject(RadarViewable* rv);
        void removeRadarObject(RadarViewable* rv);
        void radarObjectChanged(RadarViewable* rv);

        ObjectListIterator<RadarViewable> itFocus_;
        RadarViewable* focus_;
        std::map<std::string, RadarViewable::Shape> objectTypes_;
        std::set<RadarViewable*> radarObjects_;
        int objectTypeCounter_;
    };
}
#endif /* _Radar_H__ */
