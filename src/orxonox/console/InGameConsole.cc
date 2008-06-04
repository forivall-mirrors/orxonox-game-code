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
#include "core/input/InputManager.h"
#include "util/Math.h"
#include "GraphicsEngine.h"

#define LINES 30
#define CHAR_WIDTH1 7.78 //34 // fix this please - determine the char-width dynamically
#define CHAR_WIDTH2 8.28 // fix this please - determine the char-width dynamically
#define CHAR_WIDTH3 7.80 //78 // fix this please - determine the char-width dynamically

namespace orxonox
{
    SetConsoleCommand(InGameConsole, openConsole, true);
    SetConsoleCommand(InGameConsole, closeConsole, true);

    using namespace Ogre;

    float InGameConsole::REL_WIDTH = 0.8;
    float InGameConsole::REL_HEIGHT = 0.4;
    float InGameConsole::BLINK = 0.5;

    /**
        @brief Constructor: Creates and initializes the InGameConsole.
    */
    InGameConsole::InGameConsole() :
        om_(0), consoleOverlay_(0), consoleOverlayContainer_(0),
        consoleOverlayNoise_(0), consoleOverlayBorder_(0), consoleOverlayTextAreas_(0)
    {
        RegisterObject(InGameConsole);

        this->active_ = false;
        this->cursor_ = 0.0;
        this->cursorSymbol_ = '|';
        this->inputWindowStart_ = 0;
        this->numLinesShifted_ = LINES - 1;

        this->init();
        this->setConfigValues();

        Shell::getInstance().addOutputLevel(true);
    }

    /**
        @brief Destructor: Destroys the TextAreas.
    */
    InGameConsole::~InGameConsole(void)
    {
        /*for (int i = 0; i < LINES; i++)
            if (this->consoleOverlayTextAreas_[i])
                om_->destroyOverlayElement(this->consoleOverlayTextAreas_[i]);

        if (this->consoleOverlayTextAreas_)
            delete[] this->consoleOverlayTextAreas_;*/
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
        SetConfigValue(BLINK, 0.5);
    }

    /**
        @brief Called if all output-lines have to be redrawn.
    */
    void InGameConsole::linesChanged()
    {
        std::list<std::string>::const_iterator it = Shell::getInstance().getNewestLineIterator();
        int max = 0;
        for (int i = 1; i < LINES; ++i)
        {
            if (it != Shell::getInstance().getEndIterator())
            {
                ++it;
                max = i;
            }
            else
                break;
        }

        for (int i = LINES - 1; i > max; --i)
            this->print("", i, true);

        for (int i = max; i >= 1; --i)
        {
            --it;
            this->print(*it, i, true);
        }
    }

    /**
        @brief Called if only the last output-line has changed.
    */
    void InGameConsole::onlyLastLineChanged()
    {
        if (LINES > 1)
            this->print(*Shell::getInstance().getNewestLineIterator(), 1);
    }

    /**
        @brief Called if a new output-line was added.
    */
    void InGameConsole::lineAdded()
    {
        this->numLinesShifted_ = 0;
        this->shiftLines();
        this->onlyLastLineChanged();
    }

    /**
        @brief Called if the text in the input-line has changed.
    */
    void InGameConsole::inputChanged()
    {
        if (LINES > 0)
            this->print(Shell::getInstance().getInput(), 0);

        if (Shell::getInstance().getInput() == "" || Shell::getInstance().getInput().size() == 0)
            this->inputWindowStart_ = 0;
    }

    /**
        @brief Called if the position of the cursor in the input-line has changed.
    */
    void InGameConsole::cursorChanged()
    {
        /*std::string input = Shell::getInstance().getInput();
        input.insert(Shell::getInstance().getCursorPosition(), 1, this->cursorSymbol_);
        if (LINES > 0)
            this->print(input, 0);*/
        this->setCursorPosition(Shell::getInstance().getCursorPosition() - inputWindowStart_);
    }

    /**
        @brief Called if the console gets closed.
    */
    void InGameConsole::exit()
    {
        this->deactivate();
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
        this->consoleOverlayContainer_ = static_cast<OverlayContainer*>(this->om_->createOverlayElement("Panel", "InGameConsoleContainer"));
        this->consoleOverlayContainer_->setMetricsMode(Ogre::GMM_RELATIVE);
        this->consoleOverlayContainer_->setPosition((1 - InGameConsole::REL_WIDTH) / 2, 0);
        this->consoleOverlayContainer_->setDimensions(InGameConsole::REL_WIDTH, InGameConsole::REL_HEIGHT);

        // create BorderPanel
        this->consoleOverlayBorder_ = static_cast<BorderPanelOverlayElement*>(this->om_->createOverlayElement("BorderPanel", "InGameConsoleBorderPanel"));
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
            this->consoleOverlayTextAreas_[i] = static_cast<TextAreaOverlayElement*>(this->om_->createOverlayElement("TextArea", "InGameConsoleTextArea" + Ogre::StringConverter::toString(i)));
            this->consoleOverlayTextAreas_[i]->setMetricsMode(Ogre::GMM_PIXELS);
            this->consoleOverlayTextAreas_[i]->setFontName("Console");
            this->consoleOverlayTextAreas_[i]->setCharHeight(18);
            this->consoleOverlayTextAreas_[i]->setParameter("colour_top", "0.21 0.69 0.21");
            this->consoleOverlayTextAreas_[i]->setLeft(8);
            this->consoleOverlayTextAreas_[i]->setCaption("");
        }

