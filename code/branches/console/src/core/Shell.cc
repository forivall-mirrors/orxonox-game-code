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

#include "CorePrereqs.h"

#include "OrxonoxClass.h"
#include "InputBuffer.h"
#include "OutputBuffer.h"

namespace orxonox
{
    Shell::Shell()
    {
        this->inputBuffer_.registerListener(this, &Shell::inputChanged, true);
        this->inputBuffer_.registerListener(this, &Shell::execute, '\r', false);
        this->inputBuffer_.registerListener(this, &Shell::hintandcomplete, '\t', true);
        this->inputBuffer_.registerListener(this, &Shell::clear, '§', true);
        this->inputBuffer_.registerListener(this, &Shell::backspace, '\b', true);
        this->inputBuffer_.registerListener(this, &Shell::exit, (char)27, true);
        this->inputBuffer_.registerListener(this, &Shell::cursor_right, OIS::KC_RIGHT);
        this->inputBuffer_.registerListener(this, &Shell::cursor_left, OIS::KC_LEFT);
        this->inputBuffer_.registerListener(this, &Shell::history_up, OIS::KC_UP);
        this->inputBuffer_.registerListener(this, &Shell::history_down, OIS::KC_DOWN);
        this->inputBuffer_.registerListener(this, &Shell::scroll_up, OIS::KC_PGUP);
        this->inputBuffer_.registerListener(this, &Shell::scroll_down, OIS::KC_PGDOWN);
    }

    Shell& Shell::getInstance()
    {
        static instance Shell;
        return instance;
    }

    void Shell::setConfigValues()

    void Shell::outputChanged()
    void Shell::inputChanged()
    void Shell::execute()
    void Shell::hintandcomplete()
    void Shell::backspace()
    void Shell::clear()
    void Shell::cursor_right()
    void Shell::cursor_left()
    void Shell::history_up()
    void Shell::history_down()
    void Shell::scroll_up()
    void Shell::scroll_down()
    void Shell::exit()
}
