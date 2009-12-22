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

#include "Shell.h"

#include "util/OutputHandler.h"
#include "util/StringUtils.h"
#include "util/SubString.h"
#include "CommandExecutor.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"
#include "ConsoleCommand.h"

namespace orxonox
{
    SetConsoleCommandShortcut(OutputHandler, log);
    SetConsoleCommandShortcut(OutputHandler, error);
    SetConsoleCommandShortcut(OutputHandler, warning);
    SetConsoleCommandShortcut(OutputHandler, info);
    SetConsoleCommandShortcut(OutputHandler, debug);

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

    Shell::~Shell()
    {
        OutputHandler::getInstance().unregisterOutputListener(this);
        this->inputBuffer_->destroy();
    }

    void Shell::setConfigValues()
    {
        SetConfigValue(maxHistoryLength_, 100)
            .callback(this, &Shell::commandHistoryLengthChanged);
        SetConfigValue(historyOffset_, 0)
            .callback(this, &Shell::commandHistoryOffsetChanged);
        setConfigValueGeneric(this, &commandHistory_, ConfigFileType::CommandHistory, "Shell", "commandHistory_", std::vector<std::string>());

#ifdef ORXONOX_RELEASE
        const unsigned int defaultLevel = 1;
#else
        const unsigned int defaultLevel = 3;
#endif
        setConfigValueGeneric(this, &softDebugLevel_, ConfigFileType::Settings, "OutputHandler", "softDebugLevel" + this->consoleName_, defaultLevel)
            .description("The maximal level of debug output shown in the Shell");
        this->setSoftDebugLevel(this->softDebugLevel_);
    }

    void Shell::commandHistoryOffsetChanged()
    {
        if (this->historyOffset_ >= this->maxHistoryLength_)
            this->historyOffset_ = 0;
    }

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

    /*
    void Shell::history()
    {
        Shell& instance = Shell::getInstance();

        for (unsigned int i = instance.historyOffset_; i < instance.commandHistory_.size(); ++i)
            instance.addOutput(instance.commandHistory_[i] + '\n', -1);
        for (unsigned int i =  0; i < instance.historyOffset_; ++i)
            instance.addOutput(instance.commandHistory_[i] + '\n', -1);
    }
    */

    void Shell::registerListener(ShellListener* listener)
    {
        this->listeners_.push_back(listener);
    }

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

