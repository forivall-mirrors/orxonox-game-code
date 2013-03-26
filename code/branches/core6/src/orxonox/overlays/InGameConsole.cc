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


#include "InGameConsole.h"

#include <algorithm>
#include <string>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreBorderPanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <OgreFont.h>

#include "util/Clock.h"
#include "util/Convert.h"
#include "util/Math.h"
#include "util/DisplayStringConversions.h"
#include "util/ScopedSingletonManager.h"
#include "util/output/MemoryWriter.h"
#include "util/output/OutputManager.h"
#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/command/ConsoleCommand.h"
#include "core/GUIManager.h"
#include "core/input/InputManager.h"
#include "core/input/InputState.h"
#include "core/input/InputBuffer.h"
#include "core/LuaState.h"

namespace orxonox
{
    const int LINES = 30;
    const float CHAR_WIDTH = 8.0f; // fix this please - determine the char-width dynamically

    SetConsoleCommand("InGameConsole", "openConsole", &InGameConsole::openConsole);
    SetConsoleCommand("InGameConsole", "closeConsole", &InGameConsole::closeConsole);

    ManageScopedSingleton(InGameConsole, ScopeID::Graphics, false);

    /**
        @brief Constructor: Creates and initializes the InGameConsole.
    */
    InGameConsole::InGameConsole()
        : shell_(new Shell("InGameConsole", true))
        , bShowCursor_(false)
        , consoleOverlay_(0)
        , consoleOverlayContainer_(0)
        , consoleOverlayNoise_(0)
        , consoleOverlayCursor_(0)
        , consoleOverlayBorder_(0)
        , consoleOverlayTextAreas_(0)
        , inputState_(0)
    {
        RegisterObject(InGameConsole);

        this->bActive_ = false;
        this->cursor_ = 0.0f;
        this->cursorSymbol_ = '|';
        this->inputWindowStart_ = 0;
        this->numLinesShifted_ = LINES - 1;
        // for the beginning, don't scroll
        this->scroll_ = 0;

        this->setConfigValues();
        this->initialise();

        // Output buffering is not anymore needed. Not the best solution to do
        // this here, but there isn't much of another way.
        OutputManager::getInstance().getMemoryWriter()->disable();
    }

    /**
        @brief Destructor: Destroys the TextAreas.
    */
    InGameConsole::~InGameConsole()
    {
        this->deactivate();

        // destroy the input state previously created (InputBuffer gets destroyed by the Shell)
        InputManager::getInstance().destroyState("console");

        // destroy the underlaying shell
        delete this->shell_;

        Ogre::OverlayManager* ovMan = Ogre::OverlayManager::getSingletonPtr();
        if (ovMan)
        {
            if (this->consoleOverlayNoise_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->consoleOverlayNoise_);
            if (this->consoleOverlayCursor_)
                Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->consoleOverlayCursor_);
            Ogre::FontManager::getSingleton().remove("MonofurConsole");
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

        if (this->consoleOverlay_)
            Ogre::OverlayManager::getSingleton().destroy(consoleOverlay_);
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
        SetConfigValue(bHidesAllInput_, false).callback(this, &InGameConsole::bHidesAllInputChanged);
    }

    /**
        @brief Called whenever bHidesAllInput_ changes.
    */
    void InGameConsole::bHidesAllInputChanged()
    {
        if (inputState_)
        {
            if (bHidesAllInput_)
            {
                inputState_->setMouseHandler(&InputHandler::EMPTY);
                inputState_->setJoyStickHandler(&InputHandler::EMPTY);
            }
            else
            {
                inputState_->setMouseHandler(0);
                inputState_->setJoyStickHandler(0);
            }
        }
    }

