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
    @defgroup ShellConsole Shell and console
    @ingroup Command
*/

/**
    @file
    @ingroup Command ShellConsole
    @brief Declaration of the Shell and ShellListener classes.
*/

#ifndef _Shell_H__
#define _Shell_H__

#include "core/CorePrereqs.h"

#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "util/output/BaseWriter.h"
#include "core/CoreConfig.h"

namespace orxonox
{
    /**
        @brief An interface, used to get a notification if the state of the Shell changes.
    */
    class _CoreExport ShellListener
    {
        friend class Shell;

        public:
            virtual ~ShellListener() {}

        private:
            virtual void linesChanged() {}          ///< Called if all output-lines have changed
            virtual void lineAdded() {}             ///< Called if a new line was added to the output
            virtual void inputChanged() {}          ///< Called if the input has changed
            virtual void cursorChanged() {}         ///< Called if the cursor in the input line has changed
            virtual void executed() {}              ///< Called if a command from the input line was executed
            virtual void exit() {}                  ///< Called if the console should be closed
    };


    /**
        @brief The Shell is the logical component of the console that displays output to the user and allows him to enter commands.

        The Shell gathers output sent from OutputManager by inheriting from BaseWriter.
        The output-lines are stored in the shell, so they can be displayed in a graphical
        console. Additionally the Shell has an InputBuffer which is needed by the user to
        enter commands.

        Different graphical consoles build upon a Shell, for example InGameConsole and IOConsole.
    */
    class _CoreExport Shell : public BaseWriter, public DevModeListener
    {
        public:
            /// Defines the type of a line of text in the Shell - some types depend on the output level, others are of internal use.
            enum LineType
            {
                DebugOutput     = debug_output,
                Message         = message,
                UserError       = user_error,
                UserWarning     = user_warning,
                UserStatus      = user_status,
                UserInfo        = user_info,
                InternalError   = internal_error,
                InternalWarning = internal_warning,
                InternalStatus  = internal_status,
                InternalInfo    = internal_info,
                Verbose         = verbose,
                VerboseMore     = verbose_more,
                VerboseUltra    = verbose_ultra,
                Cout,
                Input,
                Command,
                Result,
                Hint
            };

            Shell(const std::string& consoleName = "", bool bScrollable = true);
            ~Shell();

            void setConfigValues();
            void commandHistoryOffsetChanged();
            void commandHistoryLengthChanged();

            void registerListener(ShellListener* listener);
            void unregisterListener(ShellListener* listener);

            /// Returns the input buffer which is needed by the user to enter text into the shell.
            inline InputBuffer* getInputBuffer()
                { return this->inputBuffer_; }

            void setCursorPosition(unsigned int cursor);
            unsigned int getCursorPosition() const;

            const std::string& getInput() const;

            typedef std::list<std::pair<std::string, LineType> > LineList;
            LineList::const_iterator getNewestLineIterator() const;
            LineList::const_iterator getEndIterator() const;

            void addOutput(const std::string& text, LineType type = DebugOutput);
            void addLine(const std::string& line, LineType type = DebugOutput);
            void clearOutput();

            /// Returns the number of output-lines that are displayed in the shell.
            inline unsigned int getNumLines() const
                { return this->outputLines_.size(); }
            /// Returns the line which is currently viewed if the user scrolls through the older output-lines in the shell.
            inline unsigned int getScrollPosition() const
                { return this->scrollPosition_; }

            /// Returns the cache size that is actually used in CommandExecutor, but placed here for better readability of the config file.
            static inline unsigned int getCacheSize()
                { return Shell::cacheSize_s; }

        private:
            Shell(const Shell& other);

            // DevModeListener
            void devModeChanged(bool value);

            void addToHistory(const std::string& command);
            const std::string& getFromHistory() const;
            void clearInput();
            // BaseWriter
            virtual void printLine(const std::string& line, OutputLevel level);

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

            /// Iterates through all registered @ref ShellListener "shell listeners" and calls the function @a F.
            template <void (ShellListener::*F)()>
            void updateListeners()
            {
                for (std::list<ShellListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); )
                    ((*(it++))->*F)();
            }

            std::list<ShellListener*> listeners_;           ///< The registered shell listeners
            InputBuffer*              inputBuffer_;         ///< The input buffer that is needed by the user to enter text
            LineList                  outputLines_;         ///< A list of all output-lines that were displayed in the shell so far
            LineList::const_iterator  scrollIterator_;      ///< An iterator to an entry of the list of output-lines, changes if the user scrolls through the output in the shell
            unsigned int              scrollPosition_;      ///< The number of the line that is currently being referenced by scrollIterator_
            unsigned int              historyPosition_;     ///< If the user scrolls through the history of entered commands (stored in commandHistory_), this contains the currently viewed history entry
            const bool                bScrollable_;         ///< If true, the user can scroll through the output-lines

            // Config values
            unsigned int              maxHistoryLength_;    ///< The maximum number of saved commands
            unsigned int              historyOffset_;       ///< The command history is a circular buffer, this variable defines the current write-offset
            std::vector<std::string>  commandHistory_;      ///< The history of commands that were entered by the user
            static unsigned int       cacheSize_s;          ///< The maximum cache size of the CommandExecutor - this is stored here for better readability of the config file and because CommandExecutor is not configurable
    };
}

#endif /* _Shell_H__ */
