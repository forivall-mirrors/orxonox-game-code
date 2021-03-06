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

#include "Controller.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    RegisterUnloadableClass(Controller);

    Controller::Controller(Context* context) : BaseObject(context)
    {
        RegisterObject(Controller);

        this->player_ = 0;
        this->controllableEntity_ = 0;
        this->bGodMode_ = false;
        this->team_=-1;
    }

    Controller::~Controller()
    {
    }
    void Controller::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Controller, XMLPort, xmlelement, mode);
        XMLPortParam(Controller, "team", setTeam, getTeam, xmlelement, mode).defaultValues(-1);
    }

}
