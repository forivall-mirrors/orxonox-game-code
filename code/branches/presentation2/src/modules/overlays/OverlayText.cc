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

#include "OverlayText.h"

#include <OgreOverlayManager.h>
#include <OgrePanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <boost/static_assert.hpp>

#include "util/StringUtils.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"


namespace orxonox
{
    CreateFactory(OverlayText);

    BOOST_STATIC_ASSERT((int)Ogre::TextAreaOverlayElement::Left   == (int)OverlayText::Left);
    BOOST_STATIC_ASSERT((int)Ogre::TextAreaOverlayElement::Center == (int)OverlayText::Center);
    BOOST_STATIC_ASSERT((int)Ogre::TextAreaOverlayElement::Right  == (int)OverlayText::Right);

    OverlayText::OverlayText(BaseObject* creator)
        : OrxonoxOverlay(creator)
    {
        RegisterObject(OverlayText);

        this->text_ = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton()
            .createOverlayElement("TextArea", "OverlayText_text_" + getUniqueNumberString()));
        this->text_->setCharHeight(1.0);

        this->setFont("Monofur");
        this->setColour(ColourValue(1.0, 1.0, 1.0, 1.0));
        this->setCaption("");
        this->setTextSize(1.0f);
        this->setAlignmentString("left");

        this->background_->addChild(this->text_);
    }

    OverlayText::~OverlayText()
    {
        if (this->isInitialized())
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->text_);
    }

    void OverlayText::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(OverlayText, XMLPort, xmlElement, mode);

        XMLPortParam(OverlayText, "font",       setFont,            getFont,            xmlElement, mode);
        XMLPortParam(OverlayText, "colour",     setColour,          getColour,          xmlElement, mode);
        XMLPortParam(OverlayText, "caption",    setCaption,         getCaption,         xmlElement, mode);
        XMLPortParam(OverlayText, "textsize",   setTextSize,        getTextSize,        xmlElement, mode);
        XMLPortParam(OverlayText, "align",      setAlignmentString, getAlignmentString, xmlElement, mode);
        XMLPortParam(OverlayText, "spacewidth", setSpaceWidth,      getSpaceWidth,      xmlElement, mode);
    }

    void OverlayText::setAlignmentString(const std::string& alignment)
    {
        if (alignment == "right")
            this->setAlignment(OverlayText::Right);
        else if (alignment == "center")
            this->setAlignment(OverlayText::Center);
        else // "left" and default
            this->setAlignment(OverlayText::Left);
    }

    std::string OverlayText::getAlignmentString() const
    {
        Ogre::TextAreaOverlayElement::Alignment alignment = this->text_->getAlignment();

        switch (alignment)
        {
            case Ogre::TextAreaOverlayElement::Right:
                return "right";
            case Ogre::TextAreaOverlayElement::Center:
                return "center";
            case Ogre::TextAreaOverlayElement::Left:
                return "left";
            default:
                assert(false); return "";
        }
    }

    void OverlayText::sizeChanged()
    {
        if (this->rotState_ == Horizontal)
            this->overlay_->setScale(size_.y * sizeCorrection_.y, size_.y * sizeCorrection_.y);
        else if (this->rotState_ == Vertical)
            this->overlay_->setScale(size_.y / (sizeCorrection_.y * sizeCorrection_.y), size_.y * sizeCorrection_.y);
        else
            this->overlay_->setScale(size_.y, size_.y);

        positionChanged();
    }

    void OverlayText::setCaption(const std::string& caption)
    {
        this->text_->setCaption(caption);
        this->changedCaption();
    }
    std::string OverlayText::getCaption() const
    {
        return this->text_->getCaption();
    }

    void OverlayText::setFont(const std::string& font)
    {
        if (!font.empty())
            this->text_->setFontName(font);
    }
    const std::string& OverlayText::getFont() const
    {
        return this->text_->getFontName();
    }

    void OverlayText::setSpaceWidth(float width)
    {
        this->text_->setSpaceWidth(width);
    }
    float OverlayText::getSpaceWidth() const
    {
        return this->text_->getSpaceWidth();
    }

    void OverlayText::setColour(const ColourValue& colour)
    {
        this->text_->setColour(colour); this->changedColour();
    }
    const ColourValue& OverlayText::getColour() const
    {
        return this->text_->getColour();
    }

    void OverlayText::setAlignment(OverlayText::Alignment alignment)
    {
        this->text_->setAlignment(static_cast<Ogre::TextAreaOverlayElement::Alignment>(alignment));
    }
    OverlayText::Alignment OverlayText::getAlignment() const
    {
        return static_cast<OverlayText::Alignment>(this->text_->getAlignment());
    }
}
