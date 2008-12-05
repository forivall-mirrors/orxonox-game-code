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

#ifndef _NotificationManager_H__
#define _NotificationManager_H__

#include "OrxonoxPrereqs.h"

#include "core/BaseObject.h"

#include <list>
#include <string>

namespace orxonox {

    struct NotificationContainer
    {
        Notification* notification;
        float remainingTime;
    };

    /**
    @brief
        
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport NotificationManager : public BaseObject
    {
    
        public:
	    NotificationManager(BaseObject* creator);
	    virtual ~NotificationManager();
	    
	    static bool insertNotification(Notification* notification);
	    
	    static void tick(float dt);
	    
	private:
            static std::list<NotificationContainer*> notifications_s;
            
            static void updateQueue(void);
            static const std::string & clipMessage(const std::string & message);

    };

}

#endif /* _NotificationManager_H__ */
