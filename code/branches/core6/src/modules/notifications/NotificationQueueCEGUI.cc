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
    @file NotificationQueueCEGUI.cc
    @brief Implementation of the NotificationQueueCEGUI class.
*/

#include "NotificationQueueCEGUI.h"

#include <sstream>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/GUIManager.h"
#include "core/LuaState.h"
#include "core/XMLPort.h"
#include "util/Convert.h"

namespace orxonox
{

    CreateFactory(NotificationQueueCEGUI);

    /*static*/ const std::string NotificationQueueCEGUI::NOTIFICATION_LAYER("NotificationLayer");

    NotificationQueueCEGUI::NotificationQueueCEGUI(Context* context) : NotificationQueue(context)
    {
        RegisterObject(NotificationQueueCEGUI);

        this->initialize();
        this->registerVariables();
    }
    
    NotificationQueueCEGUI::~NotificationQueueCEGUI()
    {
        if(this->isRegistered() && GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".removeQueue(\"" + this->getName() +  "\")");
    }

    /**
    @brief
        Initializes The NotificationQueueCEGUI.
    */
    void NotificationQueueCEGUI::initialize(void)
    {
        this->displaySize_ = Vector4(1.0, 0.0, 0.0, 0.0);
        this->position_ = Vector4(0.0, 0.0, 0.0, 0.0);
        this->alignment_ = "LeftAligned";
        this->fontSize_ = 12;
        this->fontColor_ = Vector4(1.0, 1.0, 1.0, 1.0);
        this->fontColorStr_ = "FFFFFFFF";
    }

    void NotificationQueueCEGUI::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(NotificationQueueCEGUI, XMLPort, xmlelement, mode);

