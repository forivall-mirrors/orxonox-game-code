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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

#include "core/CoreIncludes.h"

#include "QuestDescription.h"

namespace orxonox {

    CreateFactory(QuestDescription);

    QuestDescription::QuestDescription(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(QuestDescription);

        this->initialize();
    }

    QuestDescription::~QuestDescription()
    {

    }

    void QuestDescription::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(QuestDescription, XMLPort, xmlelement, mode);

        XMLPortParam(QuestDescription, "title", setTitle, getTitle, xmlelement, mode);
        XMLPortParam(QuestDescription, "description", setDescription, getDescription, xmlelement, mode);

        COUT(3) << "New QuestDescription with title '" << this->getTitle() << "' created." << std::endl;
    }

    /**
    @brief
        Initializes the object. Has to be called first in every constructor of this class.
    */
    void QuestDescription::initialize(void)
    {
        RegisterObject(QuestDescription);
    }


}
