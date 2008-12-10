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
        @brief Constructor: Creates a line.
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
    void CreateLines::setNumberOfColumns(unsigned int numberOfColumns, unsigned int lineIndex) {

        Ogre::OverlayManager* ovMan = Ogre::OverlayManager::getSingletonPtr();

        unsigned int colIndex = 0;

        while (textColumns_.size() < numberOfColumns) {

            Ogre::TextAreaOverlayElement* tempTextArea = static_cast<Ogre::TextAreaOverlayElement*>(ovMan->createOverlayElement("TextArea", "StatsLineTextArea" + getName() + convertToString(lineIndex) + convertToString(colIndex)));
            textColumns_.push_back(tempTextArea);
            this->background_->addChild(tempTextArea);

            colIndex++;

        }

        while (textColumns_.size() > numberOfColumns) {

            this->background_->_removeChild(textColumns_.back());
            ovMan->destroyOverlayElement(textColumns_.back());
            textColumns_.pop_back();

        }

    }

    void CreateLines::alignColumn(int columnIndex, float leftOffset, float topOffset) {

        this->textColumns_[columnIndex]->setPosition(leftOffset, topOffset);

    }

    void CreateLines::setColumnText(int columnIndex, std::string columnText) {

        this->textColumns_[columnIndex]->setCaption(columnText);

    }

}
