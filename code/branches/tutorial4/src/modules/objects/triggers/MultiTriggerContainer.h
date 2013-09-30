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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
*/

/**
    @file MultiTriggerContainer.h
    @brief Definition of the MultiTriggerContainer class.
    @ingroup MultiTrigger
*/

#ifndef _MultiTriggerContainer_H__
#define _MultiTriggerContainer_H__

#include "objects/ObjectsPrereqs.h"

#include "core/BaseObject.h"

#include "interfaces/PlayerTrigger.h"

namespace orxonox
{

    /**
    @brief
        This class is used by the MultiTrigger class to transport additional data via @ref orxonox::Event "Events".

    @author
        Damian 'Mozork' Frick

    @ingroup MultiTrigger
    */
    class _ObjectsExport MultiTriggerContainer : public BaseObject, public PlayerTrigger
    {

        public:
            MultiTriggerContainer(Context* context); //!< Default constructor. Registers the object and creates an empty container.
            MultiTriggerContainer(Context* context, MultiTrigger* originator, BaseObject* data); //!< Constructor. Registers the object and sets the input values.
            virtual ~MultiTriggerContainer(); //!< Destructor.

            /**
            @brief Get the originator of the Event. (The MultiTrigger that fired the Event)
            @return Returns a pointer to the originator.
            */
            inline MultiTrigger* getOriginator(void)
                { return this->originator_; }
            /**
            @brief Get the data of this container.
            @return Returns a pointer ot the data.
            */
            inline BaseObject* getData(void)
                { return this->data_; }

        private:
            MultiTrigger* originator_; //!< The originator.
            BaseObject* data_; //!< The data.

    };

}

#endif // _MultiTriggerContainer_H__
