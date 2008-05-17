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
 *      Felix Schulthess
 *   Co-authors:
 *      Fabian 'x3n' Landau
 *
 */

#include "OrxonoxStableHeaders.h"

#include "InGameConsole.h"

#include <string>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreStringConverter.h>

#include "core/Debug.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "GraphicsEngine.h"

#define LINES 20

namespace orxonox
{
    using namespace Ogre;

    float InGameConsole::REL_WIDTH = 0.8;
    float InGameConsole::REL_HEIGHT = 0.4;
    float InGameConsole::BLINK = 0.25;

    /**
        @brief Constructor: Creates and initializes the InGameConsole.
    */
    InGameConsole::InGameConsole()
    {
        RegisterObject(InGameConsole);

        this->active_ = false;
        this->cursor_ = 0.0;

        this->init();
        this->setConfigValues();
    }

    /**
        @brief Destructor: Destroys the TextAreas.
    */
    InGameConsole::~InGameConsole(void)
    {
        for (int i = 0; i < LINES; i++)
            delete this->consoleOverlayTextAreas_[i];

        delete this->consoleOverlayTextAreas_;
    }

    /**
        @brief Returns a reference to the only existing instance of InGameConsole.
    */
    InGameConsole& InGameConsole::getInstance()
    {
        static InGameConsole instance;
        return instance;
    }

    /**
        @brief Sets the config values, describing the size of the console.
    */
    void InGameConsole::setConfigValues()
    {
        SetConfigValue(REL_WIDTH, 0.8);
        SetConfigValue(REL_HEIGHT, 0.4);
        SetConfigValue(BLINK, 0.25);
    }

    /**
        @brief Called if all output-lines have to be redrawn.
    */
    void InGameConsole::linesChanged()
    {
        std::list<std::string>::const_iterator it = Shell::getInstance().getNewestLineIterator();
        for (int i = 1; i < LINES && it != Shell::getInstance().getEndIterator(); i++)
        {
            this->consoleOverlayTextAreas_[i]->setCaption(*it);
            ++it;
        }
    }

    /**
        @brief Called if only the last output-line has changed.
    */
    void InGameConsole::onlyLastLineChanged()
    {
        if (LINES > 1)
            this->consoleOverlayTextAreas_[1]->setCaption(*Shell::getInstance().getNewestLineIterator());
    }

    /**
        @brief Called if a new output-line was added.
    */
    void InGameConsole::lineAdded()
    {
        for (int i = LINES - 1; i > 1; i--)
            this->consoleOverlayTextAreas_[i]->setCaption(this->consoleOverlayTextAreas_[i - 1]->getCaption());

        if (LINES > 1)
            this->consoleOverlayTextAreas_[1]->setCaption(*Shell::getInstance().getNewestLineIterator());
    }

    /**
        @brief Called if the text in the input-line has changed.
    */
    void InGameConsole::inputChanged()
    {
        if (LINES > 0)
            this->consoleOverlayTextAreas_[0]->setCaption(Shell::getInstance().getInput());
    }

    /**
        @brief Called if the position of the cursor in the input-line has changed.
    */
    void InGameConsole::cursorChanged()
    {
        std::string input = Shell::getInstance().getInput();
        input.insert(Shell::getInstance().getCursorPosition(), 1, '|');
        if (LINES > 0)
            this->consoleOverlayTextAreas_[0]->setCaption(input);
    }

    /**
        @brief Called if the console gets closed.
    */
    void InGameConsole::exit()
    {
        this->deactivate();
        CommandExecutor::execute("set InputMode 2");
    }

