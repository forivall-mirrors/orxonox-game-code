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

#ifndef _NotificationOverlay_H__
#define _NotificationOverlay_H__

#include "OrxonoxPrereqs.h"

#include "orxonox/overlays/OverlayText.h"

#include <string>
#include <set>
#include <OgrePrerequisites.h>
#include <OgreTextAreaOverlayElement.h>

namespace orxonox
{

    /**
    @brief
        
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport NotificationOverlay : public OverlayText
    {

        public:
            NotificationOverlay(BaseObject* creator);
            NotificationOverlay(NotificationQueue* queue, Notification* notification);
            virtual ~NotificationOverlay();
            
            bool processNotification(Notification* notification);

            void setFontSize(float size)
                { this->setTextSize(size); }
        
        protected:
            const std::string clipMessage(const std::string & message);

        private:
            NotificationQueue* queue_;
            Notification* notification_;

            void initialize(void);
            void defineOverlay(void);

    };

}

#endif /* _NotificationOverlay_H__ */
