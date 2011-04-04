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
 *      Sven Stucki
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Dock.h
    @brief Definition of Dock class
    @ingroup Docking
*/

#ifndef _Dock_H__
#define _Dock_H__

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/EventIncludes.h"

#include "worldentities/StaticEntity.h"
#include "DockingEffect.h"
#include "DockingPrereqs.h"

namespace orxonox { 


    class _DockingExport Dock : public StaticEntity {
    public:
        Dock(BaseObject* creator);
        virtual ~Dock();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
        virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);

        bool execute(bool bTriggered, BaseObject* trigger);

        bool addEffect(DockingEffect* effect); //!< Add a DockingEffect to the Dock.
        const DockingEffect* getEffect(unsigned int index) const; //!< Get the DockingEffect at a given index.

    private:
        std::list<DockingEffect*> effects_; //!< The list of DockingEffects to be executed when a player docks.
    };


}

#endif /* _Dock_H__ */
