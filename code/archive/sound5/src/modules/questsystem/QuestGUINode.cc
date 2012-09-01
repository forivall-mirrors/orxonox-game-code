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
            buffer = this->window_->getName().c_str();
        }
        else
        {
            buffer.erase();
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
            this->details_ = this->gui_->getWindowManager()->createWindow("TaharezLook/FrameWindow", stream.str());
            this->details_->setSize(CEGUI::UVector2(CEGUI::UDim(0.7f, 0),CEGUI::UDim(0.7f, 0)));
            this->details_->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f, 0),CEGUI::UDim(0.1f, 0)));
            this->details_->setText(description->getTitle());
            this->details_->setAlpha(1.0);
            this->details_->setInheritsAlpha(false);
            this->details_->setProperty("CloseButtonEnabled", "True");
            this->details_->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&QuestGUINode::closeDetails, this));

            //! Create a ScrollablePane.
            stream << "/Scrollable";
            CEGUI::Window* window = this->gui_->getWindowManager()->createWindow("TaharezLook/ScrollablePane", stream.str());
            window->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, -2*QuestGUINode::BORDER_WIDTH),CEGUI::UDim(1.0, -QuestGUINode::TITLE_HEIGHT)));
            window->setPosition(CEGUI::UVector2(CEGUI::UDim(0, (float)QuestGUINode::BORDER_WIDTH),CEGUI::UDim(0, (float)QuestGUINode::TITLE_HEIGHT)));
            this->details_->addChildWindow(window);

            int height;
            int offset = 0;

            //! Display the status of the QuestItem if it is a Quest.
            Quest* quest = dynamic_cast<Quest*>(this->item_);
            if(quest != NULL) //!< If the QuestItem is a Quest
            {
                stream.str("");
                stream << this->details_->getName() << "/Status";
                CEGUI::Window* statusWindow = this->gui_->getWindowManager()->createWindow("TaharezLook/StaticText", stream.str());
                window->addChildWindow(statusWindow);
                std::string status;
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
                statusWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, (float)offset)));
                statusWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, (float)-QuestGUINode::SCROLLBAR_WIDTH),CEGUI::UDim(1.0f, 0)));
                height = setHeight(statusWindow);

                offset += height;
            }

            //! Create title pane for the description.
            stream.str("");
            stream << this->details_->getName() << "/Description";
            stream << "/Title";
            CEGUI::Window* descriptionWindowTitle = this->gui_->getWindowManager()->createWindow("TaharezLook/StaticText", stream.str());
            window->addChildWindow(descriptionWindowTitle);
            descriptionWindowTitle->setProperty("HorzFormatting", "HorzCentred");
            descriptionWindowTitle->setProperty("VertFormatting", "TopAligned");
            descriptionWindowTitle->setText("Description:");
            descriptionWindowTitle->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, (float)offset)));
            descriptionWindowTitle->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f, -QuestGUINode::SCROLLBAR_WIDTH),CEGUI::UDim(1.0f, 0)));

            offset += setHeight(descriptionWindowTitle);

            //! Display the Description of the QuestItem.
            stream.str("");
            stream << this->details_->getName() << "/Description";
            CEGUI::Window* descriptionWindow = this->gui_->getWindowManager()->createWindow("TaharezLook/StaticText", stream.str());
            window->addChildWindow(descriptionWindow);
            descriptionWindow->setProperty("HorzFormatting", "WordWrapLeftAligned");
            descriptionWindow->setProperty("VertFormatting", "TopAligned");
            descriptionWindow->setText(description->getDescription());
            descriptionWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, (float)offset)));
            descriptionWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, (float)-QuestGUINode::SCROLLBAR_WIDTH),CEGUI::UDim(1.0f, 0)));
            height = setHeight(descriptionWindow);

            offset += height;

            //! Display a list of hints if the QuestItem is a Quest.
            bool title = true;
            if(quest != NULL)
            {
                for(std::list<QuestGUINode*>::iterator it = this->subNodes_.begin(); it != this->subNodes_.end(); it++)
                {
                    if(dynamic_cast<QuestHint*>((*it)->item_) != NULL) //!< If the subNode belongs to a QuestHint.
                    {
                        if(title) //!< If no title pane for the QuestHints has been created, create one.
                        {
                            stream.str("");
                            stream << this->details_->getName() << "/Hints/Title";
                            CEGUI::Window* hintsTitle = this->gui_->getWindowManager()->createWindow("TaharezLook/StaticText", stream.str());
                            window->addChildWindow(hintsTitle);
                            hintsTitle->setProperty("HorzFormatting", "HorzCentred");
                            hintsTitle->setProperty("VertFormatting", "TopAligned");
                            hintsTitle->setText("Hints:");
                            hintsTitle->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, (float)offset)));
                            hintsTitle->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, -QuestGUINode::SCROLLBAR_WIDTH),CEGUI::UDim(1.0, 0)));
                            offset += setHeight(hintsTitle);;
                            title = false;
                        }
                        QuestGUINode* node = *it;
                        node->window_->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f, (float)-QuestGUINode::SCROLLBAR_WIDTH),CEGUI::UDim(0, (float)QuestGUINode::BUTTON_HEIGHT)));
                        node->window_->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, (float)offset)));
                        window->addChildWindow(node->window_);
                        offset += QuestGUINode::BUTTON_HEIGHT;
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

        //CEGUI::Window* window = this->gui_->getRootWindow();
        CEGUI::Window* window = this->gui_->getWindowManager()->getWindow("orxonox/QuestGUI/Background");

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
        //CEGUI::Window* window = this->gui_->getRootWindow();
        CEGUI::Window* window = this->gui_->getWindowManager()->getWindow("orxonox/QuestGUI/Background");
        window->removeChildWindow(this->details_);

        return true;
    }

    /**
    @brief
        Helper method for setHeight(). Gets the StaticTextArea for an input CEGUI Window.
    @param window
        The CEGUI window.
    @return
        Returns a CEGUI Rect.
    */
    /*static*/ CEGUI::Rect QuestGUINode::getStaticTextArea(const CEGUI::Window* window)
    {
        const CEGUI::WidgetLookFeel& lookAndFeel = CEGUI::WidgetLookManager::getSingleton().getWidgetLook(window->getLookNFeel());

        return lookAndFeel.getNamedArea("WithFrameTextRenderArea").getArea().getPixelRect(*window);
    }

    /**
    @brief
        Helper method to adjust the height of an input Window (of type StaticText) to the text it holds.
    @param window
        The  CEGUI Window (of type StaticText) for which the height is to be adjusted to the text.
    @return
        Returns the set height.
    */
    /*static*/ int QuestGUINode::setHeight(CEGUI::Window* window)
    {
        //! Get the area the text is formatted and drawn into.
        const CEGUI::Rect formattedArea = getStaticTextArea(window);

        //! Calculate the pixel height of the frame by subtracting the height of the area above from the total height of the window.
        const float frameHeight = window->getUnclippedPixelRect().getHeight() - formattedArea.getHeight();

        //! Get the formatted line count - using the formatting area obtained above.
        const float lines = (float)(window->getFont()->getFormattedLineCount(window->getText(), formattedArea, CEGUI::WordWrapLeftAligned));

        //! Calculate pixel height of window, which is the number of formatted lines multiplied by the spacing of the font, plus the pixel height of the frame.
        const float height = lines * window->getFont()->getLineSpacing() + frameHeight;

        //! Set the height to the window.
        window->setHeight(CEGUI::UDim(0, height));

        //Debug
        const CEGUI::Rect newArea = getStaticTextArea(window);

        return static_cast<int>(height);
    }

    /**
    @brief
        Update the position list item.
    */
    void QuestGUINode::updatePosition(void)
    {
        this->window_->setPosition(CEGUI::UVector2(CEGUI::UDim(0, (float)(QuestGUINode::INDENT_WIDTH*this->depth_)),CEGUI::UDim(0, (float)(QuestGUINode::BUTTON_HEIGHT*this->index_))));
        this->window_->setSize(CEGUI::UVector2(CEGUI::UDim(1, (float)(-QuestGUINode::INDENT_WIDTH*this->depth_-QuestGUINode::SCROLLBAR_WIDTH)),CEGUI::UDim(0, (float)QuestGUINode::BUTTON_HEIGHT)));
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

