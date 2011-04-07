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
 *      Maurus Kaufmann
 *   Co-authors:
 *      ...
 *
 */
 
 /* TODO: - Markiere SpaceBoundaries-Position mit einem schoenen Objekt 
          - Kugel-Model mal hinzufuegen, das nur sichtbar ist, wenn man genuegend nah an maxDistance dran ist
          - Reflexion an obiger Kugel beim Versuch durchzudringen
 */

#ifndef _SpaceBoundaries_H__
#define _SpaceBoundaries_H__

/* Einige, spezifische include-Statements */
#include "core/CoreIncludes.h"
#include "tools/interfaces/Tickable.h"
#include "interfaces/RadarViewable.h"
#include "worldentities/StaticEntity.h"
#include "worldentities/WorldEntity.h"

//#include <ColoredTextAreaOverlayElementFactory.h>

#include <string>

/**
@brief SpaceBoundaries gives level creators the possibility to bar Pawns from leaving a defined area.

       Four attributes can/should be defined in the XML-File:
       'position', 'warnDistance', 'maxDistance', 'healthDecrease'.
*/

namespace orxonox
{
    class _OrxonoxExport SpaceBoundaries : public StaticEntity, public Tickable
    {
        public:
            SpaceBoundaries(BaseObject* creator);
            ~SpaceBoundaries();
            
            void setMaxDistance(float r);
            float getMaxDistance();
            
            void setWarnDistance(float r);
            float getWarnDistance();
            
            void setHealthDecrease(float amount);
            float getHealthDecrease();

            void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            
            void tick(float dt);

        private:
            float maxDistance_; //!< maximal zulaessige Entfernung von 'this->getPosition()'.
            float warnDistance_; //!< Entfernung von 'this->getPosition()', ab der eine Warnung angezeigt wird, dass man bald das zulaessige Areal verlaesst.
            
            float healthDecrease_; //!< Mass fuer die Anzahl Health-Points, die nach ueberschreiten der Entfernung 'maxDistance_' von 'this->getPosition()' abgezogen werden.
                                   //!< Empfohlene Werte: 0.1 (langsame Health-Verminderung) bis 5 (sehr schnelle Health-Verminderung)
            
            RadarViewable* centerRadar_; //!< Repraesentation von 'this->getPosition()' auf dem Radar.
        
            float computeDistance(WorldEntity *item); //!< Auf den Mittelpunkt 'this->getPosition()' bezogen.
            void displayWarning(const std::string warnText);
            bool isHumanPlayer(Pawn *item);
            
 //           ColoredTextAreaOverlayElementFactory* pColoredTextAreaOverlayElementFactory;
    };
}

#endif /* _SpaceBoundaries_H__ */
