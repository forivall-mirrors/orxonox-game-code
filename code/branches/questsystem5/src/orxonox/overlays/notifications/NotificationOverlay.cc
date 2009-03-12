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

#include "OrxonoxStableHeaders.h"
#include "NotificationOverlay.h"

#include <OgreOverlayManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include "core/CoreIncludes.h"
#include "util/Exception.h"

#include "Notification.h"
#include "NotificationQueue.h"

namespace orxonox
{

    NotificationOverlay::NotificationOverlay(BaseObject* creator) : OrxonoxOverlay(creator)
    {
        this->initialize();
    }
    
    NotificationOverlay::NotificationOverlay(NotificationQueue* queue, Notification* notification) : OrxonoxOverlay(this)
    {
        this->initialize();
        
        if(notification == NULL || queue == NULL)
        {
            ThrowException(Argument, "There were NULL-Pointer arguments in NotificationOverlay creation.");
        }

        this->queue_ = queue;
        this->defineOverlay();
        
        this->processNotification(notification);

        COUT(3) << getCaption() << std::endl;
        COUT(3) << getFontSize() << std::endl;
        //COUT(3) << getFont() << std::endl;
        COUT(3) << getWidth() << std::endl;
        COUT(3) << getAlignmentString() << std::endl;
        COUT(3) << getTextSize() << std::endl;
    }
    
    void NotificationOverlay::initialize(void)
    {
        RegisterObject(NotificationOverlay);
        
        this->queue_ = NULL;
        this->notificationText_ = NULL;
        this->width_ = 0;
    }
    
    void NotificationOverlay::defineOverlay(void)
    {
        this->setFont(this->queue_->getFont());
        this->setFontSize(this->queue_->getFontSize());
        
        this->notificationText_ = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "NotificationOverlay_notificationText_" + getUniqueNumberString()));
        this->notificationText_->setCharHeight(1.0);
        this->notificationText_->setColour(ColourValue(1.0, 1.0, 1.0, 1.0));
        this->notificationText_->setAlignment(Ogre::TextAreaOverlayElement::Center);
        this->notificationText_->setCaption("");
        
        this->background_->addChild(this->notificationText_);
        COUT(3) << this->queue_->getFont() << std::endl;
    }
    
    NotificationOverlay::~NotificationOverlay()
    {
    
    }
    
    void NotificationOverlay::setFont(const std::string & font)
    {
        if (this->notificationText_ && font != "")
            this->notificationText_->setFontName(font);
    }

    void NotificationOverlay::setFontSize(float size)
    {
        if (this->notificationText_ && size >= 0)
            this->notificationText_->setCharHeight(size);
    }
    
    void NotificationOverlay::setWidth(int width)
    {
        this->width_ = width;
        this->notificationText_->setCaption(clipMessage(this->notification_->getMessage()));
    }

    float NotificationOverlay::getFontSize(void) const
    {
        if (this->notificationText_)
            return this->notificationText_->getCharHeight();
        else
            return 0;
    }
    
    bool NotificationOverlay::processNotification(Notification* notification)
    {
        this->notificationText_->setCaption(clipMessage(notification->getMessage()));
        this->notification_ = notification;
        return true;
    }

    const std::string NotificationOverlay::clipMessage(const std::string & str)
    {
        if(str.length() <= (unsigned int)this->queue_->getNotificationLength())
            return str;
        return str.substr(0, this->queue_->getNotificationLength());
    }

    const std::string NotificationOverlay::clipMessage2(const std::string & str)
    {
    
        std::string message = str;
        unsigned int i = 0;
        
        unsigned int found = message.find("\\n", i);
        while(found != std::string::npos)
        {
            message.replace(found, 2, "\n");
            i = found+2;
            found = message.find("\\n", i);
        }
    
        std::string clippedMessage = "";
        int wordLength = 0;
        i = 0;
        int widthLeft = this->getWidth();
        while(i < message.length())
        {
            while(i < message.length() && message[i] != ' ' && message[i] != '\n')
            {
                i++;
                wordLength++;
            }
            
            if(wordLength <= widthLeft)
            {
                clippedMessage = clippedMessage + message.substr(i-wordLength, wordLength);
                if(i < message.length())
                {
                    clippedMessage = clippedMessage + message.substr(i,1);
                }
                widthLeft -= (wordLength+1);
                if(message[i] == '\n')
                {
                    widthLeft = this->getWidth() - (wordLength+1);
                }
                wordLength = 0;
                i++;
            }
            else
            {
                clippedMessage.push_back('\n');
                clippedMessage = clippedMessage + message.substr(i-wordLength, wordLength);
                if(i < message.length())
                {
                    clippedMessage = clippedMessage + message.substr(i,1);
                }
                widthLeft = this->getWidth() - (wordLength+1);
                i++;
                wordLength = 0;
            }
        }
        
        return clippedMessage;
    }

    void NotificationOverlay::sizeChanged()
    {
        if (this->rotState_ == Horizontal)
            this->overlay_->setScale(size_.y * sizeCorrection_.y, size_.y * sizeCorrection_.y);
        else if (this->rotState_ == Vertical)
            this->overlay_->setScale(size_.y / (sizeCorrection_.y * sizeCorrection_.y), size_.y * sizeCorrection_.y);
        else
            this->overlay_->setScale(size_.y, size_.y);

        positionChanged();
    }

    void NotificationOverlay::setAlignmentString(const std::string& alignment)
    {
        if (alignment == "right")
            this->setAlignment(Ogre::TextAreaOverlayElement::Right);
        else if (alignment == "center")
            this->setAlignment(Ogre::TextAreaOverlayElement::Center);
        else // "left" and default
            this->setAlignment(Ogre::TextAreaOverlayElement::Left);
    }

    std::string NotificationOverlay::getAlignmentString() const
    {
        Ogre::TextAreaOverlayElement::Alignment alignment = this->notificationText_->getAlignment();

        switch (alignment)
        {
            case Ogre::TextAreaOverlayElement::Right:
                return "right";
            case Ogre::TextAreaOverlayElement::Center:
                return "center";
            case Ogre::TextAreaOverlayElement::Left:
                return "left";
            default:
                assert(false); return "";
        }
    }

}
