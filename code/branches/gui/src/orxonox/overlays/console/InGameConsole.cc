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
#include <OgreFontManager.h>
#include <OgreFont.h>

#include "util/Math.h"
#include "util/Convert.h"
#include "core/Debug.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "core/input/InputBuffer.h"
#include "GraphicsEngine.h"

#define LINES 30
#define CHAR_WIDTH 7.45 // fix this please - determine the char-width dynamically

namespace orxonox
{
    SetConsoleCommand(InGameConsole, openConsole, true);
    SetConsoleCommand(InGameConsole, closeConsole, true);

    InGameConsole* InGameConsole::singletonRef_s = 0;

    /**
        @brief Constructor: Creates and initializes the InGameConsole.
    */
    InGameConsole::InGameConsole()
        : consoleOverlay_(0)
        , consoleOverlayContainer_(0)
        , consoleOverlayNoise_(0)
        , consoleOverlayCursor_(0)
        , consoleOverlayBorder_(0)
        , consoleOverlayTextAreas_(0)
    {
        RegisterObject(InGameConsole);

        assert(singletonRef_s == 0);
        singletonRef_s = this;

        this->bActive_ = false;
        this->cursor_ = 0.0f;
        this->cursorSymbol_ = '|';
        this->inputWindowStart_ = 0;
        this->numLinesShifted_ = LINES - 1;
        // for the beginning, don't scroll
        this->scroll_ = 0;

        this->setConfigValues();
    }

    /**
        @brief Destructor: Destroys the TextAreas.
    */
    InGameConsole::~InGameConsole(void)
    {
        this->deactivate();

        // destroy the input state previously created (InputBuffer gets destroyed by the Shell)
        InputManager::getInstance().destroyState("console");

        Ogre::OverlayManager* ovMan = Ogre::OverlayManager::getSingletonPtr();
        if (ovMan)
        {
            if (this->consoleOverlayNoise_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->consoleOverlayNoise_);
            if (this->consoleOverlayCursor_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->consoleOverlayCursor_);
            if (this->consoleOverlayBorder_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->consoleOverlayBorder_);
            if (this->consoleOverlayTextAreas_)
            {
                for (int i = 0; i < LINES; i++)
                {
                    if (this->consoleOverlayTextAreas_[i])
                      Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->consoleOverlayTextAreas_[i]);
                    this->consoleOverlayTextAreas_[i] = 0;
                }

            }
            if (this->consoleOverlayContainer_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->consoleOverlayContainer_);
        }
        if (this->consoleOverlayTextAreas_)
        {
            delete[] this->consoleOverlayTextAreas_;
            this->consoleOverlayTextAreas_ = 0;
        }