        // create noise
        this->consoleOverlayNoise_ = static_cast<PanelOverlayElement*>(this->om_->createOverlayElement("Panel", "InGameConsoleNoise"));
        this->consoleOverlayNoise_->setMetricsMode(Ogre::GMM_PIXELS);
        this->consoleOverlayNoise_->setPosition(5,0);
        this->consoleOverlayNoise_->setMaterialName("ConsoleNoise");

        // create cursor
        this->consoleOverlayCursor_ = static_cast<PanelOverlayElement*>(this->om_->createOverlayElement("Panel", "InGameConsoleCursor"));
        this->consoleOverlayCursor_->setMetricsMode(Ogre::GMM_PIXELS);
        this->consoleOverlayCursor_->setPosition(5,219);
        this->consoleOverlayCursor_->setDimensions(1, 14);
        this->consoleOverlayCursor_->setMaterialName("Orxonox/GreenDot");

        this->consoleOverlay_ = this->om_->create("InGameConsoleConsole");
        this->consoleOverlay_->add2D(this->consoleOverlayContainer_);
        this->consoleOverlayContainer_->addChild(this->consoleOverlayBorder_);
        this->consoleOverlayContainer_->addChild(this->consoleOverlayCursor_);
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

        COUT(4) << "Info: InGameConsole initialized" << std::endl;
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
        this->desiredTextWidth_ = (int) (this->windowW_ * InGameConsole::REL_WIDTH) - 12;

        if (LINES > 0)
            this->maxCharsPerLine_ = max((unsigned int)10, (unsigned int) ((float)this->desiredTextWidth_ / CHAR_WIDTH3));
        else
            this->maxCharsPerLine_ = 10;

        for (int i = 0; i < LINES; i++)
        {
            this->consoleOverlayTextAreas_[i]->setWidth(this->desiredTextWidth_);
            this->consoleOverlayTextAreas_[i]->setTop((int) this->windowH_ * InGameConsole::REL_HEIGHT - 24 - 14*i);
        }

        this->linesChanged();
        this->cursorChanged();
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
            float timePassed = scrollTimer_;
            this->scrollTimer_ = 0;
            if (this->scroll_ != 0)
            {
                // scroll
                top = top + timePassed * this->scroll_;
                this->consoleOverlayContainer_->setTop(top);
            }
            if (top <= -1.2 * InGameConsole::REL_HEIGHT)
            {
                // window has completely scrolled up
                this->scroll_ = 0;
                this->consoleOverlay_->hide();
                this->active_ = false;
                Shell::getInstance().unregisterListener(this);
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
        if (this->cursor_ >= InGameConsole::BLINK)
        {
            this->cursor_ = 0;
            bShowCursor_ = !bShowCursor_;
            if (bShowCursor_)
              this->consoleOverlayCursor_->show();
            else
              this->consoleOverlayCursor_->hide();
        }

        /*if (this->cursor_ >= 2 * InGameConsole::BLINK)
          this->cursor_ = 0;

        if (this->cursor_ >= InGameConsole::BLINK && this->cursorSymbol_ == '|')
        {
            this->cursorSymbol_ = ' ';
            this->cursorChanged();
        }
        else if (this->cursor_ < InGameConsole::BLINK && this->cursorSymbol_ == ' ')
        {
            this->cursorSymbol_ = '|';
            this->cursorChanged();
        }*/

        // this creates a flickering effect
        this->consoleOverlayNoise_->setTiling(1, rand() % 5 + 1);
    }

