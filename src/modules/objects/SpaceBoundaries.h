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

#ifndef _SpaceBoundaries_H__
#define _SpaceBoundaries_H__

#include "objects/ObjectsPrereqs.h"

#include <string>
#include <list>
#include <vector>

#include "core/CoreIncludes.h"
#include "core/object/WeakPtr.h"

#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{

    /**
    @brief SpaceBoundaries gives level creators the possibility to bar Pawns from leaving a defined area (until now this area is a ball).

        Some attributes can/should be defined in the XML-File:
        - 'position' : absolute position of the object of SpaceBoundaries in the level (usually 0,0,0)
        - 'maxDistance' : defines the area, where a pawn is allowed to be (radius of a ball).
        - 'warnDistance' : If the distance between the pawn of the human player and 'position' is bigger than 'warnDistance', a message is displayed to
                            inform the player that he'll soon be leaving the allowed area. (not implemented yet!)
        - 'showDistance' : If the distance between the pawn and the boundary of the allowed area is smaller than 'showDistance', the boundary is shown.
        - 'reactionMode' : Integer-Value. Defines what effect appears if a space ship has crossed the boundaries.
                                0: Reflect the space ship (default).
                                1: Decrease Health of the space ship after having left the allowed area.
                                2: Inverted Version of 0. Prohibit to fly INTO a defined area.
        - 'healthDecrease' : a measure to define how fast the health of a pawn should decrease after leaving the allowed area (unnecessary if 'reactionMode' == 0).
                                Recommended values: 0.1 (slow health decrease) to 5 (very fast health decrease)

        Follow http://www.orxonox.net/wiki/SpaceBoundaries to get some further information.

        Examples:
        Two examples how one could include SpaceBoundaries in the XML-File. The first one uses reflection, the second one health decrease.
        @code
        <SpaceBoundaries position="0,0,0" maxDistance="1000" warnDistance="800" showDistance="100" reactionMode="0" />
        @endcode

        @code
        <SpaceBoundaries position="0,0,0" maxDistance="1000" warnDistance="800" showDistance="100" reactionMode="1" healthDecrease="0.2" />
        @endcode
    */
    class _ObjectsExport SpaceBoundaries : public StaticEntity, public Tickable
    {
        public:
            SpaceBoundaries(BaseObject* creator);
            ~SpaceBoundaries();

            void setMaxDistance(float r);
            float getMaxDistance();

            void setWarnDistance(float r);
            float getWarnDistance();

            void setShowDistance(float r);
            float getShowDistance();

            void setHealthDecrease(float amount);
            float getHealthDecrease();

            void setReaction(int mode);
            int getReaction();

            void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void tick(float dt);

        private:
            struct BillboardAdministration{ bool usedYet; Billboard* billy; };

            // Variabeln::
            std::list<WeakPtr<Pawn> > pawnsIn_; //!< List of the pawns that this instance of SpaceBoundaries has to handle.

            std::vector<BillboardAdministration> billboards_;

            int reaction_; //!< Values: 0, 1, 2.
                           //!< 0: Reflection on boundary (Standard).
                           //!< 1: Decrease-Health-Mode.
                           //!< 2: Inverted Version of 0. Prohibit to fly INTO a defined area.
            float maxDistance_; //!<  Maximum allowed distance.
            float warnDistance_; //!< Distance in which a warning is displayed.
            float showDistance_; //!< Distance at which the boundaries are displayed.

            float healthDecrease_; //!< Rate of health loss.

            //RadarViewable* centerRadar_; //!< Representation of the space boundary in the radar.

            // Funktionen::
            float computeDistance(WorldEntity *item); //!< Compute distance to center point.
            void displayWarning(const std::string warnText); //!< TODO: Implement.
            void displayBoundaries(Pawn *item, float alpha);
            void conditionalBounceBack(Pawn *item, float currentDistance, float dt);
            void bounceBack(Pawn *item, Vector3 *normal, Vector3 *velocity);
            bool isHumanPlayer(Pawn *item);

            void checkWhoIsIn(); //!< Update the list 'pawnsIn_'.

            void positionBillboard(const Vector3& position, float alpha); //!< Display a Billboard at the position 'position'.
            void setBillboardOptions(Billboard *billy);
            void removeAllBillboards(); //!< Hide all elements of '*billboard_' and set their attribute 'usedYet' to 0.
    };
}

#endif /* _SpaceBoundaries_H__ */