    void Shell::setCursorPosition(unsigned int cursor)
    {
        this->inputBuffer_->setCursorPosition(cursor);
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    void Shell::addOutput(const std::string& text, LineType type)
    {
        this->outputBuffer_ << text;
        this->outputChanged(type);
    }

    void Shell::clearOutput()
    {
        this->outputLines_.clear();
        this->scrollIterator_ = this->outputLines_.begin();

        this->scrollPosition_ = 0;
        this->bFinishedLastLine_ = true;

        this->updateListeners<&ShellListener::linesChanged>();
    }

    Shell::LineList::const_iterator Shell::getNewestLineIterator() const
    {
        if (this->scrollPosition_)
            return this->scrollIterator_;
        else
            return this->outputLines_.begin();
    }

    Shell::LineList::const_iterator Shell::getEndIterator() const
    {
        return this->outputLines_.end();
    }

    void Shell::addToHistory(const std::string& command)
    {
        ModifyConfigValue(commandHistory_, set, this->historyOffset_, command);
        this->historyPosition_ = 0;
        ModifyConfigValue(historyOffset_, set, (this->historyOffset_ + 1) % this->maxHistoryLength_);
    }

    const std::string& Shell::getFromHistory() const
    {
        unsigned int index = mod(static_cast<int>(this->historyOffset_) - static_cast<int>(this->historyPosition_), this->maxHistoryLength_);
        if (index < this->commandHistory_.size() && this->historyPosition_ != 0)
            return this->commandHistory_[index];
        else
            return BLANKSTRING;
    }

    void Shell::outputChanged(int lineType)
    {
        bool newline = false;
        do
        {
            std::string output;
            std::getline(this->outputBuffer_, output);

            bool eof = this->outputBuffer_.eof();
            bool fail = this->outputBuffer_.fail();
            if (eof)
                this->outputBuffer_.flush();
            if (eof || fail)
                this->outputBuffer_.clear();
            newline = (!eof && !fail);

            if (!newline && output.empty())
                break;

            if (this->bFinishedLastLine_)
            {
                this->outputLines_.push_front(std::make_pair(output, static_cast<LineType>(lineType)));

                if (this->scrollPosition_)
                    this->scrollPosition_++;
                else
                    this->scrollIterator_ = this->outputLines_.begin();

                this->bFinishedLastLine_ = newline;

                if (!this->scrollPosition_)
                    this->updateListeners<&ShellListener::lineAdded>();
            }
            else
            {
                this->outputLines_.front().first += output;
                this->bFinishedLastLine_ = newline;
                this->updateListeners<&ShellListener::onlyLastLineChanged>();
            }
            this->bFinishedLastLine_ = newline;

        } while (newline);
    }

    void Shell::clearInput()
    {
        this->inputBuffer_->clear();
        this->historyPosition_ = 0;
        this->updateListeners<&ShellListener::inputChanged>();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    void Shell::setPromptPrefix(const std::string& str)
    {
    }


    // ##########################################
    // ###   InputBuffer callback functions   ###
    // ##########################################

    void Shell::inputChanged()
    {
        this->updateListeners<&ShellListener::inputChanged>();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    void Shell::execute()
    {
        this->addToHistory(this->inputBuffer_->get());
        this->updateListeners<&ShellListener::executed>();

        if (!CommandExecutor::execute(this->inputBuffer_->get()))
        {
            this->outputBuffer_ << "Error: Can't execute \"" << this->inputBuffer_->get() << "\"." << std::endl;
            this->outputChanged(Error);
        }

        this->clearInput();
    }

    void Shell::hintAndComplete()
    {
        this->inputBuffer_->set(CommandExecutor::complete(this->inputBuffer_->get()));
        this->outputBuffer_ << CommandExecutor::hint(this->inputBuffer_->get()) << std::endl;
        this->outputChanged(Hint);

        this->inputChanged();
    }

    void Shell::backspace()
    {
        this->inputBuffer_->removeBehindCursor();
        this->updateListeners<&ShellListener::inputChanged>();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

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

    void Shell::deleteChar()
    {
        this->inputBuffer_->removeAtCursor();
        this->updateListeners<&ShellListener::inputChanged>();
    }

    void Shell::cursorRight()
    {
        this->inputBuffer_->increaseCursor();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    void Shell::cursorLeft()
    {
        this->inputBuffer_->decreaseCursor();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    void Shell::cursorEnd()
    {
        this->inputBuffer_->setCursorToEnd();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    void Shell::cursorHome()
    {
        this->inputBuffer_->setCursorToBegin();
        this->updateListeners<&ShellListener::cursorChanged>();
    }

    void Shell::historyUp()
    {
        if (this->historyPosition_ < this->commandHistory_.size())
        {
            this->historyPosition_++;
            this->inputBuffer_->set(this->getFromHistory());
        }
    }

    void Shell::historyDown()
    {
        if (this->historyPosition_ > 0)
        {
            this->historyPosition_--;
            this->inputBuffer_->set(this->getFromHistory());
        }
    }

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

    void Shell::scrollUp()
    {
        if (this->scrollIterator_ != this->outputLines_.end())
        {
            ++this->scrollIterator_;
            ++this->scrollPosition_;

            this->updateListeners<&ShellListener::linesChanged>();
        }
    }

    void Shell::scrollDown()
    {
        if (this->scrollIterator_ != this->outputLines_.begin())
        {
            --this->scrollIterator_;
            --this->scrollPosition_;

            this->updateListeners<&ShellListener::linesChanged>();
        }
    }
}
