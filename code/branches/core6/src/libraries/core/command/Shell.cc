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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      Reto Grieder
 *
 */

/**
    @file
    @brief Implementation of the Shell class.
*/

#include "Shell.h"

#include "util/Math.h"
#include "util/StringUtils.h"
#include "util/SubString.h"
#include "util/output/OutputManager.h"
#include "util/output/MemoryWriter.h"
#include "core/CoreIncludes.h"
#include "core/config/ConfigFileManager.h"
#include "core/config/ConfigValueIncludes.h"
#include "core/PathConfig.h"
#include "core/input/InputBuffer.h"
#include "CommandExecutor.h"

namespace orxonox
{
    unsigned int Shell::cacheSize_s;

    namespace DefaultLogLevel
    {
        const OutputLevel Dev  = level::internal_warning;
        const OutputLevel User = level::user_info;
    }

    RegisterClassNoArgs(Shell);

    /**
        @brief Constructor: Initializes the values.
        @param consoleName The name of the shell - used to define the name of the soft-debug-level config-value
        @param bScrollable If true, the user is allowed to scroll through the output-lines
    */
    Shell::Shell(const std::string& consoleName, bool bScrollable)
        : BaseWriter(consoleName, false)
        , inputBuffer_(new InputBuffer())
        , bScrollable_(bScrollable)
    {
        RegisterObject(Shell);

        OutputManager::getInstance().registerListener(this);

        this->scrollPosition_ = 0;
        this->maxHistoryLength_ = 100;
        this->historyPosition_ = 0;
        this->historyOffset_ = 0;

        this->clearOutput();
        this->configureInputBuffer();

        // Specify file for the command history
        ConfigFileManager::getInstance().setFilename(ConfigFileType::CommandHistory, "commandHistory.ini");

        // Choose the default level according to the path Orxonox was started (build directory or not)
        OutputLevel defaultDebugLevel = (PathConfig::buildDirectoryRun() ? DefaultLogLevel::Dev : DefaultLogLevel::User);
        this->setLevelMax(defaultDebugLevel);

        this->setConfigValues();

        // Get the previous output and add it to the Shell
        OutputManager::getInstance().getMemoryWriter()->resendOutput(this);
    }

    /**
        @brief Destructor
    */
    Shell::~Shell()
    {
        delete this->inputBuffer_;

        OutputManager::getInstance().unregisterListener(this);
    }

    /**
        @brief Defines the config values.
    */
    void Shell::setConfigValues()
    {
        SetConfigValue(maxHistoryLength_, 100)
            .callback(this, &Shell::commandHistoryLengthChanged);
        SetConfigValue(historyOffset_, 0)
            .callback(this, &Shell::commandHistoryOffsetChanged);
        setConfigValueGeneric(this, &commandHistory_, ConfigFileType::CommandHistory, "Shell", "commandHistory_", std::vector<std::string>());
        SetConfigValue(cacheSize_s, 32);

        SetConfigValueExternal(this->configurableMaxLevel_,
                               this->getConfigurableSectionName(),
                               this->getConfigurableMaxLevelName(),
                               this->configurableMaxLevel_)
            .description("The maximum level of output shown in the " + this->getName())
            .callback(static_cast<BaseWriter*>(this), &BaseWriter::changedConfigurableLevel);
        SetConfigValueExternal(this->configurableAdditionalContextsMaxLevel_,
                               this->getConfigurableSectionName(),
                               this->getConfigurableAdditionalContextsMaxLevelName(),
                               this->configurableAdditionalContextsMaxLevel_)
            .description("The maximum level of output shown in the " + this->getName() + " for additional contexts")
            .callback(static_cast<BaseWriter*>(this), &BaseWriter::changedConfigurableAdditionalContextsLevel);
        SetConfigValueExternal(this->configurableAdditionalContexts_,
                               this->getConfigurableSectionName(),
                               this->getConfigurableAdditionalContextsName(),
                               this->configurableAdditionalContexts_)
            .description("Additional output contexts shown in the " + this->getName())
            .callback(static_cast<BaseWriter*>(this), &BaseWriter::changedConfigurableAdditionalContexts);
    }

    /**
        @brief Config-value callback: Called when the history offset has changed in the config-file.
    */
    void Shell::commandHistoryOffsetChanged()
    {
        if (this->historyOffset_ >= this->maxHistoryLength_)
            this->historyOffset_ = 0;
    }

