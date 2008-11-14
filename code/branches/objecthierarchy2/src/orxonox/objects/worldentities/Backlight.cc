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

#include <OgreRibbonTrail.h>
#include <OgreSceneManager.h>

#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/Executor.h"
#include "core/XMLPort.h"
#include "objects/Scene.h"

namespace orxonox
{
    CreateFactory(Backlight);

    Backlight::Backlight(BaseObject* creator) : Billboard(creator)
    {
        RegisterObject(Backlight);

        this->ribbonTrail_ = 0;
        this->ribbonTrailNode_ = 0;

        this->width_ = 0;
        this->length_ = 1.0f;
        this->lifetime_ = 0.001f;
        this->turnofftime_ = 0.5f;
        this->bTurningOff_ = false;
        this->maxelements_ = 1;

        this->tickcount_ = 0;

        if (Core::showsGraphics())
        {
            assert(this->getScene());
            assert(this->getScene()->getSceneManager());
            assert(this->getScene()->getRootSceneNode());

            this->ribbonTrail_ = this->getScene()->getSceneManager()->createRibbonTrail(this->getNode()->getName());
            this->ribbonTrail_->addNode(this->getNode());
            this->ribbonTrailNode_ = this->getScene()->getRootSceneNode()->createChildSceneNode();
            this->ribbonTrailNode_->attachObject(this->ribbonTrail_);
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
        XMLPortParam(Backlight, "turnofftime",   setTurnOffTime,   getTurnOffTime,   xmlelement, mode).defaultValues(0.5f);
        XMLPortParam(Backlight, "trailmaterial", setTrailMaterial, getTrailMaterial, xmlelement, mode);
    }

    void Backlight::registerVariables()
    {
        REGISTERDATA  (this->width_,         direction::toclient, new NetworkCallback<Backlight>(this, &Backlight::update_width));
        REGISTERDATA  (this->lifetime_,      direction::toclient, new NetworkCallback<Backlight>(this, &Backlight::update_lifetime));
        REGISTERDATA  (this->length_,        direction::toclient, new NetworkCallback<Backlight>(this, &Backlight::update_length));
        REGISTERDATA  (this->maxelements_,   direction::toclient, new NetworkCallback<Backlight>(this, &Backlight::update_maxelements));
        REGISTERSTRING(this->trailmaterial_, direction::toclient, new NetworkCallback<Backlight>(this, &Backlight::update_trailmaterial));
    }

    void Backlight::changedColour()
    {
        Billboard::changedColour();

        if (this->ribbonTrail_ && this->isActive() && this->tickcount_ >= 2)
            this->ribbonTrail_->setInitialColour(0, this->getColour());
    }

    void Backlight::update_width()
    {
        if (this->ribbonTrail_ && this->tickcount_ >= 2)
            this->ribbonTrail_->setInitialWidth(0, this->width_);
        this->update_lifetime();
    }

    void Backlight::update_lifetime()
    {
        if (this->ribbonTrail_ && this->tickcount_ >= 2)
        {
            this->ribbonTrail_->setWidthChange(0, this->width_ / this->lifetime_/* * Backlight::timeFactor_s*/);
            this->ribbonTrail_->setColourChange(0, 0, 0, 0, 1.0f / this->lifetime_/* * Backlight::timeFactor_s*/);
        }
    }

    void Backlight::update_length()
    {
        if (this->ribbonTrail_ && this->tickcount_ >= 2)
            this->ribbonTrail_->setTrailLength(this->length_);
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

    void Backlight::changedActivity()
    {
        SUPER(Backlight, changedActivity);

        if (this->ribbonTrail_)
        {
            if (this->isActive())
                this->ribbonTrail_->setInitialColour(0, this->getColour());
            else
            {
                this->bTurningOff_ = true;
                this->turnofftimer_.setTimer(this->turnofftime_, false, this, createExecutor(createFunctor(&Backlight::stopturnoff)));
            }
        }
    }

    void Backlight::stopturnoff()
    {
        this->bTurningOff_ = false;
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
            }
        }

        if (this->bTurningOff_ && this->ribbonTrail_)
        {
            this->ribbonTrail_->setInitialColour(0, this->ribbonTrail_->getInitialColour(0) - this->getColour() / this->turnofftime_ * dt);
        }
    }

//------------------------------------------------------------------------------------
/*
    float Backlight::timeFactor_s = 1.0;

    Backlight::Backlight(float maxspeed, float brakingtime, float scale)
    {
        RegisterObject(Backlight);

        this->setConfigValues();
        this->traillength_ = 1;
        this->colour_ = ColourValue::White;

        this->configure(maxspeed, brakingtime, scale);
    }

    bool Backlight::create(){
      if(!WorldEntity::create())
        return false;

      this->getNode()->setInheritScale(false);

      this->billboard_.setBillboardSet("Flares/backlightflare");
      this->attachObject(this->billboard_.getBillboardSet());

      this->ribbonTrail_ = GraphicsEngine::getInstance().getLevelSceneManager()->createRibbonTrail(this->getName() + "RibbonTrail");
      this->ribbonTrailNode_ = GraphicsEngine::getInstance().getLevelSceneManager()->getRootSceneNode()->createChildSceneNode(this->getName() + "RibbonTrailNode");
      this->ribbonTrailNode_->attachObject(this->ribbonTrail_);
      this->ribbonTrail_->addNode(this->getNode());


      this->ribbonTrail_->setTrailLength(this->maxTraillength_);
      this->ribbonTrail_->setMaterialName("Trail/backlighttrail");

        //this->setTimeFactor(Orxonox::getInstance().getTimeFactor());
      this->setTimeFactor(1.0f);

      this->ribbonTrail_->setMaxChainElements(this->maxTrailsegments_);
      this->ribbonTrail_->setTrailLength(this->traillength_ = 2 * this->maxTrailsegments_);
      this->ribbonTrail_->setInitialWidth(0, this->width_ * this->getScale());
      this->ribbonTrail_->setWidthChange(0, this->width_ * this->getScale() / this->maxLifeTime_ * Backlight::timeFactor_s);
      return true;
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
        SUPER(Backlight, tick, dt);

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

    void Backlight::configure(float maxspeed, float brakingtime, float scale)
    {
        this->maxTraillength_ = this->maxLifeTime_ * maxspeed;
        this->maxTrailsegments_ = (size_t)(this->maxTraillength_ / this->trailSegmentLength_);

        this->brakefactor_ = this->maxLifeTime_ / brakingtime;

        this->scale(scale);
    }

    void Backlight::changedVisibility()
    {
        SUPER(Backlight, changedVisibility);

        this->billboard_.setVisible(this->isVisible());
        this->ribbonTrail_->setVisible(this->isVisible());
    }
*/
}
