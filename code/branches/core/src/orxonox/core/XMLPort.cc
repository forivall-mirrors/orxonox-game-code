/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

#include "XMLPort.h"
#include "Language.h"

namespace orxonox
{
    XMLPortParamContainer::XMLPortParamContainer()
    {
        this->bAddedDescription_ = false;
        this->bAddedDefaultValues_ = false;
    }

    XMLPortParamContainer& XMLPortParamContainer::description(const std::string description)
    {
        if (!this->bAddedDescription_)
        {
            this->description_ = std::string("XMLPortParamContainer::" + this->classname_ + "::" + this->paramname_);
            AddLanguageEntry(this->description_, description);
            this->bAddedDescription_ = true;
        }

        return (*this);
    }

    const std::string& XMLPortParamContainer::getDescription()
    {
        return GetLocalisation(this->description_);
    }
}
