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

#include "FadingBillboard.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(FadingBillboard);

    FadingBillboard::FadingBillboard(Context* context) : Billboard(context)
    {
        RegisterObject(FadingBillboard);

        this->turnontime_ = 0.0f;
        this->turnofftime_ = 0.0f;
        this->postprocessingtime_ = 0.0f;
        this->changedirection_ = 0;

        this->fadedColour_ = ColourValue::White;

        this->registerVariables();
    }

    FadingBillboard::~FadingBillboard()
    {
    }

    void FadingBillboard::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(FadingBillboard, XMLPort, xmlelement, mode);

        XMLPortParam(FadingBillboard, "turnontime",  setTurnOnTime,  getTurnOnTime,  xmlelement, mode).defaultValues(0.5f);
        XMLPortParam(FadingBillboard, "turnofftime", setTurnOffTime, getTurnOffTime, xmlelement, mode).defaultValues(0.5f);
    }

    void FadingBillboard::registerVariables()
    {
        registerVariable(this->turnontime_,  VariableDirection::ToClient);
        registerVariable(this->turnofftime_, VariableDirection::ToClient);
    }

    void FadingBillboard::changedColour()
    {
        Billboard::changedColour();

        if (this->isActive())
            this->fadedColour_ = this->getColour();
        else
            this->fadedColour_ = ColourValue::ZERO;

        this->getBillboardSet().setColour(this->fadedColour_);
    }

    void FadingBillboard::changedActivity()
    {
        SUPER(FadingBillboard, changedActivity);

        this->startturnonoff();
    }

    void FadingBillboard::changedVisibility()
    {
        SUPER(FadingBillboard, changedVisibility);

        if (this->isVisible() && !this->isActive() && this->changedirection_ == 0)
        {
            // Billboard shouldn't be visible
            this->getBillboardSet().setVisible(false);
        }
    }

    void FadingBillboard::startturnonoff()
    {
        if (this->isActive())
        {
            this->changedirection_ = 1;
            this->turnonofftimer_.setTimer(this->turnontime_, false, createExecutor(createFunctor(&FadingBillboard::stopturnonoff, this)));

            if (this->isVisible())
                this->getBillboardSet().setVisible(true);
        }
        else
        {
            this->changedirection_ = -1;
            this->turnonofftimer_.setTimer(this->turnofftime_, false, createExecutor(createFunctor(&FadingBillboard::stopturnonoff, this)));
        }
    }

    void FadingBillboard::stopturnonoff()
    {
        if (this->changedirection_ > 0)
        {
            this->fadedColour_ = this->getColour();
            this->getBillboardSet().setColour(this->fadedColour_);
        }
        else if (this->changedirection_ < 0)
        {
            this->fadedColour_ = ColourValue::ZERO;
            this->getBillboardSet().setColour(this->fadedColour_);
            this->turnonofftimer_.setTimer(this->postprocessingtime_, false, createExecutor(createFunctor(&FadingBillboard::poststopturnonoff, this)));
        }
        this->changedirection_ = 0;
    }

    void FadingBillboard::poststopturnonoff()
    {
        this->getBillboardSet().setVisible(false);
    }

    void FadingBillboard::tick(float dt)
    {
        SUPER(FadingBillboard, tick, dt);

        if (this->changedirection_ > 0 && (this->fadedColour_.a < this->getColour().a))
        {
            ColourValue colour = this->fadedColour_ + this->getColour() / this->turnontime_ * dt;

            if (colour.a < this->getColour().a)
            {
                this->fadedColour_ = colour;
                this->getBillboardSet().setColour(this->fadedColour_);
            }
        }
        else if (this->changedirection_ < 0 && (this->fadedColour_.a > 0))
        {
            ColourValue colour = this->fadedColour_ - this->getColour() / this->turnofftime_ * dt;

            if (colour.a > 0)
            {
                this->fadedColour_ = colour;
                this->getBillboardSet().setColour(this->fadedColour_);
            }
        }
    }
}
