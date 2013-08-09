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

#include "BlinkingBillboard.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(BlinkingBillboard);

    BlinkingBillboard::BlinkingBillboard(Context* context) : Billboard(context)
    {
        RegisterObject(BlinkingBillboard);

        this->amplitude_ = 1.0f;
        this->frequency_ = 1.0f;
        this->phase_ = 0;
        this->bQuadratic_ = false;
        this->time_ = 0;

        this->registerVariables();
    }

    BlinkingBillboard::~BlinkingBillboard()
    {
    }

    void BlinkingBillboard::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(BlinkingBillboard, XMLPort, xmlelement, mode);

        XMLPortParam(BlinkingBillboard, "amplitude", setAmplitude, getAmplitude, xmlelement, mode).defaultValues(1.0f);
        XMLPortParam(BlinkingBillboard, "frequency", setFrequency, getFrequency, xmlelement, mode).defaultValues(1.0f);
        XMLPortParam(BlinkingBillboard, "phase",     setPhase,     getPhase,     xmlelement, mode).defaultValues(Degree(0));
        XMLPortParam(BlinkingBillboard, "quadratic", setQuadratic, isQuadratic,  xmlelement, mode).defaultValues(false);
    }

    void BlinkingBillboard::registerVariables()
    {
        unregisterVariable(this->getScale3D());
        registerVariable(this->amplitude_,  VariableDirection::ToClient);
        registerVariable(this->frequency_,  VariableDirection::ToClient);
        registerVariable(this->phase_,      VariableDirection::ToClient);
        registerVariable(this->bQuadratic_, VariableDirection::ToClient);
    }

    void BlinkingBillboard::tick(float dt)
    {
        SUPER(BlinkingBillboard, tick, dt);

        if (this->isActive())
        {
            // Wrap around to avoid loosing floating point precision
            this->time_ = std::fmod(this->time_ + dt, 1.0f / this->frequency_);
            float value = sin((math::twoPi * this->time_ + this->phase_.valueRadians()) * this->frequency_);
            if (this->bQuadratic_)
                this->setScale(this->amplitude_ * square(value));
            else
                this->setScale(this->amplitude_ * std::abs(value));
        }
    }
}
