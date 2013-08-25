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

#include "Backlight.h"

#include <algorithm>
#include <OgreRibbonTrail.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
// windows.h is getting included above on Windows...
#undef max
#undef min

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "Scene.h"

namespace orxonox
{
    RegisterClass(Backlight);

    Backlight::Backlight(Context* context) : FadingBillboard(context)
    {
        RegisterObject(Backlight);

        this->ribbonTrail_ = 0;
        this->ribbonTrailNode_ = 0;

        this->width_ = 0;
        this->length_ = 1.0f;
        this->lifetime_ = 0.001f;
        this->maxelements_ = 1;

        this->tickcount_ = 0;

        if (GameMode::showsGraphics())
        {
            if (!this->getScene())
                ThrowException(AbortLoading, "Can't create Backlight, no scene given.");
            if (!this->getScene()->getSceneManager())
                ThrowException(AbortLoading, "Can't create Backlight, no scene manager given.");
            if (!this->getScene()->getRootSceneNode())
                ThrowException(AbortLoading, "Can't create Backlight, no root scene node given.");

            this->ribbonTrail_ = this->getScene()->getSceneManager()->createRibbonTrail(this->getNode()->getName());

            this->ribbonTrailNode_ = this->getScene()->getRootSceneNode()->createChildSceneNode();
            this->ribbonTrailNode_->attachObject(this->ribbonTrail_);

            this->ribbonTrail_->setMaxChainElements(this->maxelements_);
            this->ribbonTrail_->setTrailLength(this->length_);
            this->ribbonTrail_->setInitialWidth(0, 0);
        }

        this->registerVariables();
    }

    Backlight::~Backlight()
    {
        if (this->isInitialized())
        {
            if (this->ribbonTrail_)
            {
                if (this->ribbonTrailNode_)
                {
                    this->ribbonTrailNode_->detachObject(this->ribbonTrail_);
                    this->getScene()->getSceneManager()->destroySceneNode(this->ribbonTrailNode_->getName());
                }
                this->getScene()->getSceneManager()->destroyRibbonTrail(this->ribbonTrail_);
            }
        }
    }

    void Backlight::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Backlight, XMLPort, xmlelement, mode);

        XMLPortParam(Backlight, "length",        setLength,        getLength,        xmlelement, mode).defaultValues(100.0f);
        XMLPortParam(Backlight, "width",         setWidth,         getWidth,         xmlelement, mode).defaultValues(1.0f);
        XMLPortParam(Backlight, "elements",      setMaxElements,   getMaxElements,   xmlelement, mode).defaultValues(10);
        XMLPortParam(Backlight, "lifetime",      setLifetime,      getLifetime,      xmlelement, mode).defaultValues(1.0f);
        XMLPortParam(Backlight, "trailmaterial", setTrailMaterial, getTrailMaterial, xmlelement, mode);
    }

    void Backlight::registerVariables()
    {
        registerVariable(this->width_,         VariableDirection::ToClient, new NetworkCallback<Backlight>(this, &Backlight::update_width));
        registerVariable(this->lifetime_,      VariableDirection::ToClient, new NetworkCallback<Backlight>(this, &Backlight::update_lifetime));
        registerVariable(this->length_,        VariableDirection::ToClient, new NetworkCallback<Backlight>(this, &Backlight::update_length));
        registerVariable(this->maxelements_,   VariableDirection::ToClient, new NetworkCallback<Backlight>(this, &Backlight::update_maxelements));
        registerVariable(this->trailmaterial_, VariableDirection::ToClient, new NetworkCallback<Backlight>(this, &Backlight::update_trailmaterial));
    }

    void Backlight::changedColour()
    {
        FadingBillboard::changedColour();

        if (this->ribbonTrail_ && this->tickcount_ >= 2)
            this->ribbonTrail_->setInitialColour(0, this->getFadedColour());
    }

    void Backlight::update_width()
    {
        if (this->ribbonTrail_ && this->tickcount_ >= 2)
            this->ribbonTrail_->setInitialWidth(0, this->width_ * this->getWorldScale());
        this->update_lifetime();
    }

    void Backlight::update_lifetime()
    {
        if (this->ribbonTrail_ && this->tickcount_ >= 2)
        {
            this->ribbonTrail_->setWidthChange(0, this->width_ * this->getWorldScale() / this->lifetime_ * this->getTimeFactor());
            this->ribbonTrail_->setColourChange(0, 0, 0, 0, 1.0f / this->lifetime_ * this->getTimeFactor());
        }
    }

    void Backlight::update_length()
    {
        if (this->ribbonTrail_ && this->tickcount_ >= 2)
            this->ribbonTrail_->setTrailLength(this->length_ * this->getWorldScale());
    }

    void Backlight::update_maxelements()
    {
        if (this->ribbonTrail_ && this->tickcount_ >= 2)
            this->ribbonTrail_->setMaxChainElements(this->maxelements_);
    }

    void Backlight::update_trailmaterial()
    {
        if (this->ribbonTrail_ && this->tickcount_ >= 2)
            this->ribbonTrail_->setMaterialName(this->trailmaterial_);
    }

    void Backlight::changedVisibility()
    {
        SUPER(Backlight, changedVisibility);

        if (this->ribbonTrail_)
            this->ribbonTrail_->setVisible(this->isVisible());
    }

    void Backlight::startturnonoff()
    {
        FadingBillboard::startturnonoff();

        if (this->ribbonTrail_ && this->isActive() && this->isVisible())
            this->ribbonTrail_->setVisible(true);
    }

    void Backlight::stopturnonoff()
    {
        this->postprocessingtime_ = std::max(0.0f, this->lifetime_ - this->turnofftime_);

        FadingBillboard::stopturnonoff();

        if (this->ribbonTrail_)
            this->ribbonTrail_->setInitialColour(0, this->getFadedColour());
    }

    void Backlight::poststopturnonoff()
    {
        FadingBillboard::poststopturnonoff();

        if (this->ribbonTrail_)
            this->ribbonTrail_->setVisible(false);
    }

    void Backlight::changedScale()
    {
        SUPER(Backlight, changedScale);

        this->update_width();
        this->update_length();
    }

    void Backlight::tick(float dt)
    {
        if (this->tickcount_ < 2)
        {
            ++this->tickcount_;
            if (this->tickcount_ == 2)
            {
                this->changedColour();
                this->update_width();
                this->update_lifetime();
                this->update_length();
                this->update_maxelements();
                this->update_trailmaterial();
                if (this->ribbonTrail_)
                    this->ribbonTrail_->addNode(this->node_);
            }
        }

        SUPER(Backlight, tick, dt);

        if (this->ribbonTrail_ && this->changedirection_ != 0)
        {
            // we use alpha_blend, only adjust alpha
            const ColourValue& colour = this->getColour();
            this->ribbonTrail_->setInitialColour(0, colour.r, colour.g, colour.b, this->getFadedColour().a);
        }
    }

    void Backlight::changedTimeFactor(float factor_new, float factor_old)
    {
        this->update_lifetime();
    }
}
