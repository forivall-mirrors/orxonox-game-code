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
 *      ...
 *
 */

#ifndef _HUDEnemyHealthBar_H__
#define _HUDEnemyHealthBar_H__

#include "interfaces/RadarViewable.h"
#include "worldentities/pawns/Pawn.h"

#include "HUDHealthBar.h"
#include "interfaces/RadarListener.h"

namespace orxonox
{
    class _OverlaysExport HUDEnemyHealthBar : public HUDHealthBar, public RadarListener
    {
        public:
            HUDEnemyHealthBar(BaseObject* creator);
            virtual ~HUDEnemyHealthBar();
            void setConfigValues();

            bool compareDistance ( std::pair<RadarViewable*, unsigned int > a, std::pair<RadarViewable*, unsigned int > b );

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

            //RadarListener interface
            void addObject ( RadarViewable* object );
            void removeObject ( RadarViewable* viewable );
            void objectChanged(RadarViewable* viewable);
            virtual void radarTick ( float dt ) {}
            inline float getRadarSensitivity() const
                { return 1.0f; }

            void changedOwner();

        private:

            void setSensibility (float sense){
                this->sens_ = sense;}
            float getSensibility(){
                return this->sens_;}

            bool showObject(RadarViewable* rv);
        
            typedef std::list < std::pair<RadarViewable*, unsigned int > > sortedList;
            sortedList sortedObjectList_;

            Pawn* owner_;
            float sens_;
            bool useEnemyBar_;
            unsigned int markerLimit_;

            float currentYaw;
            float currentPitch;
    };
}
#endif /* _HUDEnemyHealthBar_H__ */
