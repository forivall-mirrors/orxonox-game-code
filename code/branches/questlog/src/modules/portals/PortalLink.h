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
 *      Andreas Büchel
 *   Co-authors:
 *      ...
 *
 */

/**
    @file PortalLink.h
    @brief Declaration of the PortalLink class
    @ingroup Portals
 */

#ifndef _PortalLink_H__
#define _PortalLink_H__

#include "portals/PortalsPrereqs.h"

#include <map>

#include "PortalEndPoint.h"

#include "core/BaseObject.h"

namespace orxonox
{
    /**
        @brief
            A PortalLink represents the connection between two @ref orxonox::PortalEndPoint "PortalEndPoints"
        @ingroup Portals
     */
    class _PortalsExport PortalLink : public BaseObject
    {
        public:
            PortalLink(BaseObject* creator);
            virtual ~PortalLink();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            inline void setFromID(unsigned int from)    //!< set the ID of the PortalEndPoint which should act as the entrance of this link
                { this->fromID_ = from; }
            inline unsigned int getFromID(unsigned int) const
                { return this->fromID_; }
            inline void setToID(unsigned int to)     //!< set the ID of the PortalEndPoint which should act as the exit of this link
                { this->toID_ = to; }
            inline unsigned int getToID(unsigned int) const
                { return this->toID_; }
            /*! \brief Let an entity enter a certain PortalEndPoint
                \param entity pointer to the entity which is entering a PortalEndPoint
                \param entrance pointer to the PortalEndPoint to enter
             */
            static void use(MobileEntity * entity, PortalEndPoint * entrance);   //!< let entity enter the PortalEndPoint pointed to by entrance
        protected:
        private:
            static std::map<PortalEndPoint *, PortalEndPoint *> links_s;   //!< maps entrances to exits
            unsigned int fromID_;   //!< id of the entrance of this Link
            unsigned int toID_;   //!< id of the exit of this Link
            PortalEndPoint* from_;   //!< pointer to this Link's entrance
            PortalEndPoint* to_;   //!< pointer to this Link's exit
    };
}

#endif /* _Portals_H__ */
