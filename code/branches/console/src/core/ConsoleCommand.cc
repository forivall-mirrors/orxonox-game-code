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

#include "ConsoleCommand.h"

namespace orxonox
{
    ConsoleCommand::ConsoleCommand(FunctorStatic* functor, const std::string& name) : ExecutorStatic(functor, name)
    {
        this->accessLevel_ = AccessLevel::None;
        this->autocompletionFunction_[0] = &autocompletion::fallback;
        this->autocompletionFunction_[1] = &autocompletion::fallback;
        this->autocompletionFunction_[2] = &autocompletion::fallback;
        this->autocompletionFunction_[3] = &autocompletion::fallback;
        this->autocompletionFunction_[4] = &autocompletion::fallback;
    }

    ConsoleCommand& ConsoleCommand::setArgumentCompletionList(unsigned int param, std::list<std::pair<std::string, std::string> > (*function) (void))
    {
        if (param < 5)
            this->autocompletionFunction_[param] = function;
        else
        {
            COUT(2) << "Warning: Couldn't add autocompletion-function for param " << param << ": index out of bound." << std::endl;
        }
        return (*this);
    }

    const std::list<std::pair<std::string, std::string> >& ConsoleCommand::getArgumentCompletionList(unsigned int param)
    {
        if (param < 5)
            this->argumentList_ = (*this->autocompletionFunction_[param])();
        else
            this->argumentList_ = autocompletion::fallback();

        return this->argumentList_;
    }

    std::list<std::pair<std::string, std::string> >::const_iterator ConsoleCommand::getArgumentCompletionListBegin(unsigned int param)
    {
        if (param < 5)
            this->argumentList_ = (*this->autocompletionFunction_[param])();
        else
            this->argumentList_ = autocompletion::fallback();

        return this->argumentList_.begin();
    }

    std::list<std::pair<std::string, std::string> >::const_iterator ConsoleCommand::getArgumentCompletionListEnd(unsigned int param)
    {
        if (param < 5)
            this->argumentList_ = (*this->autocompletionFunction_[param])();
        else
            this->argumentList_ = autocompletion::fallback();

        return this->argumentList_.end();
    }
}
