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

#include "ScriptController.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    RegisterClass(ScriptController);

    ScriptController::ScriptController(Context* context, ControllableEntity CE) : ArtificialController(context)
    {
        RegisterObject(ScriptController);
        set_controlled(CE);
    }

    void set_luasrc(std::string lsrc)
    {
        this->luasrc=lsrc;
    }

    void set_controlled(*ControllableEntity toControl)
    {
        this->controlled=toControl;
    }
    
    void ScriptController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
    	XMLPort(ScriptController, BaseObject, "lsrc", set_luasrc, xmlelement, mode);

    }

    void ScriptController::moveToPosition(const Vector3& target)
    {


    }
    /* TO DO
                in the constuctor: make accessible functions such as moveToPosition.. in LUA 
            ->tolua++ example: http://usefulgamedev.weebly.com/tolua-example.html*/

    //function to execute the luafile



}
