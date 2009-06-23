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
 *      ...
 *
 */

#include "Shell.h"
#include "CommandExecutor.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"
#include "CoreSettings.h"
#include "ConsoleCommand.h"
#include "InputInterfaces.h"

#define SHELL_UPDATE_LISTENERS(function) \
    for (std::list<ShellListener*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it) \
        (*it)->function()

namespace orxonox
{
    SetConsoleCommand(Shell, clearShell, true);
    SetConsoleCommand(Shell, history, true);

    Shell::Shell()
    {
        RegisterRootObject(Shell);

        this->scrollPosition_ = 0;
        this->maxHistoryLength_ = 100;
        this->historyPosition_ = 0;
        this->historyOffset_ = 0;
        this->finishedLastLine_ = true;
        this->bAddOutputLevel_ = false;

        this->clearLines();

        this->inputBuffer_ = 0;
        this->setInputBuffer(new InputBuffer());

        this->outputBuffer_.registerListener(this);

        this->setConfigValues();
    }

    Shell& Shell::createShell()
    {
        int level = CoreSettings::getSoftDebugLevel(OutputHandler::LD_Shell);
        CoreSettings::setSoftDebugLevel(OutputHandler::LD_Shell, -1);
        static Shell instance;
        CoreSettings::setSoftDebugLevel(OutputHandler::LD_Shell, level);
        return instance;
    }

    Shell& Shell::getInstance()
    {
        static Shell& instance = createShell();
        return instance;
    }

    void Shell::setConfigValues()
    {
        SetConfigValue(maxHistoryLength_, 100);
        SetConfigValue(historyOffset_, 0);
        SetConfigValueVector(commandHistory_, std::vector<std::string>());

        if (this->historyOffset_ >= this->maxHistoryLength_)
            this->historyOffset_ = 0;

        while (this->commandHistory_.size() > this->maxHistoryLength_)
        {
            unsigned int index = this->commandHistory_.size() - 1;
            this->commandHistory_.erase(this->commandHistory_.begin() + index);
            ModifyConfigValue(commandHistory_, remove, index);
        }
    }

    void Shell::setInputBuffer(InputBuffer* buffer)
    {
        if (this->inputBuffer_)
        {
            this->inputBuffer_->unregisterListener(this);
            // TODO: may be very dangerous. InputManager already deletes InputBuffer instance!!!
            delete this->inputBuffer_;
        }

        this->inputBuffer_ = buffer;
        this->inputBuffer_->registerListener(this, &Shell::inputChanged, true);
        this->inputBuffer_->registerListener(this, &Shell::execute, '\r', false);
        this->inputBuffer_->registerListener(this, &Shell::hintandcomplete, '\t', true);
        this->inputBuffer_->registerListener(this, &Shell::backspace, '\b', true);
        this->inputBuffer_->registerListener(this, &Shell::deletechar, KeyCode::Delete);
        this->inputBuffer_->registerListener(this, &Shell::exit, (char)27, true);
        this->inputBuffer_->registerListener(this, &Shell::cursor_right, KeyCode::Right);
        this->inputBuffer_->registerListener(this, &Shell::cursor_left, KeyCode::Left);
        this->inputBuffer_->registerListener(this, &Shell::cursor_end, KeyCode::End);
        this->inputBuffer_->registerListener(this, &Shell::cursor_home, KeyCode::Home);
        this->inputBuffer_->registerListener(this, &Shell::history_up, KeyCode::Up);
        this->inputBuffer_->registerListener(this, &Shell::history_down, KeyCode::Down);
        this->inputBuffer_->registerListener(this, &Shell::scroll_up, KeyCode::PageUp);
        this->inputBuffer_->registerListener(this, &Shell::scroll_down, KeyCode::PageDown);
    }

    void Shell::clearShell()
    {
        Shell::getInstance().clearLines();
    }

    void Shell::history()
    {
        Shell& instance = Shell::getInstance();

        for (int i = instance.historyOffset_; i < (int)instance.commandHistory_.size(); ++i)
            instance.addLine(instance.commandHistory_[i], -1);
        for (int i =  0; i < (int)instance.historyOffset_; ++i)
            instance.addLine(instance.commandHistory_[i], -1);
    }

    void Shell::registerListener(ShellListener* listener)
    {
        this->listeners_.insert(this->listeners_.end(), listener);
    }

