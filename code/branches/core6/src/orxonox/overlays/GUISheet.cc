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
#include "core/GameMode.h"

namespace orxonox
{
    CreateFactory(GUISheet);

    GUISheet::GUISheet(Context* context)
        : BaseObject(context)
        , bShowOnLoad_(false)
        , bHidePrevious_(false)
        , bHidePreviousSet_(false)
    {
        RegisterObject(GUISheet);

        if (!GameMode::showsGraphics())
            ThrowException(NoGraphics, "GUISheet construction failed: no graphics");
    }

    GUISheet::~GUISheet()
    {
    }

    void GUISheet::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(GUISheet, XMLPort, xmlelement, mode);

        XMLPortParam(GUISheet, "showOnLoad",   setShowOnLoad,     getShowOnLoad,     xmlelement, mode);
        XMLPortParam(GUISheet, "hidePrevious", setPreviousHiding, getPreviousHiding, xmlelement, mode);
        XMLPortParam(GUISheet, "sheetName",    setSheetName,      getSheetName,      xmlelement, mode);
        XMLPortParam(GUISheet, "backgroundImage",  setBackgroundImage,  getBackgroundImage,  xmlelement, mode);

        if (this->bShowOnLoad_)
            this->show();
    }

    void GUISheet::show()
    {
        GUIManager::getInstance().setBackgroundImage(this->backgroundImage_);
        if (this->bHidePreviousSet_)
            GUIManager::getInstance().showGUI(this->sheetName_, this->bHidePrevious_);
        else
            GUIManager::getInstance().showGUI(this->sheetName_);
    }

    void GUISheet::hide()
    {
        GUIManager::getInstance().hideGUI(this->sheetName_);
        if (!this->backgroundImage_.empty())
            GUIManager::getInstance().setBackgroundImage("");
    }

    void GUISheet::setSheetName(const std::string& name)
    {
        this->sheetName_ = name;
        GUIManager::getInstance().loadGUI(this->sheetName_);
    }

    void GUISheet::setPreviousHiding(bool bHide)
    {
        this->bHidePrevious_ = bHide;
        this->bHidePreviousSet_ = true;
        // Note: This call has no effect when already showing!
    }
}
