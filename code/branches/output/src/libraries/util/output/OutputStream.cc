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

#include "OutputStream.h"

#include "OutputManager.h"

namespace orxonox
{
    OutputStream::OutputStream()
    {
        this->level_ = level::none;
        this->context_ = context::none;
        this->bAcceptsOutput_ = false;
    }

    void OutputStream::sendMessage()
    {
        OutputManager::getInstanceAndCreateListeners().pushMessage(this->level_, this->context_, this->str());
        this->str("");
    }

    void OutputStream::setOutputAttributes(OutputLevel level, OutputContext context)
    {
        this->level_ = level;
        this->context_ = context;

        this->bAcceptsOutput_ = OutputManager::getInstanceAndCreateListeners().acceptsOutput(this->level_, this->context_);
    }
}