    /**
        @brief Config-value callback: Called when the length of the command history has changed in the config-file.
    */
    void Shell::commandHistoryLengthChanged()
    {
        this->commandHistoryOffsetChanged();

        while (this->commandHistory_.size() > this->maxHistoryLength_)
        {
            unsigned int index = this->commandHistory_.size() - 1;
            this->commandHistory_.erase(this->commandHistory_.begin() + index);
            ModifyConfigValue(commandHistory_, remove, index);
        }
    }

    /** Called upon changes in the development mode (by Core)
        Behaviour details see Core::devModeChanged.
    */
    void Shell::devModeChanged(bool value)
    {
        bool isNormal = (value == PathConfig::buildDirectoryRun());
        if (isNormal)
        {
            ModifyConfigValueExternal(this->configurableMaxLevel_, this->getConfigurableMaxLevelName(), update);
        }
        else
        {
            OutputLevel level = (value ? DefaultLogLevel::Dev : DefaultLogLevel::User);
            ModifyConfigValueExternal(this->configurableMaxLevel_, this->getConfigurableMaxLevelName(), tset, static_cast<int>(level));
        }
    }

    /**
        @brief Registers this object as listener for different key-events at the input buffer.
    */
    void Shell::configureInputBuffer()
    {
        this->inputBuffer_->registerListener(this, &Shell::inputChanged, true);
        this->inputBuffer_->registerListener(this, &Shell::execute,         '\r',   false);
        this->inputBuffer_->registerListener(this, &Shell::execute,         '\n',   false);
        this->inputBuffer_->registerListener(this, &Shell::hintAndComplete, '\t',   true);
        this->inputBuffer_->registerListener(this, &Shell::backspace,       '\b',   true);
        this->inputBuffer_->registerListener(this, &Shell::backspace,       '\177', true);
        this->inputBuffer_->registerListener(this, &Shell::exit,            '\033', true); // escape
        this->inputBuffer_->registerListener(this, &Shell::deleteChar,      KeyCode::Delete);
        this->inputBuffer_->registerListener(this, &Shell::cursorRight,     KeyCode::Right);
        this->inputBuffer_->registerListener(this, &Shell::cursorLeft,      KeyCode::Left);
        this->inputBuffer_->registerListener(this, &Shell::cursorEnd,       KeyCode::End);
        this->inputBuffer_->registerListener(this, &Shell::cursorHome,      KeyCode::Home);
        this->inputBuffer_->registerListener(this, &Shell::historyUp,       KeyCode::Up);
        this->inputBuffer_->registerListener(this, &Shell::historyDown,     KeyCode::Down);
        if (this->bScrollable_)
        {
            this->inputBuffer_->registerListener(this, &Shell::scrollUp,    KeyCode::PageUp);
            this->inputBuffer_->registerListener(this, &Shell::scrollDown,  KeyCode::PageDown);
        }
        else
        {
            this->inputBuffer_->registerListener(this, &Shell::historySearchUp,   KeyCode::PageUp);
            this->inputBuffer_->registerListener(this, &Shell::historySearchDown, KeyCode::PageDown);
        }
    }

    /**
        @brief Registers a shell listener which listens for changes in this shell.
    */
    void Shell::registerListener(ShellListener* listener)
    {
        this->listeners_.push_back(listener);
    }