        singletonRef_s = 0;
    }

    /**
        @brief Sets the config values, describing the size of the console.
    */
    void InGameConsole::setConfigValues()
    {
        SetConfigValue(relativeWidth, 0.8);
        SetConfigValue(relativeHeight, 0.4);
        SetConfigValue(blinkTime, 0.5);
        SetConfigValue(scrollSpeed_, 3.0f);
        SetConfigValue(noiseSize_, 1.0f);
        SetConfigValue(cursorSymbol_, '|');
    }

    /**
        @brief Initializes the InGameConsole.
    */
    void InGameConsole::initialise()
    {
        // create the corresponding input state
        InputManager::getInstance().createSimpleInputState("console", 40)
            ->setKeyHandler(Shell::getInstance().getInputBuffer());

        // create overlay and elements
        Ogre::OverlayManager* ovMan = Ogre::OverlayManager::getSingletonPtr();

        // create actual overlay
        this->consoleOverlay_ = ovMan->create("InGameConsoleConsole");

        // create a container
        this->consoleOverlayContainer_ = static_cast<Ogre::OverlayContainer*>(ovMan->createOverlayElement("Panel", "InGameConsoleContainer"));
        this->consoleOverlayContainer_->setMetricsMode(Ogre::GMM_RELATIVE);
        this->consoleOverlayContainer_->setPosition((1 - this->relativeWidth) / 2, 0);
        this->consoleOverlayContainer_->setDimensions(this->relativeWidth, this->relativeHeight);
        this->consoleOverlay_->add2D(this->consoleOverlayContainer_);

        // create BorderPanel
        this->consoleOverlayBorder_ = static_cast<Ogre::BorderPanelOverlayElement*>(ovMan->createOverlayElement("BorderPanel", "InGameConsoleBorderPanel"));
        this->consoleOverlayBorder_->setMetricsMode(Ogre::GMM_PIXELS);
        this->consoleOverlayBorder_->setMaterialName("ConsoleCenter");
        this->consoleOverlayBorder_->setBorderSize(16, 16, 0, 16);
        this->consoleOverlayBorder_->setBorderMaterialName("ConsoleBorder");
        this->consoleOverlayBorder_->setLeftBorderUV(0.0, 0.49, 0.5, 0.51);
        this->consoleOverlayBorder_->setRightBorderUV(0.5, 0.49, 1.0, 0.5);
        this->consoleOverlayBorder_->setBottomBorderUV(0.49, 0.5, 0.51, 1.0);
        this->consoleOverlayBorder_->setBottomLeftBorderUV(0.0, 0.5, 0.5, 1.0);
        this->consoleOverlayBorder_->setBottomRightBorderUV(0.5, 0.5, 1.0, 1.0);
        this->consoleOverlayContainer_->addChild(this->consoleOverlayBorder_);

        // create a new font to match the requested size exactly
        Ogre::FontPtr font = static_cast<Ogre::FontPtr>
            (Ogre::FontManager::getSingleton().create("MonofurConsole", "General"));
        font->setType(Ogre::FT_TRUETYPE);
        font->setSource("Monofur.ttf");
        font->setTrueTypeSize(18);
        // reto: I don't know why, but setting the resolution twice as high makes the font look a lot clearer
        font->setTrueTypeResolution(192);
        font->addCodePointRange(Ogre::Font::CodePointRange(33, 126));
        font->addCodePointRange(Ogre::Font::CodePointRange(161, 255));

        // create the text lines
        this->consoleOverlayTextAreas_ = new Ogre::TextAreaOverlayElement*[LINES];
        for (int i = 0; i < LINES; i++)
        {
            this->consoleOverlayTextAreas_[i] = static_cast<Ogre::TextAreaOverlayElement*>(ovMan->createOverlayElement("TextArea", "InGameConsoleTextArea" + convertToString(i)));
            this->consoleOverlayTextAreas_[i]->setMetricsMode(Ogre::GMM_PIXELS);
            this->consoleOverlayTextAreas_[i]->setFontName("MonofurConsole");
            this->consoleOverlayTextAreas_[i]->setCharHeight(18);
            this->consoleOverlayTextAreas_[i]->setParameter("colour_top", "0.21 0.69 0.21");
            this->consoleOverlayTextAreas_[i]->setLeft(8);
            this->consoleOverlayTextAreas_[i]->setCaption("");
            this->consoleOverlayContainer_->addChild(this->consoleOverlayTextAreas_[i]);
        }

        // create cursor (also a text area overlay element)
        this->consoleOverlayCursor_ = static_cast<Ogre::TextAreaOverlayElement*>(ovMan->createOverlayElement("TextArea", "InGameConsoleCursor"));
        this->consoleOverlayCursor_->setMetricsMode(Ogre::GMM_PIXELS);
        this->consoleOverlayCursor_->setFontName("MonofurConsole");
        this->consoleOverlayCursor_->setCharHeight(18);
        this->consoleOverlayCursor_->setParameter("colour_top", "0.21 0.69 0.21");
        this->consoleOverlayCursor_->setLeft(7);
        this->consoleOverlayCursor_->setCaption(std::string(this->cursorSymbol_, 1));
        this->consoleOverlayContainer_->addChild(this->consoleOverlayCursor_);

        // create noise
        this->consoleOverlayNoise_ = static_cast<Ogre::PanelOverlayElement*>(ovMan->createOverlayElement("Panel", "InGameConsoleNoise"));
        this->consoleOverlayNoise_->setMetricsMode(Ogre::GMM_PIXELS);
        this->consoleOverlayNoise_->setPosition(5,0);
        this->consoleOverlayNoise_->setMaterialName("ConsoleNoiseSmall");
        // comment following line to disable noise
        this->consoleOverlayContainer_->addChild(this->consoleOverlayNoise_);

        this->windowResized(GraphicsEngine::getSingleton().getWindowWidth(), GraphicsEngine::getSingleton().getWindowHeight());

        // move overlay "above" the top edge of the screen
        // we take -1.2 because the border makes the panel bigger
        this->consoleOverlayContainer_->setTop(-1.2 * this->relativeHeight);

        Shell::getInstance().addOutputLevel(true);

        COUT(4) << "Info: InGameConsole initialized" << std::endl;
    }

    // ###############################
    // ###  ShellListener methods  ###
    // ###############################

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
        unsigned int pos = Shell::getInstance().getCursorPosition() - inputWindowStart_;
        if (pos > maxCharsPerLine_)
            pos = maxCharsPerLine_;
        else if (pos < 0)
            pos = 0;

        this->consoleOverlayCursor_->setCaption(std::string(pos,' ') + cursorSymbol_);
        this->consoleOverlayCursor_->setTop((int) this->windowH_ * this->relativeHeight - 24);
    }

    /**
        @brief Called if the console gets closed.
    */
    void InGameConsole::exit()
    {
        this->deactivate();
    }

    // ###############################
    // ###  other external calls   ###
    // ###############################

    /**
        @brief Used to control the actual scrolling and the cursor.
    */
    void InGameConsole::tick(float dt)
    {
        if (this->scroll_ != 0)
        {
            float oldTop = this->consoleOverlayContainer_->getTop();

            if (this->scroll_ > 0)
            {
                // scrolling down
                // enlarge oldTop a little bit so that this exponential function
                // reaches 0 before infinite time has passed...
                float deltaScroll = (oldTop - 0.01) * dt * this->scrollSpeed_;
                if (oldTop - deltaScroll >= 0)
                {
                    // window has completely scrolled down
                    this->consoleOverlayContainer_->setTop(0);
                    this->scroll_ = 0;
                }
                else
                    this->consoleOverlayContainer_->setTop(oldTop - deltaScroll);
            }

            else
            {
                // scrolling up
                // note: +0.01 for the same reason as when scrolling down
                float deltaScroll = (1.2 * this->relativeHeight + 0.01 + oldTop) * dt * this->scrollSpeed_;
                if (oldTop - deltaScroll <= -1.2 * this->relativeHeight)
                {
                    // window has completely scrolled up
                    this->consoleOverlayContainer_->setTop(-1.2 * this->relativeHeight);
                    this->scroll_ = 0;
                    this->consoleOverlay_->hide();
                }
                else
                    this->consoleOverlayContainer_->setTop(oldTop - deltaScroll);
            }
        }

        if (this->bActive_)
        {
            this->cursor_ += dt;
            if (this->cursor_ >= this->blinkTime)
            {
                this->cursor_ = 0;
                bShowCursor_ = !bShowCursor_;
                if (bShowCursor_)
                    this->consoleOverlayCursor_->show();
                else
                    this->consoleOverlayCursor_->hide();
            }

            // this creates a flickering effect (extracts exactly 80% of the texture at a random location)
            float uRand = (rand() & 1023) / 1023.0f * 0.2f;
            float vRand = (rand() & 1023) / 1023.0f * 0.2f;
            this->consoleOverlayNoise_->setUV(uRand, vRand, 0.8f + uRand, 0.8f + vRand);
        }
    }

    /**
        @brief Resizes the console elements. Call if window size changes.
    */
    void InGameConsole::windowResized(int newWidth, int newHeight)
    {
        this->windowW_ = newWidth;
        this->windowH_ = newHeight;
        this->consoleOverlayBorder_->setWidth((int) this->windowW_* this->relativeWidth);
        this->consoleOverlayBorder_->setHeight((int) this->windowH_ * this->relativeHeight);
        this->consoleOverlayNoise_->setWidth((int) this->windowW_ * this->relativeWidth - 10);
        this->consoleOverlayNoise_->setHeight((int) this->windowH_ * this->relativeHeight - 5);
        this->consoleOverlayNoise_->setTiling(consoleOverlayNoise_->getWidth() / (50.0f * this->noiseSize_), consoleOverlayNoise_->getHeight() / (50.0f * this->noiseSize_));

        // now adjust the text lines...
        this->desiredTextWidth_ = (int) (this->windowW_ * this->relativeWidth) - 12;

        if (LINES > 0)
            this->maxCharsPerLine_ = max((unsigned int)10, (unsigned int) ((float)this->desiredTextWidth_ / CHAR_WIDTH));
        else
            this->maxCharsPerLine_ = 10;

        for (int i = 0; i < LINES; i++)
        {
            this->consoleOverlayTextAreas_[i]->setWidth(this->desiredTextWidth_);
            this->consoleOverlayTextAreas_[i]->setTop((int) this->windowH_ * this->relativeHeight - 24 - 14*i);
        }

        this->linesChanged();
        this->cursorChanged();
    }

    // ###############################
    // ###    internal methods     ###
    // ###############################

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
                    if (Shell::getInstance().getInputBuffer()->getCursorPosition() < this->inputWindowStart_)
                        this->inputWindowStart_ = Shell::getInstance().getInputBuffer()->getCursorPosition();
                    else if (Shell::getInstance().getInputBuffer()->getCursorPosition() >= (this->inputWindowStart_ + this->maxCharsPerLine_ - 1))
                        this->inputWindowStart_ = Shell::getInstance().getInputBuffer()->getCursorPosition() - this->maxCharsPerLine_ + 1;

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
        @brief Shows the InGameConsole.
    */
    void InGameConsole::activate()
    {
        if (!this->bActive_)
        {
            this->bActive_ = true;
            InputManager::getInstance().requestEnterState("console");
            Shell::getInstance().registerListener(this);

            this->windowResized(this->windowW_, this->windowH_);
            this->linesChanged();
            this->cursorChanged();
            this->consoleOverlay_->show();

            // scroll down
            this->scroll_ = 1;
            // the rest is done by tick
        }
    }

    /**
    @brief Hides the InGameConsole.
    */
    void InGameConsole::deactivate()
    {
        if (this->bActive_)
        {
            this->bActive_ = false;
            InputManager::getInstance().requestLeaveState("console");
            Shell::getInstance().unregisterListener(this);

            // scroll up
            this->scroll_ = -1;
            // the rest is done by tick
        }
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

    // ###############################
    // ###      satic methods      ###
    // ###############################

    /**
        @brief Activates the console.
    */
    /*static*/ void InGameConsole::openConsole()
    {
        InGameConsole::getInstance().activate();
    }

    /**
        @brief Deactivates the console.
    */
    /*static*/ void InGameConsole::closeConsole()
    {
        InGameConsole::getInstance().deactivate();
    }

    /**
        @brief Converts a string into an Ogre::UTFString.
        @param s The string to convert
        @return The converted string
    */
    /*static*/ Ogre::UTFString InGameConsole::convert2UTF(std::string s)
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
