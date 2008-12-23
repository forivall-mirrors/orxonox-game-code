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

#ifndef _NotificationOueue_H__
#define _NotificationOueue_H__

#include "OrxonoxPrereqs.h"
#include <OgreTextAreaOverlayElement.h>

#include "orxonox/overlays/OverlayText.h"
#include "orxonox/objects/Tickable.h"

#include <string>

namespace orxonox
{
    /**
    @brief
        
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport NotificationQueue : public OverlayText, public Tickable
    {
    public:
        NotificationQueue(BaseObject* creator);
        virtual ~NotificationQueue();
        
        static NotificationQueue* queue_s; //TDO Singleton? oder im level.
        
        virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);
        
        virtual void tick(float dt);
        
        void update(void);
        
        int getLength(void) const
                { return this->length_; }
        int getWidth(void) const
                { return this->width_; }
        
        void setQueueText(const std::string & text);
        bool setLength(int length);
        bool setWidth(int width);
        
    private:
        Ogre::UTFString queueText_;
        int length_;
        int width_;
    
    };

}

#endif /* _NotificationOverlay_H__ */
