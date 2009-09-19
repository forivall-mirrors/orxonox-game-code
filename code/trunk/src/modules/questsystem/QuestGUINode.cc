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

#include "QuestGUINode.h"

#include <sstream>

#include <CEGUIWindowManager.h>
#include <elements/CEGUIFrameWindow.h>
#include <elements/CEGUIPushButton.h>
#include <falagard/CEGUIFalWidgetLookFeel.h>
#include <falagard/CEGUIFalWidgetLookManager.h>

#include "core/CoreIncludes.h"
#include "Quest.h"
#include "QuestHint.h"
#include "QuestItem.h"
#include "QuestDescription.h"
#include "QuestGUI.h"

namespace orxonox {

    /**
    @brief
        Default Constructor. Registers and initializes the object.
    */
    QuestGUINode::QuestGUINode(void)
    {
        this->initialize();
    }

    /**
    @brief
        Constructor. Registers and initializes the object.
    @param gui
        The QuestGUi the node beongs to.
    @param parent
        The parent node of the newly created node.
    @param item
        The QuestItem the  newly created node is for.
    @param depth
        Parameter to define how much the list item has to be indented.
    @param index
        "Counter" for Quests and Hints.
    */
    QuestGUINode::QuestGUINode(QuestGUI* gui, QuestGUINode* parent, QuestItem* item, int depth, int index)
    {
        this->initialize();    

        this->gui_ = gui;
        this->parent_ = parent;
        this->item_ = item;
        this->depth_ = depth;
        this->index_ = index;

        this->createWindow();

        COUT(3) << "New QuestGUINode '" << this->window_->getName() << "' created." << std::endl;
    }

    /**
    @brief
        Destructor.
    @todo
        Destroying everything?
    */
    QuestGUINode::~QuestGUINode(void)
    {
        if(this->window_ != NULL)
            this->window_->destroy();
        if(this->details_ != NULL)
        {
            this->details_->destroy();
        }
    }

    /**
    @brief
        Initialize the object.
    */
    void QuestGUINode::initialize(void)
    {
        RegisterRootObject(QuestGUINode);

        this->parent_ = NULL;
        this->item_ = NULL;
        this->window_ = NULL;
        this->details_ = NULL;
        this->depth_ = 0;
        this->index_ = 0;
        this->visible_ = true;
    }

    void QuestGUINode::toggleVisibility(void)
    {
        
    }

    /**
    @brief
        Sets the input buffer to the name of the node.
    @param buffer
        The buffer that is set to the name of the node.
    @todo
        Needed?
    */
    void QuestGUINode::getName(std::string & buffer)
    {
        if(this->window_ != NULL)
        {
            buffer = (std::string)(this->window_->getName().c_str());
        }
        else
        {
            buffer = "";
        }
    }

