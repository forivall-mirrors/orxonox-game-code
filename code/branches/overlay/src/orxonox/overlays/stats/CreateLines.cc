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
 *      Benjamin Hildebrandt
 *
 */

#include "OrxonoxStableHeaders.h"
#include "CreateLines.h"

#include <string>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>

#include "util/Convert.h"
#include "util/Debug.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"

namespace orxonox
{
    CreateFactory(CreateLines);

    /**
        @brief Constructor: Creates and initializes a line.
    */
    CreateLines::CreateLines(BaseObject* creator)
        : OrxonoxOverlay(creator)
    {
        RegisterObject(CreateLines);
    }

    /**
        @brief Initializes the lines.
    */
    void CreateLines::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(CreateLines, XMLPort, xmlElement, mode);
    }

    void createTextArea(unsigned int lineIndex) {

        this->statsOverlayTextAreas_ = static_cast<Ogre::TextAreaOverlayElement*>(ovMan->createOverlayElement("TextArea", "StatsLineTextArea" + getName() + convertToString(lineIndex)));
        this->overlay_->add2D(this->statsOverlayTextAreas_);

        // this->textColumns_.push_back();    pointer to be added

    }

}
