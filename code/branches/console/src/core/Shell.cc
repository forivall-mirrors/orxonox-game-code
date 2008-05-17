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

#define SHELL_UPDATE_LISTENERS(function) \
    for (std::list<ShellListener*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it) \
        (*it)->function()

namespace orxonox
{
    Shell::Shell()
    {
        RegisterRootObject(Shell);

        this->scrollPosition_ = 0;
        this->maxHistoryLength_ = 100;
        this->historyPosition_ = 0;
        this->historyOffset_ = 0;

        this->clearLines();
/*
        this->inputBuffer_.registerListener(this, &Shell::inputChanged, true);
        this->inputBuffer_.registerListener(this, &Shell::execute, '\r', false);
        this->inputBuffer_.registerListener(this, &Shell::hintandcomplete, '\t', true);
        this->inputBuffer_.registerListener(this, &Shell::backspace, '\b', true);
        this->inputBuffer_.registerListener(this, &Shell::deletechar, OIS::KC_DELETE);
        this->inputBuffer_.registerListener(this, &Shell::exit, (char)27, true);
        this->inputBuffer_.registerListener(this, &Shell::cursor_right, OIS::KC_RIGHT);
        this->inputBuffer_.registerListener(this, &Shell::cursor_left, OIS::KC_LEFT);
        this->inputBuffer_.registerListener(this, &Shell::cursor_end, OIS::KC_END);
        this->inputBuffer_.registerListener(this, &Shell::cursor_home, OIS::KC_HOME);
        this->inputBuffer_.registerListener(this, &Shell::history_up, OIS::KC_UP);
        this->inputBuffer_.registerListener(this, &Shell::history_down, OIS::KC_DOWN);
        this->inputBuffer_.registerListener(this, &Shell::scroll_up, OIS::KC_PGUP);
        this->inputBuffer_.registerListener(this, &Shell::scroll_down, OIS::KC_PGDOWN);
*/
        this->setConfigValues();
    }

    Shell& Shell::getInstance()
    {
        static Shell instance;
        return instance;
    }

    void Shell::setConfigValues()
    {
        SetConfigValue(maxHistoryLength_, 100);
        SetConfigValue(historyOffset_, 0);
        SetConfigValueVector(commandHistory_, std::vector<std::string>(1, ""));

        if (this->historyOffset_ >= this->maxHistoryLength_)
            this->historyOffset_ = 0;

        while (this->commandHistory_.size() > this->maxHistoryLength_)
        {
            unsigned int index = this->commandHistory_.size() - 1;
            this->commandHistory_.erase(this->commandHistory_.begin() + index);
            ModifyConfigValue(commandHistory_, remove, index);
        }
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
        this->inputBuffer_.setCursorPosition(cursor);
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::setInput(const std::string& input)
    {
        this->inputBuffer_.set(input);
        this->inputChanged();
    }

    void Shell::addLine(const std::string& line, unsigned int level)
    {
        if ((*this->lines_.begin()) != "")
        {
            orxonox::OutputHandler::getOutStream().setOutputLevel(level) << std::endl;
        }
        orxonox::OutputHandler::getOutStream().setOutputLevel(level) << line << std::endl;
    }

    void Shell::clearLines()
    {
        this->lines_.clear();
        this->lines_.insert(this->lines_.begin(), "");
        this->scrollIterator_ = this->lines_.begin();

        this->scrollPosition_ = 0;

        SHELL_UPDATE_LISTENERS(linesChanged);
    }

    std::list<std::string>::const_iterator Shell::getNewestLineIterator() const
    {
        if (this->scrollPosition_)
        {
            return this->scrollIterator_;
        }
        else
        {
            if ((*this->lines_.begin()) == "" && this->lines_.size() > 1)
                return (++this->lines_.begin());
            else
                return this->lines_.begin();
        }
    }

    std::list<std::string>::const_iterator Shell::getEndIterator() const
    {
        return this->lines_.end();
    }

    void Shell::addToHistory(const std::string& command)
    {
        this->historyOffset_ = (this->historyOffset_ + 1) % this->maxHistoryLength_;
        ModifyConfigValue(commandHistory_, set, this->historyOffset_, command);
        this->commandHistory_[this->historyOffset_] = command;
        this->historyPosition_ = 0;
    }

    std::string Shell::getFromHistory() const
    {
        return this->commandHistory_[(this->historyOffset_ - this->historyPosition_) % this->maxHistoryLength_];
    }

    void Shell::outputChanged()
    {
        std::string output;
        while (this->outputBuffer_.getLine(&output))
        {
            bool newline = false;
            if ((*this->lines_.begin()) == "")
                newline = true;

            (*this->lines_.begin()) += output;

            SHELL_UPDATE_LISTENERS(onlyLastLineChanged);

            this->lines_.insert(this->lines_.begin(), "");

            if (this->scrollPosition_)
                this->scrollPosition_++;
            else
                this->scrollIterator_ = this->lines_.begin();

            if (newline)
            {
                SHELL_UPDATE_LISTENERS(lineAdded);
            }
        }

        (*this->lines_.begin()) += output;
        SHELL_UPDATE_LISTENERS(onlyLastLineChanged);
    }

    void Shell::inputChanged()
    {
        SHELL_UPDATE_LISTENERS(inputChanged);
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::execute()
    {
        if (CommandExecutor::execute(this->inputBuffer_.get()))
            this->addLine(this->inputBuffer_.get(), 0);
        else
            this->addLine("Error: Can't execute \"" + this->inputBuffer_.get() + "\".", 1);

        this->clear();
    }

    void Shell::hintandcomplete()
    {
        this->addLine(CommandExecutor::hint(this->inputBuffer_.get()), 0);
        this->inputBuffer_.set(CommandExecutor::complete(this->inputBuffer_.get()));

        this->inputChanged();
    }

    void Shell::backspace()
    {
        this->inputBuffer_.removeBehindCursor();
        SHELL_UPDATE_LISTENERS(inputChanged);
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::deletechar()
    {
        this->inputBuffer_.removeAtCursor();
        SHELL_UPDATE_LISTENERS(inputChanged);
    }

    void Shell::clear()
    {
        this->inputBuffer_.clear();
        SHELL_UPDATE_LISTENERS(inputChanged);
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::cursor_right()
    {
        this->inputBuffer_.increaseCursor();
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::cursor_left()
    {
        this->inputBuffer_.decreaseCursor();
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::cursor_end()
    {
        this->inputBuffer_.setCursorToEnd();
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::cursor_home()
    {
        this->inputBuffer_.setCursorToBegin();
        SHELL_UPDATE_LISTENERS(cursorChanged);
    }

    void Shell::history_up()
    {
        if (this->historyPosition_ < (this->commandHistory_.size() - 1))
        {
            this->historyPosition_++;
            this->inputBuffer_.set(this->getFromHistory());
        }
    }

    void Shell::history_down()
    {
        if (this->historyPosition_ > 0)
        {
            this->historyPosition_++;
            this->inputBuffer_.set(this->getFromHistory());
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
        if (this->inputBuffer_.getSize() > 0)
        {
            this->clear();
            return;
        }

        this->clear();
        SHELL_UPDATE_LISTENERS(exit);
    }
}