        XMLPortParam(NotificationQueueCEGUI, "position", setPosition, getPosition, xmlelement, mode);
        XMLPortParam(NotificationQueueCEGUI, "fontSize", setFontSize, getFontSize, xmlelement, mode);
        XMLPortParam(NotificationQueueCEGUI, "fontColor", setFontColor, getFontColor, xmlelement, mode);
        XMLPortParam(NotificationQueueCEGUI, "alignment", setAlignment, getAlignment, xmlelement, mode);
        XMLPortParam(NotificationQueueCEGUI, "displaySize", setDisplaySize, getDisplaySize, xmlelement, mode);
    }
    
    void NotificationQueueCEGUI::registerVariables()
    {
        registerVariable( this->position_, VariableDirection::ToClient, new NetworkCallback<NotificationQueueCEGUI>(this, &NotificationQueueCEGUI::positionChanged));
        registerVariable( this->fontSize_, VariableDirection::ToClient, new NetworkCallback<NotificationQueueCEGUI>(this, &NotificationQueueCEGUI::fontSizeChanged));
        registerVariable( this->fontColor_, VariableDirection::ToClient, new NetworkCallback<NotificationQueueCEGUI>(this, &NotificationQueueCEGUI::fontColorChanged));
        registerVariable( this->alignment_, VariableDirection::ToClient, new NetworkCallback<NotificationQueueCEGUI>(this, &NotificationQueueCEGUI::alignmentChanged));
        registerVariable( this->displaySize_, VariableDirection::ToClient, new NetworkCallback<NotificationQueueCEGUI>(this, &NotificationQueueCEGUI::displaySizeChanged));
    }

    void NotificationQueueCEGUI::changedName(void)
    {
        SUPER(NotificationQueueCEGUI, changedName);

        this->positionChanged();
        this->fontSizeChanged();
        this->fontColorChanged();
        this->alignmentChanged();
        this->displaySizeChanged();
    }

    /**
    @brief
        Destroys the NotificationQueueCEGUI.
        Used in lua and NotificationManager.
    @param noGraphics
        If this is set to true (false is default), then the queue is not removed in lua. This is used to destroy the queue, after the GUIManager has been destroyed.
    */
    void NotificationQueueCEGUI::destroy(bool noGraphics)
    {
        // Remove the NotificationQueue in lua.
        if(this->isRegistered() && GameMode::showsGraphics() && !noGraphics)
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".removeQueue(\"" + this->getName() +  "\")");

        NotificationQueue::destroy();
    }

    /**
    @brief
        Set the size of the window that displays the NotificationQueue.
    @param size
        The size is a vector with components:
        - The relative width of the window. (A value between 0 and 1)
        - The absolute width in pixels. (Additional to the relative width, can be negative)
        - The relative height of the window. (A value between 0 and 1)
        - The absolute height in pixels. (Additional to the relative width, can be negative.)
        If both the 3rd and 4th component of size are set to 0 the height is set such that exactly as many Notifications fit as is the maximum size of the NotificationQueue (in terms of the number of Notifications).
    */
    void NotificationQueueCEGUI::setDisplaySize(const Vector4& size)
    {
        if(this->displaySize_ == size)
            return;

        if(size.x < 0.0 || size.x > 1.0 || size.z < 0.0 || size.z > 1.0)
        {
            orxout(internal_warning, context::notifications) << "The display size of the NotificationQueueCEGUI " << this->getName() << " was trying to be set, but the relative size was not in [0,1]. Aborting..." << endl;
            return;
        }

        this->displaySize_ = size;
        this->displaySizeChanged();
    }

    /**
    @brief
        Is called when the display size has changed.
    */
    void NotificationQueueCEGUI::displaySizeChanged(void)
    {
        if(this->isRegistered() && GameMode::showsGraphics())
        {
            if(this->displaySize_.z == 0.0 && this->displaySize_.w == 0.0)
                GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".resizeQueue(\"" + this->getName() + "\", " + multi_cast<std::string>(this->displaySize_.x) + ", " + multi_cast<std::string>(this->displaySize_.y) + ")");
            else
                GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".resizeQueue(\"" + this->getName() + "\", " + multi_cast<std::string>(this->displaySize_.x) + ", " + multi_cast<std::string>(this->displaySize_.y) + ", " + multi_cast<std::string>(this->displaySize_.z) + ", " + multi_cast<std::string>(this->displaySize_.w) + ")");
        }
    }

    /**
    @brief
        Set the position of the window that displays the NotificationQueue.
    @param position
        The position is a vector with components:
        - The relative x-position of the window. (A value between 0 and 1)
        - The absolute x-position in pixels. (Additional to the relative x-position, can be negative)
        - The relative y-position of the window. (A value between 0 and 1)
        - The absolute y-position in pixels. (Additional to the relative y-position, can be negative.)
    */
    void NotificationQueueCEGUI::setPosition(const Vector4& position)
    {
        if(this->position_ == position)
            return;

        if(position.x < 0.0 || position.x > 1.0 || position.z < 0.0 || position.z > 1.0)
        {
            orxout(internal_warning, context::notifications) << "The position the NotificationQueueCEGUI " << this->getName() << " was trying to be set, but the relative position was not in [0,1]. Aborting..." << endl;
            return;
        }

        this->position_ = position;
        this->positionChanged();
    }

    /**
    @brief
        Is called when the NotificationQueue's position has changed.
    */
    void NotificationQueueCEGUI::positionChanged(void)
    {
        if(this->isRegistered() && GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".moveQueue(\"" + this->getName() + "\", " + multi_cast<std::string>(this->position_.x) + ", " + multi_cast<std::string>(this->position_.y) + ", " + multi_cast<std::string>(this->position_.z) + ", " + multi_cast<std::string>(this->position_.w) + ")");
    }

    /**
    @brief
        Set the horizontal alignment of the Notifications text.
    @param alignment
        The alignment of the Notifications, they are the possible string that the CEGUI Falagard StaticText HorzFormatting property can take.
    @see http://cegui.org.uk/api_reference/classCEGUI_1_1FalagardStaticTextProperties_1_1HorzFormatting.html
    */
    void NotificationQueueCEGUI::setAlignment(const std::string& alignment)
    {
        if(this->alignment_ == alignment)
            return;

        // TODO: Check whether the alignment string is correct?
        this->alignment_ = alignment;
        this->alignmentChanged();
    }

    /**
    @brief
        Is called when the horizontal alignment of the Notifications text has changed.
    */
    void NotificationQueueCEGUI::alignmentChanged(void)
    {
        if(this->isRegistered() && GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".changeQueueAlignment(\"" + this->getName() + "\", \"" + this->alignment_ + "\")");
    }

    /**
    @brief
        Set the font size of the text displayed by this NotificationQueue.
    @param size
        The font size.
    */
    void NotificationQueueCEGUI::setFontSize(unsigned int size)
    {
        if(this->fontSize_ == size)
            return;

        this->fontSize_ = size;
        this->fontSizeChanged();
    }

    /**
    @brief
        Is called when the font size of the text displayed by this NotificationQueue has changed.
    */
    void NotificationQueueCEGUI::fontSizeChanged(void)
    {
        if(this->isRegistered() && GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".changeQueueFontSize(\"" + this->getName() + "\", " + multi_cast<std::string>(this->fontSize_) + ")");
    }

    /**
    @brief
        Set the font color if the text displayed by this NotificationQueue.
    @param color
        The color is a vector with the components being RGBA and taking values from 0 to 1.
    */
    void NotificationQueueCEGUI::setFontColor(const Vector4& color)
    {
        if(this->fontColor_ == color)
            return;

        this->fontColor_ = color;
        this->fontColorChanged();
    }

    /**
    @brief
        Is called when the font color if the text displayed by this NotificationQueue.
    */
    void NotificationQueueCEGUI::fontColorChanged(void)
    {
        // Convert to ARGB format.
        std::stringstream stream;
        for(unsigned int i = 0; i < 4; i++)
            stream << std::hex << std::setw(2) << std::setfill('0') << int(this->fontColor_[(i+3)%4]*255);
        this->fontColorStr_ = stream.str();

        if(this->isRegistered() && GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".changeQueueFontColor(\"" + this->getName() + "\", \"" + this->fontColorStr_ + "\")");
    }

    /**
    @brief
        Get the NotificationQueueCEGUI with the input name.
    @param name
        The name of the NotificationQueueCEGUI to be got.
    @return
        Returns a pointer to the NotificationQueueCEGUI, or NULL if it doesn't exist.
    */
    /*static*/ NotificationQueueCEGUI* NotificationQueueCEGUI::getQueue(const std::string& name)
    {
        NotificationQueue* queue = NotificationManager::getInstance().getQueue(name);
        if(queue == NULL || !queue->isA(Class(NotificationQueueCEGUI)))
            return NULL;
        return static_cast<NotificationQueueCEGUI*>(queue);
    }

    /**
    @brief
        Is called by the NotificationQueue when a Notification was pushed.
    @param notification
        The Notification that was pushed.
    */
    void NotificationQueueCEGUI::notificationPushed(Notification* notification)
    {
         // Push the Notification to the GUI.
        if(this->isRegistered() && GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".pushNotification(\"" + this->getName() + "\", \"" + notification->getMessage() + "\")");
    }

    /**
    @brief
        Is called by the NotificationQueue when a Notification was popped.
    */
    void NotificationQueueCEGUI::notificationPopped(void)
    {
        // Pops the Notification from the GUI.
        if(this->isRegistered() && GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".popNotification(\"" + this->getName() + "\")");
    }

    /**
    @brief Is called when a notification was removed.
    @param index The index the removed Notification was at.
    */
    void NotificationQueueCEGUI::notificationRemoved(unsigned int index)
    {
        // Removes the Notification from the GUI.
        if(this->isRegistered() && GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".removeNotification(\"" + this->getName() + "\", " + multi_cast<std::string>(index) + ")");
    }

    /**
    @brief
        Clears the NotificationQueue by removing all NotificationContainers.
    @param noGraphics
        If this is set to true the GUI is not informed of the clearing of the NotificationQueue. This is needed only internally.
    */
    void NotificationQueueCEGUI::clear(bool noGraphics)
    {
        NotificationQueue::clear(noGraphics);

        // Clear the NotificationQueue in the GUI.
        if(this->isRegistered() && GameMode::showsGraphics() && !noGraphics)
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".clearQueue(\"" + this->getName() + "\")");
    }

    /**
    @brief
        Creates the NotificationQueue in lua.
    */
    void NotificationQueueCEGUI::create(void)
    {
        if(this->isRegistered() && GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".removeQueue(\"" + this->getName() +  "\")");
        
        this->NotificationQueue::create();
        
        if(this->isRegistered() && GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString(NotificationQueueCEGUI::NOTIFICATION_LAYER + ".createQueue(\"" + this->getName() +  "\", " + multi_cast<std::string>(this->getMaxSize()) + ")");
    }

}

