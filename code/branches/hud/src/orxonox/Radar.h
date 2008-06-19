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

/**
    @file
    @brief
*/

#ifndef _Radar_H__
#define _Radar_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "core/Iterator.h"
#include "core/OrxonoxClass.h"
#include "objects/Tickable.h"
#include "RadarViewable.h"

namespace orxonox
{
    class _OrxonoxExport RadarListener : virtual public OrxonoxClass
    {
    public:
        RadarListener();
        virtual ~RadarListener() { }

        virtual void displayObject(RadarViewable* viewable, bool bIsMarked) = 0;
        virtual void hideMarker() = 0;
        virtual float getRadarSensitivity() = 0;
        virtual void radarTick(float dt) = 0;
    };

    /**
    @brief This class merely ensures that no one can inherit from Radar.
    */
    class _OrxonoxExport RadarBase
    {
    private:
        friend class Radar;
        RadarBase() { }
    };
 
    class _OrxonoxExport Radar : public Tickable, private virtual RadarBase
    {
    public:
        Radar();
        ~Radar();

        //void unregisterObject(RadarViewable* object);
        const RadarViewable* getFocus();
        RadarViewable::Shape addObjectDescription(const std::string name);

        static Radar& getInstance();
        static Radar* getInstancePtr() { return instance_s; }

        static void cycleNavigationFocus();
        static void releaseNavigationFocus();

    private:
        Radar(Radar& instance);
        void tick(float dt);

        void releaseFocus();
        void updateFocus();
        void cycleFocus();

        Iterator<RadarViewable> itFocus_;
        RadarViewable* focus_;
        std::map<std::string, RadarViewable::Shape> objectTypes_;
        int objectTypeCounter_; 

        static Radar* instance_s;
    };
}
#endif /* _Radar_H__ */
