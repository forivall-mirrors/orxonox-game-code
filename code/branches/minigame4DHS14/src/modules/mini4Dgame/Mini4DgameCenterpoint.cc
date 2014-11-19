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

/**
    @file Mini4DgameCenterpoint.cc
    @brief Implementation of the Mini4DgameCenterpoint class.
*/

#include "Mini4DgameCenterpoint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "../orxonox/graphics/BlinkingBillboard.h"

#include "Mini4Dgame.h"

namespace orxonox
{

    RegisterClass(Mini4DgameCenterpoint);
    /**
    @brief
        Constructor. Registers and initializes the object and checks whether the gametype is actually Mini4Dgame.
    */
    Mini4DgameCenterpoint::Mini4DgameCenterpoint(Context* context) : StaticEntity(context)
    {
        RegisterObject(Mini4DgameCenterpoint);

        this->width_ = 200;
        this->height_ = 200;
        this->length_ = 200;

        this->checkGametype();
    }

    /**
    @brief
        Method to create a Mini4DgameCenterpoint through XML.
    */
    void Mini4DgameCenterpoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Mini4DgameCenterpoint, XMLPort, xmlelement, mode);

        XMLPortParam(Mini4DgameCenterpoint, "dimension", setFieldDimension, getFieldDimension, xmlelement, mode);
        XMLPortParam(Mini4DgameCenterpoint, "boardtemplate", setBoardtemplate, getBoardtemplate, xmlelement, mode);
    }

    /**
    @brief
        Is called when the gametype has changed.
    */
    void Mini4DgameCenterpoint::changedGametype()
    {
        SUPER(Mini4DgameCenterpoint, changedGametype);

        // Check, whether it's still Mini4Dgame.
        this->checkGametype();
    }

    /**
    @brief
        Checks whether the gametype is Mini4Dgame and if it is, sets its centerpoint.
    */
    void Mini4DgameCenterpoint::checkGametype()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(Mini4Dgame)))
        {
            Mini4Dgame* Mini4DgameGametype = orxonox_cast<Mini4Dgame*>(this->getGametype().get());
            Mini4DgameGametype->setCenterpoint(this);
        }
    }
}
