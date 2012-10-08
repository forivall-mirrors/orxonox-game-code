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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

/**
    @file TetrisCenterpoint.cc
    @brief Implementation of the TetrisCenterpoint class.
    @ingroup Tetris
*/

#include "TetrisCenterpoint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Tetris.h"

namespace orxonox
{
    CreateFactory(TetrisCenterpoint);

    /**
    @brief
        Constructor. Registers and initializes the object and checks whether the gametype is actually Tetris.
    */
    TetrisCenterpoint::TetrisCenterpoint(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(TetrisCenterpoint);

        this->width_ = 10;
        this->height_ = 11;
        this->stoneSize_ = 10.0f;
        this->stoneTemplate_ = "";
        this->brickTemplate_ = "";
        this->stoneSpeed_ = 20.0f;
        
        this->checkGametype();
    }

    /**
    @brief
        Method to create a TetrisCenterpoint through XML.
    */
    void TetrisCenterpoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TetrisCenterpoint, XMLPort, xmlelement, mode);

        XMLPortParam(TetrisCenterpoint, "width", setWidth, getWidth, xmlelement, mode); // die Breite
        XMLPortParam(TetrisCenterpoint, "height", setHeight, getHeight, xmlelement, mode); // die Grösse //was sollte das mit setWidth??
        XMLPortParam(TetrisCenterpoint, "stoneSize", setStoneSize, getStoneSize, xmlelement, mode); 
        XMLPortParam(TetrisCenterpoint, "stoneTemplate", setStoneTemplate, getStoneTemplate, xmlelement, mode);
        XMLPortParam(TetrisCenterpoint, "brickTemplate", setBrickTemplate, getBrickTemplate, xmlelement, mode);
        XMLPortParam(TetrisCenterpoint, "stoneSpeed", setStoneSpeed, getStoneSpeed, xmlelement, mode);
    }

    /**
    @brief
        Is called when the gametype has changed.
    */
    void TetrisCenterpoint::changedGametype()
    {
        SUPER(TetrisCenterpoint, changedGametype);

        // Check, whether it's still Tetris.
        this->checkGametype();
    }

    /**
    @brief
        Checks whether the gametype is Tetris and if it is, sets its centerpoint.
    */
    void TetrisCenterpoint::checkGametype()
    {
        if (this->getGametype() != NULL && this->getGametype()->isA(Class(Tetris)))
        {
            Tetris* tetrisGametype = orxonox_cast<Tetris*>(this->getGametype().get());
            tetrisGametype->setCenterpoint(this);
        }
    }
}