    /**
        @brief Called once by constructor, initializes the InGameConsole.
    */
    void InGameConsole::init()
    {
        // for the beginning, don't scroll
        this->scroll_ = 0;
        this->scrollTimer_ = 0;
        this->cursor_ = 0;

        // create overlay and elements
        this->om_ = &Ogre::OverlayManager::getSingleton();

        // create a container
        this->consoleOverlayContainer_ = static_cast<OverlayContainer*>(this->om_->createOverlayElement("Panel", "container"));
        this->consoleOverlayContainer_->setMetricsMode(Ogre::GMM_RELATIVE);
        this->consoleOverlayContainer_->setPosition((1 - InGameConsole::REL_WIDTH) / 2, 0);
        this->consoleOverlayContainer_->setDimensions(InGameConsole::REL_WIDTH, InGameConsole::REL_HEIGHT);

        // create BorderPanel
        this->consoleOverlayBorder_ = static_cast<BorderPanelOverlayElement*>(this->om_->createOverlayElement("BorderPanel", "borderPanel"));
        this->consoleOverlayBorder_->setMetricsMode(Ogre::GMM_PIXELS);
        this->consoleOverlayBorder_->setMaterialName("ConsoleCenter");
        // set parameters for border
        this->consoleOverlayBorder_->setBorderSize(16, 16, 0, 16);
        this->consoleOverlayBorder_->setBorderMaterialName("ConsoleBorder");
        this->consoleOverlayBorder_->setLeftBorderUV(0.0, 0.49, 0.5, 0.51);
        this->consoleOverlayBorder_->setRightBorderUV(0.5, 0.49, 1.0, 0.5);
        this->consoleOverlayBorder_->setBottomBorderUV(0.49, 0.5, 0.51, 1.0);
        this->consoleOverlayBorder_->setBottomLeftBorderUV(0.0, 0.5, 0.5, 1.0);
        this->consoleOverlayBorder_->setBottomRightBorderUV(0.5, 0.5, 1.0, 1.0);

        // create the text lines
        this->consoleOverlayTextAreas_ = new TextAreaOverlayElement*[LINES];
        for (int i = 0; i < LINES; i++)
        {
            this->consoleOverlayTextAreas_[i] = static_cast<TextAreaOverlayElement*>(this->om_->createOverlayElement("TextArea", "textArea" + Ogre::StringConverter::toString(i)));
            this->consoleOverlayTextAreas_[i]->setMetricsMode(Ogre::GMM_PIXELS);
            this->consoleOverlayTextAreas_[i]->setFontName("Console");
            this->consoleOverlayTextAreas_[i]->setCharHeight(20);
            this->consoleOverlayTextAreas_[i]->setParameter("colour_top", "0.21 0.69 0.21");
            this->consoleOverlayTextAreas_[i]->setLeft(8);
            this->consoleOverlayTextAreas_[i]->setCaption("");
        }

        // create noise
        this->consoleOverlayNoise_ = static_cast<PanelOverlayElement*>(this->om_->createOverlayElement("Panel", "noise"));
        this->consoleOverlayNoise_->setMetricsMode(Ogre::GMM_PIXELS);
        this->consoleOverlayNoise_->setPosition(5,0);
        this->consoleOverlayNoise_->setMaterialName("ConsoleNoise");

        this->consoleOverlay_ = this->om_->create("Console");
        this->consoleOverlay_->add2D(this->consoleOverlayContainer_);
        this->consoleOverlayContainer_->addChild(this->consoleOverlayBorder_);
        //comment following line to disable noise
        this->consoleOverlayContainer_->addChild(this->consoleOverlayNoise_);
        for (int i = 0; i < LINES; i++)
            this->consoleOverlayContainer_->addChild(this->consoleOverlayTextAreas_[i]);

        this->resize();

        // move overlay "above" the top edge of the screen
        // we take -1.2 because the border mkes the panel bigger
        this->consoleOverlayContainer_->setTop(-1.2 * InGameConsole::REL_HEIGHT);
        // show overlay
        this->consoleOverlay_->show();

        COUT(3) << "Info: InGameConsole initialized" << std::endl;
    }

