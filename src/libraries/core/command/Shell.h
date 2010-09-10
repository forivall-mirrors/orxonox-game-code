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

#include "util/OutputHandler.h"
#include "core/OrxonoxClass.h"
#include "core/input/InputBuffer.h"

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
            virtual void onlyLastLineChanged() {}   ///< Called if only the last output-line has changed
            virtual void lineAdded() {}             ///< Called if a new line was added to the output
            virtual void inputChanged() {}          ///< Called if the input has changed
            virtual void cursorChanged() {}         ///< Called if the cursor in the input line has changed
            virtual void executed() {}              ///< Called if a command from the input line was executed
            virtual void exit() {}                  ///< Called if the console should be closed
    };


    /**
        @brief The Shell is the logical component of the console that displays output to the user and allows him to enter commands.

        The Shell gathers output sent from OutputHandler by inheriting from OutputListener.
        The output-lines are stored in the shell, so they can be displayed in a graphical
        console. Additionally the Shell has an InputBuffer which is needed by the user to
        enter commands.

        Different graphical consoles build upon a Shell, for example InGameConsole and IOConsole.
    */
    class _CoreExport Shell : virtual public OrxonoxClass, public OutputListener
    {
        public:
            /// Defines the type of a line of text in the Shell - some types depend on the output level, others are of internal use.
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

            /// Returns the input buffer which is needed by the user to enter text into the shell.
            inline InputBuffer* getInputBuffer()
                { return this->inputBuffer_; }

            void setCursorPosition(unsigned int cursor);
            /// Returns the current position of the cursor in the input buffer.
            inline unsigned int getCursorPosition() const
                { return this->inputBuffer_->getCursorPosition(); }

            /// Returns the current content of the input buffer (the text which was entered by the user)
            inline const std::string& getInput() const
                { return this->inputBuffer_->get(); }

            typedef std::list<std::pair<std::string, LineType> > LineList;
            LineList::const_iterator getNewestLineIterator() const;
            LineList::const_iterator getEndIterator() const;

            void addOutput(const std::string& text, LineType type = None);
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

            /// Iterates through all registered @ref ShellListener "shell listeners" and calls the function @a F.
            template <void (ShellListener::*F)()>
            void updateListeners()
            {
                for (std::list<ShellListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); )
                    ((*(it++))->*F)();
            }

            std::list<ShellListener*> listeners_;           ///< The registered shell listeners
            InputBuffer*              inputBuffer_;         ///< The input buffer that is needed by the user to enter text
            std::stringstream         outputBuffer_;        ///< The output buffer that is used to retrieve lines of output from OutputListener
            bool                      bFinishedLastLine_;   ///< Stores if the most recent output-line was terminated with a line-break or if more output is expected for this line
            LineList                  outputLines_;         ///< A list of all output-lines that were displayed in the shell so far
            LineList::const_iterator  scrollIterator_;      ///< An iterator to an entry of the list of output-lines, changes if the user scrolls through the output in the shell
            unsigned int              scrollPosition_;      ///< The number of the line that is currently being referenced by scrollIterator_
            unsigned int              historyPosition_;     ///< If the user scrolls through the history of entered commands (stored in commandHistory_), this contains the currently viewed history entry

            const std::string         consoleName_;         ///< The name of this shell - used to define the name of the soft-debug-level config-value
            const bool                bScrollable_;         ///< If true, the user can scroll through the output-lines

            // Config values
            unsigned int              maxHistoryLength_;    ///< The maximum number of saved commands
            unsigned int              historyOffset_;       ///< The command history is a circular buffer, this variable defines the current write-offset
            std::vector<std::string>  commandHistory_;      ///< The history of commands that were entered by the user
            int                       softDebugLevel_;      ///< The maximum level of output that is displayed in the shell (will be passed to OutputListener to filter output)
            static unsigned int       cacheSize_s;          ///< The maximum cache size of the CommandExecutor - this is stored here for better readability of the config file and because CommandExecutor is no OrxonoxClass
    };
}

#endif /* _Shell_H__ */
