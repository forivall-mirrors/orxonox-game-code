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

#ifndef _Shell_H__
#define _Shell_H__

#include "CorePrereqs.h"

#include <cassert>
#include <list>
#include <string>
#include <vector>

#include "OrxonoxClass.h"
#include "input/InputBuffer.h"
#include "util/OutputBuffer.h"

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
            virtual void exit() {}
    };

    class _CoreExport Shell : virtual public OrxonoxClass, public OutputBufferListener
    {
        public:
            Shell();
            virtual ~Shell();

            static Shell& getInstance() { assert(singletonRef_s); return *singletonRef_s; }

            static void clearShell();
            static void history();

            virtual void setConfigValues();
            void commandHistoryOffsetChanged();
            void commandHistoryLengthChanged();

            void registerListener(ShellListener* listener);
            void unregisterListener(ShellListener* listener);

            inline InputBuffer* getInputBuffer()
                { return this->inputBuffer_; }
            inline OutputBuffer& getOutputBuffer()
                { return this->outputBuffer_; }

            void setCursorPosition(unsigned int cursor);
            inline unsigned int getCursorPosition() const
                { return this->inputBuffer_->getCursorPosition(); }

            void setInput(const std::string& input);

            inline void clearInput()
                { this->setInput(""); }
            inline std::string getInput() const
                { return this->inputBuffer_->get(); }

            std::list<std::string>::const_iterator getNewestLineIterator() const;
            std::list<std::string>::const_iterator getEndIterator() const;

            void addLine(const std::string& line, int level = 0);
            void clearLines();

            inline unsigned int getNumLines() const
                { return this->lines_.size(); }
            inline unsigned int getScrollPosition() const
                { return this->scrollPosition_; }

            inline void addOutputLevel(bool bAddOutputLevel)
                { this->bAddOutputLevel_ = bAddOutputLevel; }

        private:
            Shell(const Shell& other);

            void configureInputBuffer();

            void addToHistory(const std::string& command);
            std::string getFromHistory() const;

            virtual void outputChanged();
            void inputChanged();
            void execute();
            void hintandcomplete();
            void backspace();
            void deletechar();
            void clear();
            void cursor_right();
            void cursor_left();
            void cursor_end();
            void cursor_home();
            void history_up();
            void history_down();
            void scroll_up();
            void scroll_down();
            void exit();

            std::list<ShellListener*> listeners_;
            InputBuffer* inputBuffer_;
            OutputBuffer outputBuffer_;
            bool finishedLastLine_;
            std::list<std::string> lines_;
            std::list<std::string>::const_iterator scrollIterator_;
            unsigned int scrollPosition_;
            std::vector<std::string> commandHistory_;
            unsigned int maxHistoryLength_;
            unsigned int historyPosition_;
            unsigned int historyOffset_;
            bool bAddOutputLevel_;

            static Shell* singletonRef_s;
    };
}

#endif /* _Shell_H__ */