    /**
        @brief Initializes the InGameConsole.
    */
    void InGameConsole::initialise()
    {
        // create the corresponding input state
        inputState_ = InputManager::getInstance().createInputState("console", false, false, InputStatePriority::Console);
        inputState_->setKeyHandler(this->shell_->getInputBuffer());
        bHidesAllInputChanged();

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
        this->consoleOverlayBorder_->setLeftBorderUV(0.0f, 0.49f, 0.5f, 0.51f);
        this->consoleOverlayBorder_->setRightBorderUV(0.5f, 0.49f, 1.0f, 0.5f);
        this->consoleOverlayBorder_->setBottomBorderUV(0.49f, 0.5f, 0.51f, 1.0f);
        this->consoleOverlayBorder_->setBottomLeftBorderUV(0.0f, 0.5f, 0.5f, 1.0f);
        this->consoleOverlayBorder_->setBottomRightBorderUV(0.5f, 0.5f, 1.0f, 1.0f);
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

        // create noise
        this->consoleOverlayNoise_ = static_cast<Ogre::PanelOverlayElement*>(ovMan->createOverlayElement("Panel", "InGameConsoleNoise"));
        this->consoleOverlayNoise_->setMetricsMode(Ogre::GMM_PIXELS);
        this->consoleOverlayNoise_->setPosition(5,0);
        this->consoleOverlayNoise_->setMaterialName("ConsoleNoiseSmall");
        // comment following line to disable noise
        this->consoleOverlayBorder_->addChild(this->consoleOverlayNoise_);

        // create the text lines
        this->consoleOverlayTextAreas_ = new Ogre::TextAreaOverlayElement*[LINES];
        for (int i = 0; i < LINES; i++)
        {
            this->consoleOverlayTextAreas_[i] = static_cast<Ogre::TextAreaOverlayElement*>(ovMan->createOverlayElement("TextArea", "InGameConsoleTextArea" + multi_cast<std::string>(i)));
            this->consoleOverlayTextAreas_[i]->setMetricsMode(Ogre::GMM_PIXELS);
            this->consoleOverlayTextAreas_[i]->setFontName("MonofurConsole");
            this->consoleOverlayTextAreas_[i]->setCharHeight(18);
            this->consoleOverlayTextAreas_[i]->setParameter("colour_top", "0.21 0.69 0.21");
            this->consoleOverlayTextAreas_[i]->setLeft(8);
            this->consoleOverlayTextAreas_[i]->setCaption("");
            this->consoleOverlayNoise_->addChild(this->consoleOverlayTextAreas_[i]);
        }

        // create cursor (also a text area overlay element)
        this->consoleOverlayCursor_ = static_cast<Ogre::TextAreaOverlayElement*>(ovMan->createOverlayElement("TextArea", "InGameConsoleCursor"));
        this->consoleOverlayCursor_->setMetricsMode(Ogre::GMM_PIXELS);
        this->consoleOverlayCursor_->setFontName("MonofurConsole");
        this->consoleOverlayCursor_->setCharHeight(18);
        this->consoleOverlayCursor_->setParameter("colour_top", "0.21 0.69 0.21");
        this->consoleOverlayCursor_->setLeft(7);
        this->consoleOverlayCursor_->setCaption(std::string(this->cursorSymbol_, 1));
        this->consoleOverlayNoise_->addChild(this->consoleOverlayCursor_);

        this->windowResized(this->getWindowWidth(), this->getWindowWidth());

        // move overlay "above" the top edge of the screen
        // we take -1.3 because the border makes the panel bigger
        this->consoleOverlayContainer_->setTop(-1.3f * this->relativeHeight);

        orxout(internal_info) << "InGameConsole initialized" << endl;
    }

    // ###############################
    // ###  ShellListener methods  ###
    // ###############################

    /**
        @brief Called if all output-lines have to be redrawn.
    */
    void InGameConsole::linesChanged()
    {
        Shell::LineList::const_iterator it = this->shell_->getNewestLineIterator();
        int max = 0;
        for (int i = 1; i < LINES; ++i)
        {
            if (it != this->shell_->getEndIterator())
            {
                ++it;
                max = i;
            }
            else
                break;
        }

        for (int i = LINES - 1; i > max; --i)
            this->print("", Shell::DebugOutput, i, true);

        for (int i = max; i >= 1; --i)
        {
            --it;
            this->print(it->first, it->second, i, true);
        }
    }

    /**
        @brief Called if a new output-line was added.
    */
    void InGameConsole::lineAdded()
    {
        this->numLinesShifted_ = 0;
        this->shiftLines();
        if (LINES > 1)
            this->print(this->shell_->getNewestLineIterator()->first, this->shell_->getNewestLineIterator()->second, 1);
    }

    /**
        @brief Called if the text in the input-line has changed.
    */
    void InGameConsole::inputChanged()
    {
        if (LINES > 0)
            this->print(this->shell_->getInput(), Shell::Input, 0);

        if (this->shell_->getInput().empty())
            this->inputWindowStart_ = 0;
    }

