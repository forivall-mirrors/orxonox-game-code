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

#include "OrxonoxStableHeaders.h"
#include "Backlight.h"

#include <OgreBillboard.h>
#include <OgreRibbonTrail.h>
#include <OgreSceneManager.h>

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Executor.h"
#include "util/Math.h"
#include "GraphicsEngine.h"
#include "Orxonox.h"

namespace orxonox
{
    CreateFactory(Backlight);

    float Backlight::timeFactor_s = 1.0;

    Backlight::Backlight(float maxspeed, float brakingtime, float scale)
    {
        RegisterObject(Backlight);

        this->setConfigValues();
        this->traillength_ = 1;

        this->getNode()->setInheritScale(false);

        this->billboard_.setBillboardSet("Flares/backlightflare");
        this->attachObject(this->billboard_.getBillboardSet());

        this->ribbonTrail_ = GraphicsEngine::getInstance().getLevelSceneManager()->createRibbonTrail(this->getName() + "RibbonTrail");
        this->ribbonTrailNode_ = GraphicsEngine::getInstance().getLevelSceneManager()->getRootSceneNode()->createChildSceneNode(this->getName() + "RibbonTrailNode");
        this->ribbonTrailNode_->attachObject(this->ribbonTrail_);
        this->ribbonTrail_->addNode(this->getNode());

        this->configure(maxspeed, brakingtime, scale);

        this->ribbonTrail_->setTrailLength(this->maxTraillength_);
        this->ribbonTrail_->setMaterialName("Trail/backlighttrail");

        //this->setTimeFactor(Orxonox::getInstance().getTimeFactor());
        this->setTimeFactor(1.0f);
    }

    Backlight::~Backlight()
    {
        if (this->isInitialized())
        {
            this->detachObject(this->billboard_.getBillboardSet());
            GraphicsEngine::getInstance().getLevelSceneManager()->destroySceneNode(this->getName() + "RibbonTrailNode");
            GraphicsEngine::getInstance().getLevelSceneManager()->destroyRibbonTrail(this->ribbonTrail_);
        }
    }

    void Backlight::setConfigValues()
    {
        SetConfigValue(maxLifeTime_, 4.0).description("The maximal amount of seconds the trail behind a SpaceShip stays visible");
        SetConfigValue(trailSegmentLength_, 50).description("The length of one segment of the trail behind a SpaceShip (lower values make it more smooth)");
        SetConfigValue(width_, 7.0).description("The width of the trail");
    }

    void Backlight::setTimeFactor(float factor)
    {
        Backlight::timeFactor_s = factor;
        float change = Backlight::timeFactor_s / this->maxLifeTime_;
        this->ribbonTrail_->setWidthChange(0, this->width_ * change);
        this->updateColourChange();
    }

    void Backlight::updateColourChange()
    {
        this->ribbonTrail_->setColourChange(0, ColourValue(0, 0, 0, this->maxTraillength_ / this->traillength_ / this->maxLifeTime_ * Backlight::timeFactor_s));
    }

    void Backlight::tick(float dt)
    {
        WorldEntity::tick(dt);

        if (this->isActive())
        {
            if (this->traillength_ < this->maxTraillength_)
            {
                this->traillength_ = min<float>(this->maxTraillength_, this->traillength_ + dt * this->maxTraillength_ / this->maxLifeTime_);
                this->updateColourChange();
            }
        }
        else
        {
            if (this->traillength_ > 1)
            {
                this->traillength_ = max<float>(1, this->traillength_ - this->brakefactor_ * dt * this->maxTraillength_ / this->maxLifeTime_);
                this->updateColourChange();
            }
        }

        this->ribbonTrail_->setTrailLength(this->traillength_);
    }

    void Backlight::setColour(const ColourValue& colour)
    {
        this->billboard_.getBillboardSet()->getBillboard(0)->setColour(colour);
        this->ribbonTrail_->setInitialColour(0, ColourValue(colour.r / 4 + 0.75, colour.g / 4 + 0.75, colour.b / 4 + 0.75));
    }

    void Backlight::configure(float maxspeed, float brakingtime, float scale)
    {
        this->maxTraillength_ = this->maxLifeTime_ * maxspeed;
        this->maxTrailsegments_ = (size_t)(this->maxTraillength_ / this->trailSegmentLength_);
        this->ribbonTrail_->setMaxChainElements(this->maxTrailsegments_);
        this->ribbonTrail_->setTrailLength(this->traillength_ = 2 * this->maxTrailsegments_);

        this->brakefactor_ = this->maxLifeTime_ / brakingtime;

        this->scale(scale);
        this->ribbonTrail_->setInitialWidth(0, this->width_ * scale);
        this->ribbonTrail_->setWidthChange(0, this->width_ * scale / this->maxLifeTime_ * Backlight::timeFactor_s);
    }

    void Backlight::changedVisibility()
    {
        WorldEntity::changedVisibility();

        this->billboard_.setVisible(this->isVisible());
        this->ribbonTrail_->setVisible(this->isVisible());
    }
}