    /**
        @brief Unregisters a shell listener.
    */
    void Shell::unregisterListener(ShellListener* listener)
    {
        for (std::list<ShellListener*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); )
        {
            if ((*it) == listener)
                it = this->listeners_.erase(it);
            else
                ++it;
        }
    }

    /**
        @brief Changes the position of the cursor in the input buffer.
    */
    void Shell::setCursorPosition(unsigned int cursor)
    {
        this->inputBuffer_->setCursorPosition(cursor);
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    /// Returns the current position of the cursor in the input buffer.
    unsigned int Shell::getCursorPosition() const
    {
        return this->inputBuffer_->getCursorPosition();
    }

    /// Returns the current content of the input buffer (the text which was entered by the user)
    const std::string& Shell::getInput() const
    {
        return this->inputBuffer_->get();
    }

    /**
        @brief Adds multiple lines to the internal output buffer.
    */
    void Shell::addOutput(const std::string& text, LineType type)
    {
        std::vector<std::string> lines;
        vectorize(text, '\n', &lines);

        for (size_t i = 0; i < lines.size(); ++i)
            this->addLine(lines[i], type);
    }

    /**
        @brief Adds a line to the internal output buffer.
    */
    void Shell::addLine(const std::string& line, LineType type)
    {
        // yes it was - push the new line to the list
        this->outputLines_.push_front(std::make_pair(line, static_cast<LineType>(type)));

        // adjust the scroll position if needed
        if (this->scrollPosition_)
            this->scrollPosition_++;
        else
            this->scrollIterator_ = this->outputLines_.begin();

        if (!this->scrollPosition_)
            this->updateListeners<&ShellListener::lineAdded>();
    }

    /**
        @brief Clears the list of output-lines.
    */
    void Shell::clearOutput()
    {
        this->outputLines_.clear();
        this->scrollIterator_ = this->outputLines_.begin();

        this->scrollPosition_ = 0;

        this->updateListeners<&ShellListener::linesChanged>();
    }

    /**
        @brief Inherited from BaseWriter (LogListener), called if a new line of output was sent.
    */
    void Shell::printLine(const std::string& line, OutputLevel level)
    {
        this->addLine(line, static_cast<LineType>(level));
    }

    /**
        @brief Returns an iterator to the newest line of output (except if the user is currently scrolling through the output).
    */
    Shell::LineList::const_iterator Shell::getNewestLineIterator() const
    {
        if (this->scrollPosition_)
            return this->scrollIterator_;
        else
            return this->outputLines_.begin();
    }

    /**
        @brief Returns the end() iterator of the list of output-lines.
    */
    Shell::LineList::const_iterator Shell::getEndIterator() const
    {
        return this->outputLines_.end();
    }

    /**
        @brief Adds a command to the history of entered commands and writes it to the config-file.
    */
    void Shell::addToHistory(const std::string& command)
    {
        if (command == "")
            return;

        size_t previous_offset = mod(static_cast<int>(this->historyOffset_) - 1, this->maxHistoryLength_);
        if (previous_offset < this->commandHistory_.size() && command == this->commandHistory_[previous_offset])
            return;

        ModifyConfigValue(commandHistory_, set, this->historyOffset_, command);
        this->historyPosition_ = 0;
        ModifyConfigValue(historyOffset_, set, (this->historyOffset_ + 1) % this->maxHistoryLength_);
    }

    /**
        @brief Returns a command from the history of entered commands (usually the most recent history entry, but the user can scroll through the history).
    */
    const std::string& Shell::getFromHistory() const
    {
        unsigned int index = mod(static_cast<int>(this->historyOffset_) - static_cast<int>(this->historyPosition_), this->maxHistoryLength_);
        if (index < this->commandHistory_.size() && this->historyPosition_ != 0)
            return this->commandHistory_[index];
        else
            return BLANKSTRING;
    }

    /**
        @brief Clears the text in the input buffer.
    */
    void Shell::clearInput()
    {
        this->inputBuffer_->clear();
        this->historyPosition_ = 0;
        this->updateListeners<&ShellListener::inputChanged>();
        this->updateListeners<&ShellListener::cursorChanged>();
    }


    // ##########################################
    // ###   InputBuffer callback functions   ###
    // ##########################################

    /// InputBuffer callback: Called if the input changes.
    void Shell::inputChanged()
    {
        this->updateListeners<&ShellListener::inputChanged>();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    /// InputBuffer callback: Called if a key was pressed that executes a command (usually [return]).
    void Shell::execute()
    {
        this->addToHistory(this->inputBuffer_->get());
        this->updateListeners<&ShellListener::executed>();

        int error;
        const std::string& result = CommandExecutor::query(this->inputBuffer_->get(), &error);
        if (error)
            this->addOutput("Error: Can't execute \"" + this->inputBuffer_->get() + "\", " + CommandExecutor::getErrorDescription(error) + ". (Shell)", UserError);
        else if (result != "")
            this->addOutput(result, Result);

        this->clearInput();
    }

    /// InputBuffer callback: Called if a key was pressed that shows hints and completes a command (usually [tab]).
    void Shell::hintAndComplete()
    {
        this->inputBuffer_->set(CommandExecutor::evaluate(this->inputBuffer_->get()).complete());
        this->addOutput(CommandExecutor::evaluate(this->inputBuffer_->get()).hint(), Hint);

        this->inputChanged();
    }

    /// InputBuffer callback: Called if a key was pressed that deletes the character before the cursor (usually [backspace]).
    void Shell::backspace()
    {
        this->inputBuffer_->removeBehindCursor();
        this->updateListeners<&ShellListener::inputChanged>();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    /// InputBuffer callback: Called if a key was pressed that deletes the character after the cursor (usually [delete]).
    void Shell::deleteChar()
    {
        this->inputBuffer_->removeAtCursor();
        this->updateListeners<&ShellListener::inputChanged>();
    }

    /// InputBuffer callback: Called if a key was pressed that moves the input cursor the right (usually [arrow right]).
    void Shell::cursorRight()
    {
        this->inputBuffer_->increaseCursor();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    /// InputBuffer callback: Called if a key was pressed that moves the input cursor the left (usually [arrow left]).
    void Shell::cursorLeft()
    {
        this->inputBuffer_->decreaseCursor();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    /// InputBuffer callback: Called if a key was pressed that moves the input cursor the end of the input line (usually [end]).
    void Shell::cursorEnd()
    {
        this->inputBuffer_->setCursorToEnd();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    /// InputBuffer callback: Called if a key was pressed that moves the input cursor the beginning of the input line (usually [home]).
    void Shell::cursorHome()
    {
        this->inputBuffer_->setCursorToBegin();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    /// InputBuffer callback: Called if a key was pressed that scrolls upwards through the history of entered commands (usually [arrow up]).
    void Shell::historyUp()
    {
        if (this->historyPosition_ < this->commandHistory_.size())
        {
            this->historyPosition_++;
            this->inputBuffer_->set(this->getFromHistory());
        }
    }

    /// InputBuffer callback: Called if a key was pressed that scrolls downwards through the history of entered commands (usually [arrow down]).
    void Shell::historyDown()
    {
        if (this->historyPosition_ > 0)
        {
            this->historyPosition_--;
            this->inputBuffer_->set(this->getFromHistory());
        }
    }

    /// InputBuffer callback: Called if a key was pressed that searches upwards through the history for a command stat starts like the one the user is currently typing (usually [page up]). Only if the shell is not scrollable.
    void Shell::historySearchUp()
    {
        if (this->historyPosition_ == this->historyOffset_)
            return;
        unsigned int cursorPosition = this->getCursorPosition();
        const std::string& input_str(this->getInput().substr(0, cursorPosition)); // only search for the expression from the beginning of the inputline until the cursor position
        for (unsigned int newPos = this->historyPosition_ + 1; newPos <= this->historyOffset_; newPos++)
        {
            if (getLowercase(this->commandHistory_[this->historyOffset_ - newPos]).find(getLowercase(input_str)) == 0) // search case insensitive
            {
                this->historyPosition_ = newPos;
                this->inputBuffer_->set(this->getFromHistory());
                this->setCursorPosition(cursorPosition);
                return;
            }
        }
    }

    /// InputBuffer callback: Called if a key was pressed that searches downwards through the history for a command stat starts like the one the user is currently typing (usually [page down]). Only if the shell is not scrollable.
    void Shell::historySearchDown()
    {
        if (this->historyPosition_ == 0)
            return;
        unsigned int cursorPosition = this->getCursorPosition();
        const std::string& input_str(this->getInput().substr(0, cursorPosition)); // only search for the expression from the beginning
        for (unsigned int newPos = this->historyPosition_ - 1; newPos > 0; newPos--)
        {
            if (getLowercase(this->commandHistory_[this->historyOffset_ - newPos]).find(getLowercase(input_str)) == 0) // sear$
            {
                this->historyPosition_ = newPos;
                this->inputBuffer_->set(this->getFromHistory());
                this->setCursorPosition(cursorPosition);
                return;
            }
        }
    }

    /// InputBuffer callback: Called if a key was pressed that scrolls upwards through the output history (usually [page up]). Only if the shell is scrollable.
    void Shell::scrollUp()
    {
        if (this->scrollIterator_ != this->outputLines_.end())
        {
            ++this->scrollIterator_;
            ++this->scrollPosition_;

            this->updateListeners<&ShellListener::linesChanged>();
        }
    }

    /// InputBuffer callback: Called if a key was pressed that scrolls downwards through the output history (usually [page down]). Only if the shell is scrollable.
    void Shell::scrollDown()
    {
        if (this->scrollIterator_ != this->outputLines_.begin())
        {
            --this->scrollIterator_;
            --this->scrollPosition_;

            this->updateListeners<&ShellListener::linesChanged>();
        }
    }

    /// InputBuffer callback: Called if a key was pressed that clears the text in the input buffer or closes the shell (usually [esc]).
    void Shell::exit()
    {
        if (this->inputBuffer_->getSize() > 0)
        {
            this->clearInput();
            return;
        }

        this->clearInput();
        this->scrollPosition_ = 0;
        this->scrollIterator_ = this->outputLines_.begin();

        this->updateListeners<&ShellListener::exit>();
    }
}
