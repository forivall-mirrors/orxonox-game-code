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

#include "HUDHealthBar.h"

#include <OgreOverlayManager.h>
#include <OgrePanelOverlayElement.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/worldentities/pawns/Pawn.h"
#include "objects/items/Engine.h"
#include "overlays/OverlayGroup.h"
#include "util/Convert.h"

namespace orxonox
{
    CreateFactory(HUDHealthBar);

    HUDHealthBar::HUDHealthBar(BaseObject* creator) : HUDBar(creator)
    {
        RegisterObject(HUDHealthBar);

        this->owner_ = 0;
        this->bUseBarColour_ = false;

        this->textoverlay_ = new OverlayText(this);

        assert(this->textoverlay_);

        this->textoverlay_->setCaption("");
    }

    HUDHealthBar::~HUDHealthBar()
    {
        if (this->isInitialized())
            delete this->textoverlay_;
    }

    void HUDHealthBar::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HUDHealthBar, XMLPort, xmlelement, mode);

        XMLPortParam(HUDHealthBar, "showtext",          setTextVisible,          getTextVisible,          xmlelement, mode).defaultValues(true);
        XMLPortParam(HUDHealthBar, "textfont",          setTextFont,             getTextFont,             xmlelement, mode).defaultValues("Monofur");
        XMLPortParam(HUDHealthBar, "textusebarcolour",  setTextUseBarColour,     getTextUseBarColour,     xmlelement, mode).defaultValues(false);
        XMLPortParam(HUDHealthBar, "textcolour",        setTextColour,           getTextColour,           xmlelement, mode).defaultValues(ColourValue(1.0, 1.0, 1.0, 1.0));
        XMLPortParam(HUDHealthBar, "textsize",          setTextSize,             getTextSize,             xmlelement, mode).defaultValues(1.0f);
        XMLPortParam(HUDHealthBar, "textalign",         setTextAlignmentString,  getTextAlignmentString,  xmlelement, mode).defaultValues("left");
        XMLPortParam(HUDHealthBar, "textoffset",        setTextOffset,           getTextOffset,           xmlelement, mode).defaultValues(Vector2::ZERO);
        XMLPortParam(HUDHealthBar, "textpickpoint",     setTextPickPoint,        getTextPickPoint,        xmlelement, mode).defaultValues(Vector2::ZERO);
        XMLPortParam(HUDHealthBar, "textrotation",      setTextRotation,         getTextRotation,         xmlelement, mode).defaultValues(0.0f);
        XMLPortParam(HUDHealthBar, "textcorrectaspect", setTextAspectCorrection, getTextAspectCorrection, xmlelement, mode).defaultValues(true);
        XMLPortParam(HUDHealthBar, "textspacewidth",    setTextSpaceWidth,       getTextSpaceWidth,       xmlelement, mode).defaultValues(true);
    }

    void HUDHealthBar::tick(float dt)
    {
        SUPER(HUDHealthBar, tick, dt);

        if (this->owner_)
        {
            this->setValue(this->owner_->getHealth() / this->owner_->getInitialHealth());
            this->textoverlay_->setCaption(convertToString((int)this->owner_->getHealth()));
        }

        if (this->bUseBarColour_)
            this->textoverlay_->setColour(this->getCurrentBarColour());
    }

    void HUDHealthBar::changedOwner()
    {
        SUPER(HUDHealthBar, changedOwner);

        this->owner_ = dynamic_cast<Pawn*>(this->getOwner());
    }

    void HUDHealthBar::changedOverlayGroup()
    {
        SUPER(HUDHealthBar, changedOverlayGroup);

        this->getOverlayGroup()->addElement(this->textoverlay_);
    }

    void HUDHealthBar::changedVisibility()
    {
        SUPER(HUDHealthBar, changedVisibility);

        this->textoverlay_->setVisible(this->isVisible());
    }

    void HUDHealthBar::changedName()
    {
        SUPER(HUDHealthBar, changedName);

        this->textoverlay_->setName(this->getName() + "text");
    }

    void HUDHealthBar::setTextColour(const ColourValue& colour)
    {
        this->textColour_ = colour;
        if (!this->bUseBarColour_)
            this->textoverlay_->setColour(colour);
    }

    void HUDHealthBar::setTextUseBarColour(bool bUseBarColour)
    {
        this->bUseBarColour_ = bUseBarColour;
        if (!bUseBarColour)
            this->textoverlay_->setColour(this->textColour_);
    }
}
