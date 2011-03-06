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

#include "util/OutputHandler.h"
#include "util/StringUtils.h"
#include "util/SubString.h"
#include "core/CoreIncludes.h"
#include "core/ConfigFileManager.h"
#include "core/ConfigValueIncludes.h"
#include "CommandExecutor.h"
#include "ConsoleCommand.h"

namespace orxonox
{
    SetConsoleCommand("log",     OutputHandler::log    );
    SetConsoleCommand("error",   OutputHandler::error  ).hide();
    SetConsoleCommand("warning", OutputHandler::warning).hide();
    SetConsoleCommand("info",    OutputHandler::info   ).hide();
    SetConsoleCommand("debug",   OutputHandler::debug  ).hide();

    unsigned int Shell::cacheSize_s;

    /**
        @brief Constructor: Initializes the values and registers itself at OutputHandler.
        @param consoleName The name of the shell - used to define the name of the soft-debug-level config-value
        @param bScrollable If true, the user is allowed to scroll through the output-lines
    */
    Shell::Shell(const std::string& consoleName, bool bScrollable)
        : OutputListener(consoleName)
        , inputBuffer_(new InputBuffer())
        , consoleName_(consoleName)
        , bScrollable_(bScrollable)
    {
        RegisterRootObject(Shell);

        this->scrollPosition_ = 0;
        this->maxHistoryLength_ = 100;
        this->historyPosition_ = 0;
        this->historyOffset_ = 0;
        this->bFinishedLastLine_ = true;

        this->clearOutput();
        this->configureInputBuffer();

        // Specify file for the command history
        ConfigFileManager::getInstance().setFilename(ConfigFileType::CommandHistory, "commandHistory.ini");

        // Use a stringstream object to buffer the output
        this->outputStream_ = &this->outputBuffer_;

        this->setConfigValues();

        // Get the previous output and add it to the Shell
        for (OutputHandler::OutputVectorIterator it = OutputHandler::getInstance().getOutputVectorBegin();
            it != OutputHandler::getInstance().getOutputVectorEnd(); ++it)
        {
            if (it->first <= this->getSoftDebugLevel())
            {
                this->outputBuffer_ << it->second;
                this->outputChanged(it->first);
            }
        }

        // Register the shell as output listener
        OutputHandler::getInstance().registerOutputListener(this);
    }

    /**
        @brief Destructor: Unregisters the shell from OutputHandler.
    */
    Shell::~Shell()
    {
        OutputHandler::getInstance().unregisterOutputListener(this);
        this->inputBuffer_->destroy();
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

#ifdef ORXONOX_RELEASE
        const unsigned int defaultLevel = 1;
#else
        const unsigned int defaultLevel = 3;
#endif
        SetConfigValueExternal(softDebugLevel_, "OutputHandler", "softDebugLevel" + this->consoleName_, defaultLevel)
            .description("The maximal level of debug output shown in the Shell");
        this->setSoftDebugLevel(this->softDebugLevel_);
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

    /**
        @brief Sends output to the internal output buffer.
    */
    void Shell::addOutput(const std::string& text, LineType type)
    {
        this->outputBuffer_ << text;
        this->outputChanged(type);
    }

    /**
        @brief Clears the list of output-lines.
    */
    void Shell::clearOutput()
    {
        this->outputLines_.clear();
        this->scrollIterator_ = this->outputLines_.begin();

        this->scrollPosition_ = 0;
        this->bFinishedLastLine_ = true;

        this->updateListeners<&ShellListener::linesChanged>();
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

        size_t previous_offset = mod(this->historyOffset_ - 1, this->maxHistoryLength_);
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
        @brief Called by OutputHandler or internally whenever output was sent to the output buffer. Reads from the buffer and writes the new output-lines to the list.
    */
    void Shell::outputChanged(int lineType)
    {
        bool newline = false;
        do
        {
            // get the first line from the buffer
            std::string output;
            std::getline(this->outputBuffer_, output);

            // check the state of the buffer
            bool eof = this->outputBuffer_.eof();
            bool fail = this->outputBuffer_.fail();
            if (eof)
                this->outputBuffer_.flush(); // check if more output was received in the meantime
            if (eof || fail)
                this->outputBuffer_.clear(); // clear the error flags

            // the line is terminated with a line-break if neither an error occurred nor the end of the file was reached
            newline = (!eof && !fail);

            // no output retrieved - break the loop
            if (!newline && output.empty())
                break;

            // check if the last line was terminated with a line-break
            if (this->bFinishedLastLine_)
            {
                // yes it was - push the new line to the list
                this->outputLines_.push_front(std::make_pair(output, static_cast<LineType>(lineType)));

                // adjust the scroll position if needed
                if (this->scrollPosition_)
                    this->scrollPosition_++;
                else
                    this->scrollIterator_ = this->outputLines_.begin();

                if (!this->scrollPosition_)
                    this->updateListeners<&ShellListener::lineAdded>();
            }
            else
            {
                // no it wasn't - add the new output to the last line
                this->outputLines_.front().first += output;
                this->updateListeners<&ShellListener::onlyLastLineChanged>();
            }

            // remember if the last line was terminated with a line-break
            this->bFinishedLastLine_ = newline;

        } while (newline); // loop as long as more lines are in the buffer
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
        {
            switch (error)
            {
                case CommandExecutor::Error:       this->outputBuffer_ << "Error: Can't execute \"" << this->inputBuffer_->get() << "\", command doesn't exist. (S)" << std::endl; break;
                case CommandExecutor::Incomplete:  this->outputBuffer_ << "Error: Can't execute \"" << this->inputBuffer_->get() << "\", not enough arguments given. (S)" << std::endl; break;
                case CommandExecutor::Deactivated: this->outputBuffer_ << "Error: Can't execute \"" << this->inputBuffer_->get() << "\", command is not active. (S)" << std::endl; break;
                case CommandExecutor::Denied:      this->outputBuffer_ << "Error: Can't execute \"" << this->inputBuffer_->get() << "\", access denied. (S)" << std::endl; break;
            }
            this->outputChanged(Error);
        }
        else if (result != "")
        {
            this->outputBuffer_ << result << std::endl;
            this->outputChanged(Command);
        }

        this->clearInput();
    }

    /// InputBuffer callback: Called if a key was pressed that shows hints and completes a command (usually [tab]).
    void Shell::hintAndComplete()
    {
        this->inputBuffer_->set(CommandExecutor::evaluate(this->inputBuffer_->get()).complete());
        this->outputBuffer_ << CommandExecutor::evaluate(this->inputBuffer_->get()).hint() << std::endl;
        this->outputChanged(Hint);

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
