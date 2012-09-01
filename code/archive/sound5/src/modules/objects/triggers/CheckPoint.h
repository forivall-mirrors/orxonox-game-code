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
 *      Aurelian Jaggi
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief
*/

#ifndef _CheckPoint_H__
#define _CheckPoint_H__

#include "objects/ObjectsPrereqs.h"

#include "interfaces/RadarViewable.h"
#include "DistanceTrigger.h"

namespace orxonox
{
    class _ObjectsExport CheckPoint : public DistanceTrigger, public RadarViewable
    {
    public:
        CheckPoint(BaseObject* creator);
        virtual ~CheckPoint();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a CheckPoint object through XML.
        virtual void changedActivity();

    private:
        virtual void triggered(bool bIsTriggered);
        virtual void notifyMaskUpdate();

        inline void setDestination(bool isDestination)
            { bIsDestination_ = isDestination; }

        inline const WorldEntity* getWorldEntity() const
            { return this; }

        inline bool getDestination()
            { return bIsDestination_; }

        inline void setFirst(bool isFirst)
            { this->bIsFirst_ = isFirst; }

        inline bool getFirst()
            { return this->bIsFirst_; }

        inline void setAddTime(float time)
            { this->addTime_ = time; }

        inline bool getAddTime()
            { return this->addTime_; }

        bool bIsFirst_;
        bool bIsDestination_;
        float addTime_;
    };
}

#endif /* _CheckPoint_H__ */