    void Shell::unregisterListener(ShellListener* listener)
    {
        for (std::list<ShellListener*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); )
        {
            if ((*it) == listener)
                this->listeners_.erase(it++);
            else
                ++it;
        }
    }

    void Shell::setCursorPosition(unsigned int cursor)
    {
        this->inputBuffer_->setCursorPosition(cursor);
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::setInput(const std::string& input)
    {
        this->inputBuffer_->set(input);
        this->inputChanged();
    }

    void Shell::addLine(const std::string& line, int level)
    {
        int original_level = OutputHandler::getOutStream().getOutputLevel();
        OutputHandler::getOutStream().setOutputLevel(level);

        if (!this->finishedLastLine_)
            this->outputBuffer_ << std::endl;

        this->outputBuffer_ << line << std::endl;
        OutputHandler::getOutStream().setOutputLevel(original_level);
    }

    void Shell::clearLines()
    {
        this->lines_.clear();
        this->scrollIterator_ = this->lines_.begin();

        this->scrollPosition_ = 0;
        this->finishedLastLine_ = true;

        SHELL_UPDATE_LISTENERS(linesChanged);
    }

    std::list<std::string>::const_iterator Shell::getNewestLineIterator() const
    {
        if (this->scrollPosition_)
            return this->scrollIterator_;
        else
            return this->lines_.begin();
    }

    std::list<std::string>::const_iterator Shell::getEndIterator() const
    {
        return this->lines_.end();
    }

    void Shell::addToHistory(const std::string& command)
    {
        ModifyConfigValue(commandHistory_, set, this->historyOffset_, command);
        this->historyPosition_ = 0;
        ModifyConfigValue(historyOffset_, set, (this->historyOffset_ + 1) % this->maxHistoryLength_);
    }

    std::string Shell::getFromHistory() const
    {
        unsigned int index = mod(((int)this->historyOffset_) - ((int)this->historyPosition_), this->maxHistoryLength_);
        if (index < this->commandHistory_.size() && this->historyPosition_ != 0)
            return this->commandHistory_[index];
        else
            return "";
    }

    void Shell::outputChanged()
    {
        std::string output;
        bool newline;
        do
        {
            newline = this->outputBuffer_.getLine(&output);

            if (!newline && output == "")
                break;

            if (this->finishedLastLine_)
            {
                if (this->bAddOutputLevel_)
                    output.insert(0, 1, (char)OutputHandler::getOutStream().getOutputLevel());

                this->lines_.insert(this->lines_.begin(), output);

                if (this->scrollPosition_)
                    this->scrollPosition_++;
                else
                    this->scrollIterator_ = this->lines_.begin();

                this->finishedLastLine_ = newline;

                if (!this->scrollPosition_)
                {
                    SHELL_UPDATE_LISTENERS(lineAdded);
                }
            }
            else
            {
                (*this->lines_.begin()) += output;
                this->finishedLastLine_ = newline;
                SHELL_UPDATE_LISTENERS(onlyLastLineChanged);
            }

        } while (newline);
    }

    void Shell::inputChanged()
    {
        SHELL_UPDATE_LISTENERS(inputChanged);
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::execute()
    {
        this->addToHistory(this->inputBuffer_->get());
        this->addLine(this->inputBuffer_->get(), 0);

        if (!CommandExecutor::execute(this->inputBuffer_->get()))
            this->addLine("Error: Can't execute \"" + this->inputBuffer_->get() + "\".", 1);

        this->clear();
    }

    void Shell::hintandcomplete()
    {
        this->inputBuffer_->set(CommandExecutor::complete(this->inputBuffer_->get()));
        this->addLine(CommandExecutor::hint(this->inputBuffer_->get()), -1);

        this->inputChanged();
    }

    void Shell::backspace()
    {
        this->inputBuffer_->removeBehindCursor();
        SHELL_UPDATE_LISTENERS(inputChanged);
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::deletechar()
    {
        this->inputBuffer_->removeAtCursor();
        SHELL_UPDATE_LISTENERS(inputChanged);
    }

    void Shell::clear()
    {
        this->inputBuffer_->clear();
        this->historyPosition_ = 0;
        SHELL_UPDATE_LISTENERS(inputChanged);
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::cursor_right()
    {
        this->inputBuffer_->increaseCursor();
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::cursor_left()
    {
        this->inputBuffer_->decreaseCursor();
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::cursor_end()
    {
        this->inputBuffer_->setCursorToEnd();
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::cursor_home()
    {
        this->inputBuffer_->setCursorToBegin();
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::history_up()
    {
        if (this->historyPosition_ < this->commandHistory_.size())
        {
            this->historyPosition_++;
            this->inputBuffer_->set(this->getFromHistory());
        }
    }

    void Shell::history_down()
    {
        if (this->historyPosition_ > 0)
        {
            this->historyPosition_--;
            this->inputBuffer_->set(this->getFromHistory());
        }
    }

    void Shell::scroll_up()
    {
        if (this->scrollIterator_ != this->lines_.end())
        {
            ++this->scrollIterator_;
            ++this->scrollPosition_;

            SHELL_UPDATE_LISTENERS(linesChanged);
        }
    }

    void Shell::scroll_down()
    {
        if (this->scrollIterator_ != this->lines_.begin())
        {
            --this->scrollIterator_;
            --this->scrollPosition_;

            SHELL_UPDATE_LISTENERS(linesChanged);
        }
    }

    void Shell::exit()
    {
        if (this->inputBuffer_->getSize() > 0)
        {
            this->clear();
            return;
        }

        this->clear();
        this->scrollPosition_ = 0;
        this->scrollIterator_ = this->lines_.begin();

        SHELL_UPDATE_LISTENERS(exit);
    }
}
