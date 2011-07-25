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

#include "FadeoutText.h"

#include <OgreTextAreaOverlayElement.h>
#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(FadeoutText);

    FadeoutText::FadeoutText(BaseObject* creator) : OverlayText(creator)
    {
        RegisterObject(FadeoutText);

        this->delay_ = 3.0f;
        this->fadeouttime_ = 1.0f;

        this->bFadingOut_ = false;
        this->fadeouttimer_.setTimer(3.0f, false, createExecutor(createFunctor(&FadeoutText::fadeout, this)));
        this->fadeouttimer_.stopTimer();

        this->initialAlpha_ = 1.0f;
    }

    void FadeoutText::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(FadeoutText, XMLPort, xmlelement, mode);

        XMLPortParam(FadeoutText, "delay", setDelay, getDelay, xmlelement, mode);
        XMLPortParam(FadeoutText, "fadeout", setFadeouttime, getFadeouttime, xmlelement, mode);
    }

    void FadeoutText::tick(float dt)
    {
        if (this->bFadingOut_)
        {
            ColourValue colour = this->getColour();

            if (colour.a > 0)
            {
                float alpha = colour.a;

                alpha -= dt / this->fadeouttime_;
                if (alpha < 0)
                    alpha = 0;
                colour.a = alpha;

                this->text_->setColour(colour);
            }
            else
            {
                this->text_->setCaption("");
                this->reset();
            }
        }
    }

    void FadeoutText::fadeout()
    {
        this->bFadingOut_ = true;
    }

    void FadeoutText::reset()
    {
        this->bFadingOut_ = false;

        ColourValue colour = this->getColour();
        colour.a = this->initialAlpha_;
        this->text_->setColour(colour);
    }

    void FadeoutText::changedColour()
    {
        OverlayText::changedColour();

        this->initialAlpha_ = this->getColour().a;
    }

    void FadeoutText::changedCaption()
    {
        OverlayText::changedCaption();

        this->reset();
        this->fadeouttimer_.setInterval(this->delay_);
        this->fadeouttimer_.startTimer();
    }
}
