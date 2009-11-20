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

    @author
        Damian 'Mozork' Frick
    */
    class _QuestsystemExport QuestNotification : public Notification
    {
        public:
            QuestNotification(BaseObject* creator);
            QuestNotification(const std::string & message);
            virtual ~QuestNotification();

            bool send(void);

        private:
            static const std::string SENDER;

            void initialize(void);

    };

}

#endif /* _QuestNotification_H__ */
