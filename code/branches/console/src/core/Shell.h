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

#include "OrxonoxClass.h"
#include "InputBuffer.h"
#include "OutputBuffer.h"

namespace orxonox
{
    class ShellListener
    {
        virtual void outputChanged() = 0;
        virtual void lastLineChanged() = 0;
        virtual void inputChanged() = 0;
        virtual void exit() = 0;
    }

    class _CoreExport Shell : virtual public OrxonoxClass, public InputBufferListener, public OutputBufferListener
    {
        public:
            static Shell& getInstance();

            virtual void setConfigValues();

        private:
            Shell();
            Shell(const Shell& other);
            ~Shell() {}

            virtual void outputChanged();
            void inputChanged();
            void execute();
            void hintandcomplete();
            void backspace();
            void cursor_right();
            void cursor_left();
            void history_up();
            void history_down();
            void scroll_up();
            void scroll_down();
            void exit();

            InputBuffer inputBuffer_;
            OutputBuffer OutputBuffer_;
            std::list<std::string> lines_;
            unsigned int cursor_;
            std::vector<std::string> commandHistory_;
            unsigned int maxHistoryLength_;
    };
}

#endif /* _Shell_H__ */
