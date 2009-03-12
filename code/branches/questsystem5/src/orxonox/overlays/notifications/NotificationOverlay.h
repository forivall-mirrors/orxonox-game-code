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

#include "orxonox/overlays/OrxonoxOverlay.h"

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
    class _OrxonoxExport NotificationOverlay : public OrxonoxOverlay
    {
    
        friend class NotificationQueue; //TDO: Best solution?
    
        public:
            NotificationOverlay(BaseObject* creator);
            NotificationOverlay(NotificationQueue* queue, Notification* notification);
            virtual ~NotificationOverlay();
            
            bool processNotification(Notification* notification);
        
        protected:
            const std::string clipMessage(const std::string & message);
            const std::string clipMessage2(const std::string & message); //Outdated
        
        private:
            NotificationQueue* queue_;
            Notification* notification_;
            
            int width_;
            
            Ogre::TextAreaOverlayElement* notificationText_;
            
            void initialize(void);
            void defineOverlay(void);

            virtual void sizeChanged();

            void setAlignment(Ogre::TextAreaOverlayElement::Alignment alignment)
                { this->notificationText_->setAlignment(alignment); }
            void setCaption(const std::string& caption)
                { this->notificationText_->setCaption(caption); }
            void setColour(const ColourValue& colour)
                { this->notificationText_->setColour(colour); }
            void setFont(const std::string & font);
            void setFontSize(float size);
            void setWidth(int width);
            void setAlignmentString(const std::string& alignment);
            void setTextSize(float size)
                { this->setSize(Vector2(size, size)); }
            
            Ogre::TextAreaOverlayElement::Alignment getAlignment() const
                { return this->notificationText_->getAlignment(); }
            std::string getCaption() const
                { return this->notificationText_->getCaption(); }
            float getFontSize(void) const;
            const std::string & getFont(void) const
                { return this->notificationText_->getFontName(); }
            int getWidth(void) const
                { return this->width_; }
            const ColourValue& getColour() const
                { return this->notificationText_->getColour(); }
            std::string getAlignmentString() const;
            float getTextSize() const
                { return this->getSize().y; }

    };

}

#endif /* _NotificationOverlay_H__ */