    /**
    @brief
        Creates the details window.
    @return
        Returns the details window.
    @todo
        Return necessary?
    */
    CEGUI::Window* QuestGUINode::getDetails(void)
    {

        if(this->details_ == NULL) //!< If the details window was not already created.
        {  
            std::ostringstream stream;

            //! Create the main window for the details.
            stream << this->window_->getName() << "/Details";
            const QuestDescription* description = this->item_->getDescription();
            this->details_ = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/FrameWindow", stream.str());
            this->details_->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0),CEGUI::UDim(0.8, 0)));
            this->details_->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0),CEGUI::UDim(0.1, 0)));
            this->details_->setText(description->getTitle());
            this->details_->setAlpha(1.0);
            this->details_->setInheritsAlpha(false);
            this->details_->setProperty("CloseButtonEnabled", "True");
            this->details_->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&QuestGUINode::closeDetails, this));

            //! Create a ScrollablePane.
            stream << "/Scrollable";
            CEGUI::Window* window = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/ScrollablePane", stream.str());
            window->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, -10),CEGUI::UDim(1.0, -26)));
            window->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 5),CEGUI::UDim(0, 26)));
            this->details_->addChildWindow(window);

            int height;
            int offset = 0;

            //! Display the status of the QuestItem if it is a Quest.
            Quest* quest = dynamic_cast<Quest*>(this->item_);
            if(quest != NULL) //!< If the QuestItem is a Quest
            {
                stream.str("");
                stream << this->details_->getName() << "/Status";
                CEGUI::Window* statusWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", stream.str());
                window->addChildWindow(statusWindow);
                std::string status = "";
                if(quest->isActive(this->gui_->getPlayer()))
                {
                    status = "This quest is active.";
                }
                else if(quest->isCompleted(this->gui_->getPlayer()))
                {
                    status = "This quest was completed.";
                }
                else if(quest->isFailed(this->gui_->getPlayer()))
                {
                    status = "This quest was failed.";
                }
                statusWindow->setProperty("HorzFormatting", "WordWrapLeftAligned");
                statusWindow->setProperty("VertFormatting", "TopAligned");
                statusWindow->setText(status);
                statusWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, offset)));
                statusWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, -13),CEGUI::UDim(1.0, 0)));
                height = setHeight(statusWindow);

                offset += height;
            }

            //! Display the Description of the QuestItem.
            stream.str("");
            stream << this->details_->getName() << "/Description";
            CEGUI::Window* descriptionWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", stream.str());
            window->addChildWindow(descriptionWindow);
            descriptionWindow->setProperty("HorzFormatting", "WordWrapLeftAligned");
            descriptionWindow->setProperty("VertFormatting", "TopAligned");
            descriptionWindow->setText(description->getDescription());
            descriptionWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, offset)));
            descriptionWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, -13),CEGUI::UDim(1.0, 0)));
            height = setHeight(descriptionWindow);

            offset += height;

            //! Display a list of hints if the QuestItem is a Quest.
            if(quest != NULL)
            {
                for(std::list<QuestGUINode*>::iterator it = this->subNodes_.begin(); it != this->subNodes_.end(); it++)
                {
                    if(dynamic_cast<QuestHint*>((*it)->item_) != NULL) //!< If the subNode belongs to a QuestHint.
                    {
                        QuestGUINode* node = *it;
                        node->window_->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, -13),CEGUI::UDim(0, 30)));
                        node->window_->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, offset)));
                        node->window_->setProperty("HorizontalAlignment", "Left"); // TODO: Get this working.
                        window->addChildWindow(node->window_);
                        offset += 30;
                    }
                }
            }

            COUT(3) << "Show Details: " << this->details_->getName() << std::endl;
        }

        return this->details_;
    }

    /**
    @brief
        Opens the details window for the Quest/QuestHint clicked on.
    */
    bool QuestGUINode::openDetails(const CEGUI::EventArgs& e)
    {
        COUT(3) << "Open QuestItem..." << std::endl;
        
        CEGUI::Window* window = this->gui_->getRootWindow();

        if(window != NULL)
            window->addChildWindow(this->getDetails());

        return true;
    }

    /**
    @brief
        Close the details window.
    */
    bool QuestGUINode::closeDetails(const CEGUI::EventArgs& e)
    {        
        CEGUI::Window* questsList = this->gui_->getRootWindow();
        questsList->removeChildWindow(this->details_);

        return true;
    }

    /*static*/ CEGUI::Rect QuestGUINode::getStaticTextArea(const CEGUI::Window* window)
    {
        const CEGUI::WidgetLookFeel& lookAndFeel = CEGUI::WidgetLookManager::getSingleton().getWidgetLook(window->getLookNFeel());

        return lookAndFeel.getNamedArea("WithFrameTextRenderArea").getArea().getPixelRect(*window);
    }

    /*static*/ int QuestGUINode::setHeight(CEGUI::Window* window)
    {
        //! Get the area the text is formatted and drawn into.
        const CEGUI::Rect formattedArea = getStaticTextArea(window);
        COUT(1) << "PixelRect before setHeight(): " << formattedArea.getHeight() << "x" << formattedArea.getWidth() << std::endl; //Debug

        //! Calculate the pixel height of the frame by subtracting the height of the area above from the total height of the window.
        const float frameHeight = window->getUnclippedPixelRect().getHeight() - formattedArea.getHeight();

        //! Get the formatted line count - using the formatting area obtained above.
        const float lines = window->getFont()->getFormattedLineCount(window->getText(), formattedArea, CEGUI::WordWrapLeftAligned);
        COUT(1) << "Lines: " << lines << std::endl; //Debug

        //! Calculate pixel height of window, which is the number of formatted lines multiplied by the spacing of the font, plus the pixel height of the frame.
        const float height = lines * window->getFont()->getLineSpacing() + frameHeight;

        //! Set the height to the window.
        window->setHeight(CEGUI::UDim(0, height));
        
        //Debug
        const CEGUI::Rect newArea = getStaticTextArea(window);
        COUT(1) << "PixelRect after setHeight(): " << newArea.getHeight() << "x" << newArea.getWidth() << std::endl; //Debug

        return static_cast<int>(height);
    }

    //int QuestGUINode::setHeight(CEGUI::Window* window)
    //{
    //    COUT(1) << "PixelRect before setHeight(): " << window->getPixelRect().getHeight() << "x" << window->getPixelRect().getWidth() << std::endl; //Debug
    //    int lines = window->getFont()->getFormattedLineCount(window->getText(), window->getPixelRect(), CEGUI::WordWrapLeftAligned);
    //    int height = 2*lines*window->getFont()->getLineSpacing();
    //    COUT(1) << "Lines: " << lines << ", LineSpacing: " << window->getFont()->getLineSpacing() << std::endl; //Debug
    //    window->setHeight(CEGUI::UDim(0, height));
    //    COUT(1) << "PixelRect after setHeight(): " << window->getPixelRect().getHeight() << "x" << window->getPixelRect().getWidth() << std::endl; //Debug
    //    return height;
    //}

    /**
    @brief
        Update the position list item.
    */
    void QuestGUINode::updatePosition(void)
    {
        this->window_->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 20*this->depth_),CEGUI::UDim(0, 30*this->index_)));
        this->window_->setSize(CEGUI::UVector2(CEGUI::UDim(1, -20*this->depth_-13),CEGUI::UDim(0, 30)));
    }

    /**
    @brief
        Helper method to create the CEGUI Window the node.
    */
    void QuestGUINode::createWindow(void)
    {
        Quest* quest = dynamic_cast<Quest*>(this->item_);
        
        this->window_ = this->gui_->getWindow();
        std::ostringstream stream;
        stream << "QuestGUI/Quests/";
        if(quest == NULL)
        {
            stream << this->parent_->index_ << "/Hints/";
        }
        stream << this->index_;
        
        this->window_->rename(stream.str());
        this->window_->setText(this->item_->getDescription()->getTitle());

        this->parent_->subNodes_.push_back(this);

        if(dynamic_cast<Quest*>(this->item_) != NULL)
        {
            this->gui_->getRootWindow()->addChildWindow(this->window_);
            this->updatePosition();
        }
        else
        {
            this->window_->setDestroyedByParent(false);
        }

        this->window_->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&QuestGUINode::openDetails, this));
    }

}