    /**
        @brief Called if the position of the cursor in the input-line has changed.
    */
    void InGameConsole::cursorChanged()
    {
        unsigned int pos = this->shell_->getCursorPosition() - inputWindowStart_;
        if (pos > maxCharsPerLine_)
            pos = maxCharsPerLine_;

        this->consoleOverlayCursor_->setCaption(std::string(pos,' ') + cursorSymbol_);
        this->consoleOverlayCursor_->setTop(static_cast<int>(this->windowH_ * this->relativeHeight) - 24.0f);
    }

    /**
        @brief Called if a command is about to be executed
    */
    void InGameConsole::executed()
    {
        this->shell_->addOutput(this->shell_->getInput(), Shell::Command);
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
    void InGameConsole::preUpdate(const Clock& time)
    {
        if (this->scroll_ != 0)
        {
            float oldTop = this->consoleOverlayContainer_->getTop();

            if (this->scroll_ > 0)
            {
                // scrolling down
                // enlarge oldTop a little bit so that this exponential function
                // reaches 0 before infinite time has passed...
                float deltaScroll = (oldTop - 0.01f) * time.getDeltaTime() * this->scrollSpeed_;
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
                float deltaScroll = (1.3f * this->relativeHeight + 0.01f + oldTop) * time.getDeltaTime() * this->scrollSpeed_;
                if (oldTop - deltaScroll <= -1.3 * this->relativeHeight)
                {
                    // window has completely scrolled up
                    this->consoleOverlayContainer_->setTop(-1.3f * this->relativeHeight);
                    this->scroll_ = 0;
                    this->consoleOverlay_->hide();
                }
                else
                    this->consoleOverlayContainer_->setTop(oldTop - deltaScroll);
            }
        }

        if (this->bActive_)
        {
            this->cursor_ += time.getDeltaTime();
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
    void InGameConsole::windowResized(unsigned int newWidth, unsigned int newHeight)
    {
        this->windowW_ = newWidth;
        this->windowH_ = newHeight;
        this->consoleOverlayBorder_->setWidth((float)(int)(this->windowW_* this->relativeWidth));
        this->consoleOverlayBorder_->setHeight((float)(int)(this->windowH_ * this->relativeHeight));
        this->consoleOverlayNoise_->setWidth((float)(int)(this->windowW_ * this->relativeWidth) - 10.0f);
        this->consoleOverlayNoise_->setHeight((float)(int)(this->windowH_ * this->relativeHeight) - 5.0f);
        this->consoleOverlayNoise_->setTiling(consoleOverlayNoise_->getWidth() / (50.0f * this->noiseSize_), consoleOverlayNoise_->getHeight() / (50.0f * this->noiseSize_));

        // now adjust the text lines...
        this->desiredTextWidth_ = static_cast<int>(this->windowW_ * this->relativeWidth) - 24;

        if (LINES > 0)
            this->maxCharsPerLine_ = std::max(10U, static_cast<unsigned int>(static_cast<float>(this->desiredTextWidth_) / CHAR_WIDTH));
        else
            this->maxCharsPerLine_ = 10;

        for (int i = 0; i < LINES; i++)
        {
            this->consoleOverlayTextAreas_[i]->setWidth((float)this->desiredTextWidth_);
            this->consoleOverlayTextAreas_[i]->setTop((float)(int)(this->windowH_ * this->relativeHeight) - 24 - 14*i);
        }

        this->linesChanged();
        this->cursorChanged();
    }

    // ###############################
    // ###    internal methods     ###
    // ###############################

    /**
        @brief Prints string to bottom line.
        @param text The string to be printed
        @param type The type of the text, defines the color
        @param index The index of the text overlay in which the string will be displayed
        @param alwaysShift If true the ohter lines in the console are always shifted by one line
    */
    void InGameConsole::print(const std::string& text, Shell::LineType type, int index, bool alwaysShift)
    {
        std::string output = text;
        if (LINES > index)
        {
            this->colourLine(type, index);

            if (index > 0)
            {
                unsigned int linesUsed = 1;
                while (output.size() > this->maxCharsPerLine_)
                {
                    ++linesUsed;
                    this->consoleOverlayTextAreas_[index]->setCaption(multi_cast<Ogre::DisplayString>(output.substr(0, this->maxCharsPerLine_)));
                    output.erase(0, this->maxCharsPerLine_);
                    output.insert(0, 1, ' ');
                    if (linesUsed > numLinesShifted_ || alwaysShift)
                        this->shiftLines();
                    this->colourLine(type, index);
                }
                this->consoleOverlayTextAreas_[index]->setCaption(multi_cast<Ogre::DisplayString>(output));
                this->displayedText_ = output;
                this->numLinesShifted_ = linesUsed;
            }
            else
            {
                if (output.size() > this->maxCharsPerLine_)
                {
                    if (this->shell_->getInputBuffer()->getCursorPosition() < this->inputWindowStart_)
                        this->inputWindowStart_ = this->shell_->getInputBuffer()->getCursorPosition();
                    else if (this->shell_->getInputBuffer()->getCursorPosition() >= (this->inputWindowStart_ + this->maxCharsPerLine_ - 1))
                        this->inputWindowStart_ = this->shell_->getInputBuffer()->getCursorPosition() - this->maxCharsPerLine_ + 1;

                    output = output.substr(this->inputWindowStart_, this->maxCharsPerLine_);
                }
                else
                    this->inputWindowStart_ = 0;
                this->displayedText_ = output;
                this->consoleOverlayTextAreas_[index]->setCaption(multi_cast<Ogre::DisplayString>(output));
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
            InputManager::getInstance().enterState("console");
            this->shell_->registerListener(this);

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
            GUIManager::getInstance().getLuaState()->doString("inGameConsoleClosed()"); // Notify the SheetManager in lua, that the console has been closed.
            InputManager::getInstance().leaveState("console");
            this->shell_->unregisterListener(this);

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

    void InGameConsole::colourLine(Shell::LineType type, int index)
    {
        ColourValue colourTop, colourBottom;
        switch (type)
        {
            case Shell::Message:
            case Shell::DebugOutput:     colourTop = ColourValue(0.9f, 0.9f, 0.9f); break;

            case Shell::UserError:       colourTop = ColourValue(0.9f, 0.0f, 0.0f); break;
            case Shell::UserWarning:     colourTop = ColourValue(0.9f, 0.5f, 0.0f); break;
            case Shell::UserStatus:      colourTop = ColourValue(0.0f, 0.9f, 0.0f); break;
            case Shell::UserInfo:        colourTop = ColourValue(0.0f, 0.8f, 0.8f); break;

            case Shell::InternalError:   colourTop = ColourValue(0.5f, 0.0f, 0.0f); break;
            case Shell::InternalWarning: colourTop = ColourValue(0.5f, 0.2f, 0.0f); break;
            case Shell::InternalStatus:  colourTop = ColourValue(0.0f, 0.5f, 0.0f); break;
            case Shell::InternalInfo:    colourTop = ColourValue(0.0f, 0.4f, 0.4f); break;

            case Shell::Verbose:         colourTop = ColourValue(0.3f, 0.3f, 0.9f); break;
            case Shell::VerboseMore:     colourTop = ColourValue(0.2f, 0.2f, 0.7f); break;
            case Shell::VerboseUltra:    colourTop = ColourValue(0.1f, 0.1f, 0.5f); break;

            case Shell::Command:         colourTop = ColourValue(0.8f, 0.2f, 0.8f); break;
            case Shell::Hint:            colourTop = ColourValue(0.4f, 0.0f, 0.4f); break;
            case Shell::Input:           colourTop = ColourValue(0.9f, 0.9f, 0.9f); break;

            default:                     colourTop = ColourValue(0.5f, 0.5f, 0.5f); break;
        }

        colourBottom = ColourValue(sqrt(colourTop.r), sqrt(colourTop.g), sqrt(colourTop.b));

        this->consoleOverlayTextAreas_[index]->setColourTop   (colourTop);
        this->consoleOverlayTextAreas_[index]->setColourBottom(colourBottom);
    }

    // ################################
    // ###      static methods      ###
    // ################################

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
        GUIManager::getInstance().getLuaState()->doString("inGameConsoleClosed()");  // Notify the SheetManager in lua, that the console has been closed, but not by ESC.
        InGameConsole::getInstance().deactivate();
    }

}
