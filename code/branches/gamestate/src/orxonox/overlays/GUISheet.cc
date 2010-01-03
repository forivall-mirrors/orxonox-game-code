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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "GUISheet.h"

#include "core/CoreIncludes.h"
#include "core/GUIManager.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(GUISheet);

    GUISheet::GUISheet(BaseObject* creator)
        : BaseObject(creator)
        , bShowOnLoad_(false)
        , bShowCursor_(true)
        , bHidePrevious_(false)
    {
        RegisterObject(GUISheet);

        if (!GameMode::showsGraphics())
            ThrowException(NoGraphics, "GUISheet construction failed: no graphics");
    }

    GUISheet::~GUISheet()
    {
    }

    void GUISheet::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(GUISheet, XMLPort, xmlElement, mode);

        XMLPortParam(GUISheet, "showOnLoad",   setShowOnLoad,       getShowOnLoad,       xmlElement, mode);
        XMLPortParam(GUISheet, "showCursor",   setCursorVisibility, getCursorVisibility, xmlElement, mode);
        XMLPortParam(GUISheet, "hidePrevious", setPreviousHiding,   getPreviousHiding,   xmlElement, mode);
        XMLPortParam(GUISheet, "script",       setScript,           getScript,           xmlElement, mode);
    }

    void GUISheet::show()
    {
        GUIManager::showGUI(this->script_, this->bHidePrevious_, this->bShowCursor_);
    }

    void GUISheet::hide()
    {
        GUIManager::hideGUI(this->script_);
    }

    void GUISheet::setScript(const std::string& filename)
    {
        this->script_ = filename;
        GUIManager::getInstance().loadGUI(this->script_);
        if (this->bShowOnLoad_)
            this->show();
    }

    void GUISheet::setCursorVisibility(bool bShow)
    {
        this->bShowCursor_ = bShow;
        // TODO: This call has no effect when already showing!
    }

    void GUISheet::setPreviousHiding(bool bHide)
    {
        this->bHidePrevious_ = bHide;
        // TODO: This call has no effect when already showing!
    }
}
