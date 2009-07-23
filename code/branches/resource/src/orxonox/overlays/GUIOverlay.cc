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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

#include "GUIOverlay.h"

#include <string>
#include <sstream>

#include "core/input/InputManager.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "gui/GUIManager.h"

namespace orxonox
{
    CreateFactory(GUIOverlay);

    GUIOverlay::GUIOverlay(BaseObject* creator) : OrxonoxOverlay(creator)
    {
        RegisterObject(GUIOverlay);
    }

    GUIOverlay::~GUIOverlay()
    {
    }

    void GUIOverlay::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(GUIOverlay, XMLPort, xmlElement, mode);

        XMLPortParam(GUIOverlay, "guiname", setGUIName, getGUIName, xmlElement, mode);
    }

    void GUIOverlay::changedVisibility()
    {
        SUPER(GUIOverlay, changedVisibility);

        if (this->isVisible())
        {
            std::string str;
            std::stringstream out;
            out << reinterpret_cast<long>(this);
            str = out.str();
            GUIManager::getInstance().executeCode("showCursor()");
            InputManager::getInstance().enterState("guiMouseOnly");
            GUIManager::getInstance().executeCode("showGUI(\"" + this->guiName_ + "\", " + str + ")");
        }
        else
        {
            GUIManager::getInstance().executeCode("hideGUI(\"" + this->guiName_ + "\")");
            GUIManager::getInstance().executeCode("hideCursor()");
            InputManager::getInstance().leaveState("guiMouseOnly");
        }
    }

}
