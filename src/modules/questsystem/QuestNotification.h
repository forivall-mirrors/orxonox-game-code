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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See thes
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

#ifndef _QuestNotification_H__
#define _QuestNotification_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <string>
#include "notifications/Notification.h"

namespace orxonox {

    /**
    @brief
        The QuestNotification is a special Notification that has the single property that it is only sent by the questsystem.
    @author
        Damian 'Mozork' Frick
    */
    class _QuestsystemExport QuestNotification : public Notification
    {
        public:
            QuestNotification(BaseObject* creator); //!< Default Constructor.
            QuestNotification(BaseObject* creator, const std::string & message); //!< Constructor.
            virtual ~QuestNotification(); //!< Destructor.

            bool send(void); //!< Send the QuestNotification.

        private:
            static const std::string SENDER; //!< A string identifying the questsystem as the sender.

    };

}

#endif /* _QuestNotification_H__ */
