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
 *      Daniel 'Huty' Haggenmueller
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Definition of DroppedItem
*/

#ifndef _DroppedItem_H__
#define _DroppedItem_H__

#include "OrxonoxPrereqs.h"

#include "tools/Timer.h"
#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport DroppedItem : public StaticEntity, public Tickable
    {
    public:
        DroppedItem(BaseObject* creator);
        virtual ~DroppedItem();

        //TODO: Comment.
        //DroppedItem -> Item with no owner, alone in space?
        //Would be much nicer if it would be triggered by a standard issue DistanceTrigger.
        //Where is this created? I see no XMLPort.
        //Where is the item for this created? What happens if more than one pawn triggers this?
        //Add more than just one items, or even better create the ability to add a Collection.? Rename to ...?

        void tick(float dt);
        void trigger(Pawn* pawn);

        static DroppedItem* createDefaultDrop(BaseItem* item, const Vector3& position, const ColourValue& flareColour = ColourValue(0.5f, 1.0f, 0.3f), float timeToLive = 0);
        static DroppedItem* createDefaultDrop(BaseItem* item, Pawn* pawn, const ColourValue& flareColour = ColourValue(0.5f, 1.0f, 0.3f), float timeToLive = 0);

        void createTimer(); //TODO: Can this be made private, too?
        void timerCallback(); //TODO: This should really be private.

        inline float getTriggerDistance() const
            { return this->triggerDistance_; }
        inline void setTriggerDistance(float distance)
            { this->triggerDistance_ = distance; }

        inline BaseItem* getItem() const
            { return this->item_; }
        //TODO: Needs to be public?
        inline void setItem(BaseItem* item)
            { this->item_ = item; }

        inline float getTimeToLive() const
            { return this->timeToLive_; }
        //TODO: Needs to be public?
        inline void setTimeToLive(float time)
            { this->timeToLive_ = time; }
    private:
        float timeToLive_;
        float triggerDistance_;
        BaseItem* item_;

        Timer timer_;
    };
}

#endif /* _DroppedItem_H__ */
