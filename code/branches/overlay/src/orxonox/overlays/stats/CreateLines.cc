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

    /**
        @brief Ensures that the number of OverlayElements is equal to numberOfColumns.
    */
    void setNumberOfColumns(unsigned int numberOfColumns) {

        Ogre::OverlayManager* ovMan = Ogre::OverlayManager::getSingletonPtr();

        while (textColumns_.getSize() < numberOfColumns) {

            tempTextArea* = static_cast<Ogre::TextAreaOverlayElement*>(ovMan->createOverlayElement("TextArea", "StatsLineTextArea" + getName() + convertToString(lineIndex)));
            textColumns_->add2D(tempTextArea);
            textColumns_.push_back(tempTextArea);

        }

        while (textColumns_.getSize() > numberOfColumns) {

            ovMan->destroy(textColumns_.back());
            textColumns_.pop_back();

        }

    }

    void alignColumn(int columnIndex, float leftOffset) {

        this->textColumns_[columnIndex]->setPosition(leftOffset,0);

    }

    void setColumnText(int columnIndex, string columnText) {

        this->textColumns_[columnIndex]->setCaption(columnText);

    }

}
