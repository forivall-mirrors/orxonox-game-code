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

#include "OrxonoxStableHeaders.h"
#include "OverlayText.h"

#include <OgreOverlayManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include "util/Convert.h"
#include "util/String.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(OverlayText);

    OverlayText::OverlayText()
        : text_(0)
    {
        RegisterObject(OverlayText);
    }

    OverlayText::~OverlayText()
    {
        if (this->text_)
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->text_);
    }

    void OverlayText::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        OrxonoxOverlay::XMLPort(xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            this->text_ = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton()
                .createOverlayElement("TextArea", "OverlayText_text_" + getUniqueNumberStr()));
            this->text_->setCharHeight(1.0);

            this->background_->addChild(this->text_);
        }

        XMLPortParam(OverlayText, "font",     setFont,     getFont,     xmlElement, mode).defaultValues("Monofur");
        XMLPortParam(OverlayText, "caption",  setCaption,  getCaption,  xmlElement, mode).defaultValues("");
        XMLPortParam(OverlayText, "textSize", setTextSize, getTextSize, xmlElement, mode).defaultValues(1.0f);
    }

    void OverlayText::setFont(const std::string& font)
    {
        if (this->text_ && font != "")
            this->text_->setFontName(font);
    }

    const std::string& OverlayText::getFont() const
    {
        if (this->text_)
            return this->text_->getFontName();
        else
            return blankString;
    }

    void OverlayText::sizeChanged()
    {
        if (!this->overlay_)
            return;

        if (this->rotState_ == Horizontal)
            this->overlay_->setScale(size_.y * sizeCorrection_.y, size_.y * sizeCorrection_.y);
        else if (this->rotState_ == Vertical)
            this->overlay_->setScale(size_.y / (sizeCorrection_.y * sizeCorrection_.y), size_.y * sizeCorrection_.y);
        else
            this->overlay_->setScale(size_.y, size_.y);

        positionChanged();
    }
}