    /**
        @brief Shows the InGameConsole.
    */
    void InGameConsole::activate()
    {
        InputManager::setInputState(InputManager::IS_CONSOLE);
        Shell::getInstance().registerListener(this);
        this->linesChanged();
        this->cursorChanged();

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
        InputManager::setInputState(InputManager::IS_NORMAL);
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
        @brief Shifts all output lines one line up
    */
    void InGameConsole::shiftLines()
    {
        for (unsigned int i = LINES - 1; i > 1; --i)
        {
            this->consoleOverlayTextAreas_[i]->setCaption(this->consoleOverlayTextAreas_[i - 1]->getCaption());
            this->consoleOverlayTextAreas_[i]->setColourTop(this->consoleOverlayTextAreas_[i - 1]->getColourTop());
            this->consoleOverlayTextAreas_[i]->setColourBottom(this->consoleOverlayTextAreas_[i - 1]->getColourBottom());
        }
    }

    void InGameConsole::colourLine(int colourcode, int index)
    {
        if (colourcode == -1)
        {
            this->consoleOverlayTextAreas_[index]->setColourTop   (ColourValue(0.90, 0.90, 0.90, 1.00));
            this->consoleOverlayTextAreas_[index]->setColourBottom(ColourValue(1.00, 1.00, 1.00, 1.00));
        }
        else if (colourcode == 1)
        {
            this->consoleOverlayTextAreas_[index]->setColourTop   (ColourValue(0.95, 0.25, 0.25, 1.00));
            this->consoleOverlayTextAreas_[index]->setColourBottom(ColourValue(1.00, 0.50, 0.50, 1.00));
        }
        else if (colourcode == 2)
        {
            this->consoleOverlayTextAreas_[index]->setColourTop   (ColourValue(0.95, 0.50, 0.20, 1.00));
            this->consoleOverlayTextAreas_[index]->setColourBottom(ColourValue(1.00, 0.70, 0.50, 1.00));
        }
        else if (colourcode == 3)
        {
            this->consoleOverlayTextAreas_[index]->setColourTop   (ColourValue(0.50, 0.50, 0.95, 1.00));
            this->consoleOverlayTextAreas_[index]->setColourBottom(ColourValue(0.80, 0.80, 1.00, 1.00));
        }
        else if (colourcode == 4)
        {
            this->consoleOverlayTextAreas_[index]->setColourTop   (ColourValue(0.65, 0.48, 0.44, 1.00));
            this->consoleOverlayTextAreas_[index]->setColourBottom(ColourValue(1.00, 0.90, 0.90, 1.00));
        }
        else if (colourcode == 5)
        {
            this->consoleOverlayTextAreas_[index]->setColourTop   (ColourValue(0.40, 0.20, 0.40, 1.00));
            this->consoleOverlayTextAreas_[index]->setColourBottom(ColourValue(0.80, 0.60, 0.80, 1.00));
        }
        else
        {
            this->consoleOverlayTextAreas_[index]->setColourTop   (ColourValue(0.21, 0.69, 0.21, 1.00));
            this->consoleOverlayTextAreas_[index]->setColourBottom(ColourValue(0.80, 1.00, 0.80, 1.00));
        }
    }

    void InGameConsole::setCursorPosition(unsigned int pos)
    {
        static std::string char1 = "bdefgilpqtzCEGIJKNOPQT5[}дь";
        static std::string char2 = "Z4";

        if (pos > maxCharsPerLine_)
          pos = maxCharsPerLine_;
        else if (pos < 0)
          pos = 0;

        float width = 0;
        for (unsigned int i = 0; i < pos && i < displayedText_.size(); ++i)
        {
            if (char1.find(displayedText_[i]) != std::string::npos)
                width += CHAR_WIDTH1;
            else if (char2.find(displayedText_[i]) != std::string::npos)
                width += CHAR_WIDTH2;
            else
                width += CHAR_WIDTH3;
        }
        this->consoleOverlayCursor_->setPosition(width + 5, this->windowH_ * InGameConsole::REL_HEIGHT - 20);
    }

    /**
        @brief Prints string to bottom line.
        @param s String to be printed
    */
    void InGameConsole::print(const std::string& text, int index, bool alwaysShift)
    {
        char level = 0;
        if (text.size() > 0)
            level = text[0];

        std::string output = text;

        if (level >= -1 && level <= 5)
            output.erase(0, 1);

        if (LINES > index)
        {
            this->colourLine(level, index);

            if (index > 0)
            {
                unsigned int linesUsed = 1;
                while (output.size() > this->maxCharsPerLine_)
                {
                    ++linesUsed;
                    this->consoleOverlayTextAreas_[index]->setCaption(convert2UTF(output.substr(0, this->maxCharsPerLine_)));
                    output.erase(0, this->maxCharsPerLine_);
                    output.insert(0, 1, ' ');
                    if (linesUsed > numLinesShifted_ || alwaysShift)
                        this->shiftLines();
                    this->colourLine(level, index);
                }
                this->consoleOverlayTextAreas_[index]->setCaption(convert2UTF(output));
                this->displayedText_ = output;
                this->numLinesShifted_ = linesUsed;
            }
            else
            {
                if (output.size() > this->maxCharsPerLine_)
                {
                    if (Shell::getInstance().getInputBuffer().getCursorPosition() < this->inputWindowStart_)
                        this->inputWindowStart_ = Shell::getInstance().getInputBuffer().getCursorPosition();
                    else if (Shell::getInstance().getInputBuffer().getCursorPosition() >= (this->inputWindowStart_ + this->maxCharsPerLine_ - 1))
                        this->inputWindowStart_ = Shell::getInstance().getInputBuffer().getCursorPosition() - this->maxCharsPerLine_ + 1;

                    output = output.substr(this->inputWindowStart_, this->maxCharsPerLine_);
                }
                else
                  this->inputWindowStart_ = 0;
                this->displayedText_ = output;
                this->consoleOverlayTextAreas_[index]->setCaption(convert2UTF(output));
            }
        }
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