    /**
        @brief Used to control the actual scrolling and the cursor.
    */
    void InGameConsole::tick(float dt)
    {
        this->scrollTimer_ += dt;
        if (this->scrollTimer_ >= 0.01)
        {
            float top = this->consoleOverlayContainer_->getTop();
            this->scrollTimer_ = 0;
            if (this->scroll_ != 0)
            {
                // scroll
                top = top + 0.02 * this->scroll_;
                this->consoleOverlayContainer_->setTop(top);
            }
            if (top <= -1.2 * InGameConsole::REL_HEIGHT)
            {
                // window has completely scrolled up
                this->scroll_ = 0;
                this->consoleOverlay_->hide();
                this->active_ = false;
            }
            if (top >= 0)
            {
                // window has completely scrolled down
                this->scroll_ = 0;
                this->consoleOverlayContainer_->setTop(0);
                this->active_ = true;
            }
        }

        this->cursor_ += dt;
        if (this->cursor_ >= 2 * InGameConsole::BLINK)
            this->cursor_ = 0;
//        print(convert2UTF(this->ib_->get()));

        // this creates a flickering effect
        this->consoleOverlayNoise_->setTiling(1, rand() % 5 + 1);
    }

    /**
        @brief Resizes the console elements. Call if window size changes.
    */
    void InGameConsole::resize()
    {
        this->windowW_ = GraphicsEngine::getSingleton().getWindowWidth();
        this->windowH_ = GraphicsEngine::getSingleton().getWindowHeight();
        this->consoleOverlayBorder_->setWidth((int) this->windowW_* InGameConsole::REL_WIDTH);
        this->consoleOverlayBorder_->setHeight((int) this->windowH_ * InGameConsole::REL_HEIGHT);
        this->consoleOverlayNoise_->setWidth((int) this->windowW_ * InGameConsole::REL_WIDTH - 10);
        this->consoleOverlayNoise_->setHeight((int) this->windowH_ * InGameConsole::REL_HEIGHT - 5);
        // now adjust the text lines...
        for (int i = 0; i < LINES; i++)
        {
            this->consoleOverlayTextAreas_[i]->setWidth((int) this->windowW_ * InGameConsole::REL_WIDTH);
            this->consoleOverlayTextAreas_[i]->setTop((int) this->windowH_ * InGameConsole::REL_HEIGHT - 24 - 16*i);
        }
    }

    /**
        @brief Shows the InGameConsole.
    */
    void InGameConsole::activate()
    {
        this->consoleOverlay_->show();
        // just in case window size has changed...
        this->resize();
        // scroll down
        this->scroll_ = 1;
        // the rest is done by tick
    }

    /**
    @brief Hides the InGameConsole.
    */
    void InGameConsole::deactivate()
    {
        // scroll up
        this->scroll_ = -1;
        // the rest is done by tick
    }

    /**
        @brief Activates the console.
    */
    void InGameConsole::openConsole()
    {
        InGameConsole::getInstance().activate();
    }

    /**
        @brief Deactivates the console.
    */
    void InGameConsole::closeConsole()
    {
        InGameConsole::getInstance().deactivate();
    }

    /**
        @brief Prints string to bottom line.
        @param s String to be printed
    */
    void InGameConsole::print(Ogre::UTFString s)
    {
        if (this->cursor_ > InGameConsole::BLINK)
            this->consoleOverlayTextAreas_[0]->setCaption(">" + s);
        else
            this->consoleOverlayTextAreas_[0]->setCaption(">" + s + "_");
    }

    /**
        @brief Shifts all lines up and clears the bottom line.
    */
    void InGameConsole::newline()
    {
        Ogre::UTFString line;
        for (int i = LINES - 1; i >= 1; i--)
        {
            line = this->consoleOverlayTextAreas_[i - 1]->getCaption();
            // don't copy the cursor...
            int l = line.length();
            if (!line.empty() && line.substr(l-1) == "_")
                line.erase(l-1);
            this->consoleOverlayTextAreas_[i]->setCaption(line);
        }
        this->consoleOverlayTextAreas_[0]->setCaption(">");
    }

    /**
        @brief Converts a string into an Ogre::UTFString.
        @param s The string to convert
        @return The converted string
    */
    Ogre::UTFString InGameConsole::convert2UTF(std::string s)
    {
        Ogre::UTFString utf;
        Ogre::UTFString::code_point cp;
        for (unsigned int i = 0; i < s.size(); ++i)
        {
          cp = s[i];
          cp &= 0xFF;
          utf.append(1, cp);
        }
        return utf;
    }
}
