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
 *      Sven Stucki
 *   Co-authors:
 *      ...
 *
 */

/**
    @file DockingTarget.cc
	@brief Docking system main class
*/

#include "DockingTarget.h"
#include "core/XMLPort.h"


namespace orxonox
{
    CreateFactory(DockingTarget);

    DockingTarget::DockingTarget(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(DockingTarget);
    }

    DockingTarget::~DockingTarget()
    {
    }


    void DockingTarget::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(DockingTarget, XMLPort, xmlelement, mode);
        XMLPortParam(DockingTarget, "name", setName, getName, xmlelement, mode);

        COUT(0) << "DockingTarget with name '" << this->getName() << "' created.." << std::endl;
    }


    void DockingTarget::setName(std::string str) {
        this->name = str;
    }

    std::string DockingTarget::getName() {
        return this->name;
    }
}
