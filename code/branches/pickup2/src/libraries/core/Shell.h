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

#ifndef _Shell_H__
#define _Shell_H__

#include "CorePrereqs.h"

#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "util/OutputHandler.h"
#include "OrxonoxClass.h"
#include "ConfigFileManager.h"
#include "input/InputBuffer.h"

namespace orxonox
{
    class _CoreExport ShellListener
    {
        friend class Shell;

        public:
            virtual ~ShellListener() {}

        private:
            virtual void linesChanged() {}
            virtual void onlyLastLineChanged() {}
            virtual void lineAdded() {}
            virtual void inputChanged() {}
            virtual void cursorChanged() {}
            virtual void executed() {}
            virtual void exit() {}
    };


    class _CoreExport Shell : virtual public OrxonoxClass, public OutputListener
    {
        public:
            enum LineType
            {
                None    = OutputLevel::None,
                Warning = OutputLevel::Warning,
                Error   = OutputLevel::Error,
                Info    = OutputLevel::Info,
                Debug   = OutputLevel::Debug,
                Verbose = OutputLevel::Verbose,
                Ultra   = OutputLevel::Ultra,
                Input,
                Command,
                Hint
            };

            Shell(const std::string& consoleName, bool bScrollable);
            ~Shell();

            void setConfigValues();
            void commandHistoryOffsetChanged();
            void commandHistoryLengthChanged();

            void registerListener(ShellListener* listener);
            void unregisterListener(ShellListener* listener);

            inline InputBuffer* getInputBuffer()
                { return this->inputBuffer_; }

            void setCursorPosition(unsigned int cursor);
            inline unsigned int getCursorPosition() const
                { return this->inputBuffer_->getCursorPosition(); }

            inline const std::string& getInput() const
                { return this->inputBuffer_->get(); }

            typedef std::list<std::pair<std::string, LineType> > LineList;
            LineList::const_iterator getNewestLineIterator() const;
            LineList::const_iterator getEndIterator() const;

            void addOutput(const std::string& text, LineType type = None);
            void clearOutput();

            inline unsigned int getNumLines() const
                { return this->outputLines_.size(); }
            inline unsigned int getScrollPosition() const
                { return this->scrollPosition_; }

            inline const std::string& getPromptPrefix() const { return this->promptPrefix_; }
            void setPromptPrefix(const std::string& str);

        private:
            Shell(const Shell& other);

            void addToHistory(const std::string& command);
            const std::string& getFromHistory() const;
            void clearInput();
            // OutputListener
            void outputChanged(int level);

            void configureInputBuffer();

            // InputBuffer callbacks
            void inputChanged();
            void execute();
            void hintAndComplete();
            void backspace();
            void deleteChar();
            void cursorRight();
            void cursorLeft();
            void cursorEnd();
            void cursorHome();
            void historyUp();
            void historyDown();
            void historySearchUp();
            void historySearchDown();
            void scrollUp();
            void scrollDown();
            void exit();

            template <void (ShellListener::*F)()>
            void updateListeners()
            {
                for (std::list<ShellListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); )
                    ((*(it++))->*F)();
            }

            std::list<ShellListener*> listeners_;
            InputBuffer*              inputBuffer_;
            std::stringstream         outputBuffer_;
            bool                      bFinishedLastLine_;
            LineList                  outputLines_;
            LineList::const_iterator  scrollIterator_;
            unsigned int              scrollPosition_;
            unsigned int              historyPosition_;

            std::string               promptPrefix_;
            const std::string         consoleName_;
            const bool                bScrollable_;

            // Config values
            unsigned int              maxHistoryLength_;
            unsigned int              historyOffset_;
            std::vector<std::string>  commandHistory_;
            int                       softDebugLevel_;
    };
}

#endif /* _Shell_H__ */
