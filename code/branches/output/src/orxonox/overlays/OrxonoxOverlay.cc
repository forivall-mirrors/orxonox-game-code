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

/**
@file
@brief Definition of the OrxonoxOverlay class.
*/

#include "OrxonoxOverlay.h"

#include <cmath>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgrePanelOverlayElement.h>
#include <OgreRenderWindow.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

#include "util/Convert.h"
#include "util/Exception.h"
#include "core/GameMode.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/command/ConsoleCommand.h"

#include "OverlayGroup.h"

namespace orxonox
{
    unsigned int OrxonoxOverlay::hudOverlayCounter_s = 0;
    std::map<std::string, OrxonoxOverlay*> OrxonoxOverlay::overlays_s;

    SetConsoleCommand("OrxonoxOverlay", "scaleOverlay",     &OrxonoxOverlay::scaleOverlay);
    SetConsoleCommand("OrxonoxOverlay", "scrollOverlay",    &OrxonoxOverlay::scrollOverlay);
    SetConsoleCommand("OrxonoxOverlay", "toggleVisibility", &OrxonoxOverlay::toggleVisibility);
    SetConsoleCommand("OrxonoxOverlay", "show",     &OrxonoxOverlay::showOverlay);
    SetConsoleCommand("OrxonoxOverlay", "rotateOverlay",    &OrxonoxOverlay::rotateOverlay);

    OrxonoxOverlay::OrxonoxOverlay(BaseObject* creator)
        : BaseObject(creator)
    {
        RegisterObject(OrxonoxOverlay);

        this->owner_ = 0;
        this->group_ = 0;

        if (!GameMode::showsGraphics())
            ThrowException(NoGraphics, "Can't create OrxonoxOverlay, graphics engine not initialized");

        // create the Ogre::Overlay
        overlay_ = Ogre::OverlayManager::getSingleton().create("OrxonoxOverlay_overlay_"
            + multi_cast<std::string>(hudOverlayCounter_s++));

        // create background panel (can be used to show any picture)
        this->background_ = static_cast<Ogre::PanelOverlayElement*>(
            Ogre::OverlayManager::getSingleton().createOverlayElement("Panel",
            "OrxonoxOverlay_background_" + multi_cast<std::string>(hudOverlayCounter_s++)));
        this->overlay_->add2D(this->background_);

        // Get aspect ratio from the render window. Later on, we get informed automatically
        this->windowAspectRatio_ = static_cast<float>(this->getWindowWidth()) / this->getWindowHeight();

        this->size_ = Vector2(1.0f, 1.0f);
        this->pickPoint_= Vector2(0.0f, 0.0f);
        this->position_ = Vector2(0.0f, 0.0f);
        this->angle_ = Degree(0.0);
        this->bCorrectAspect_ = false;
        this->rotState_ = Horizontal;
        this->angleChanged(); // updates all other values as well

        setBackgroundMaterial("");
    }

    /**
    @brief
        Make sure everything gets removed/destroyed.
    @remark
        We assume that the Ogre::OverlayManager exists (there is an assert in Ogre for that!)
    */
    OrxonoxOverlay::~OrxonoxOverlay()
    {
        if (this->isInitialized())
        {
            // erase ourself from the map with all overlays
            std::map<std::string, OrxonoxOverlay*>::iterator it = overlays_s.find(this->getName());
            if (it != overlays_s.end())
                overlays_s.erase(it);

            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->background_);
            Ogre::OverlayManager::getSingleton().destroy(this->overlay_);
        }
    }

    /**
    @brief
        Loads the OrxonoxOverlay.

        This has to be called before usage, otherwise strange behaviour is
        guaranteed! (there should be no segfaults however).
    @copydoc
        BaseObject::XMLPort()
    */
    void OrxonoxOverlay::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(OrxonoxOverlay, XMLPort, xmlelement, mode);

        XMLPortParam(OrxonoxOverlay, "size",      setSize,      getSize,      xmlelement, mode);
        XMLPortParam(OrxonoxOverlay, "pickpoint", setPickPoint, getPickPoint, xmlelement, mode);
        XMLPortParam(OrxonoxOverlay, "position",  setPosition,  getPosition,  xmlelement, mode);
        XMLPortParam(OrxonoxOverlay, "rotation",  setRotation,  getRotation,  xmlelement, mode);
        XMLPortParam(OrxonoxOverlay, "correctaspect", setAspectCorrection,   getAspectCorrection,   xmlelement, mode);
        XMLPortParam(OrxonoxOverlay, "background",    setBackgroundMaterial, getBackgroundMaterial, xmlelement, mode);
        XMLPortParam(OrxonoxOverlay, "backgroundtex", setBackgroundTexture,  getBackgroundTexture,  xmlelement, mode);
    }

    void OrxonoxOverlay::changedName()
    {
        SUPER(OrxonoxOverlay, changedName);

        OrxonoxOverlay::overlays_s.erase(this->getOldName());

        if (OrxonoxOverlay::overlays_s.find(this->getName()) != OrxonoxOverlay::overlays_s.end())
            orxout(internal_warning) << "Overlay names should be unique or you cannnot access them via console. Name: \"" << this->getName() << '"' << endl;

        OrxonoxOverlay::overlays_s[this->getName()] = this;
    }

    //! Only sets the background material name if not ""
    void OrxonoxOverlay::setBackgroundMaterial(const std::string& material)
    {
        if (this->background_ && !material.empty())
            this->background_->setMaterialName(material);
    }

    //! Returns the the material name of the background
    const std::string& OrxonoxOverlay::getBackgroundMaterial() const
    {
        if (this->background_)
            return this->background_->getMaterialName();
        else
            return BLANKSTRING;
    }

    //! Sets the background texture name and creates a new material if necessary
    void OrxonoxOverlay::setBackgroundTexture(const std::string& texture)
    {
        if (this->background_ && this->background_->getMaterial().isNull() && !texture.empty())
        {
            // create new material
            const std::string& materialname = "generated_material" + getUniqueNumberString();
            Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(materialname, "General"));
            material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
            Ogre::TextureUnitState* textureUnitState_ = material->getTechnique(0)->getPass(0)->createTextureUnitState();
            textureUnitState_->setTextureName(texture);
            textureUnitState_->setNumMipmaps(0);
            this->background_->setMaterialName(materialname);
        }
    }

    //! Returns the the texture name of the background
    const std::string& OrxonoxOverlay::getBackgroundTexture() const
    {
        if (this->background_)
        {
            Ogre::TextureUnitState* tempTx = this->background_->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0);
            return tempTx->getTextureName();
        }
        else
            return BLANKSTRING;
    }

    //! Called by BaseObject when visibility has changed.
    void OrxonoxOverlay::changedVisibility()
    {
        SUPER( OrxonoxOverlay, changedVisibility );

        if (!this->overlay_)
            return;

        // only set to visible if corresponding OverlayGroup is also visible
        if (this->isVisible() && (!this->getOverlayGroup() || this->getOverlayGroup()->isVisible()) )
            this->overlay_->show();
        else
            this->overlay_->hide();
    }

    /**
    @brief
        Called by the GraphicsManager whenever the window size changes.
        Calculates the aspect ratio only.
    */
    void OrxonoxOverlay::windowResized(unsigned int newWidth, unsigned int newHeight)
    {
        this->windowAspectRatio_ = static_cast<float>(newWidth) / newHeight;
        this->sizeCorrectionChanged();
    }

    /**
    @brief
        Called whenever the rotation angle has changed.
    */
    void OrxonoxOverlay::angleChanged()
    {
        if (!this->overlay_)
            return;

        this->overlay_->setRotate(this->angle_);
        this->sizeCorrectionChanged();
    }

    /**
    @brief
        Called whenever the aspect ratio or the angle has changed.
        The method calculates a correction factor for the size to compensate
        for aspect distortion if desired.
    @remarks
        This only works when the angle is about 0 or 90 degrees.
    */
    void OrxonoxOverlay::sizeCorrectionChanged()
    {
        if (this->bCorrectAspect_)
        {
            float angle = this->angle_.valueDegrees();
            if (angle < 0.0)
                angle = -angle;
            angle -= 180.0f * static_cast<int>(angle / 180.0);

            // take the reverse if angle is about 90 degrees
            float tempAspect;
            if (angle > 89.0f && angle < 91.0f)
            {
                tempAspect = 1.0f / this->windowAspectRatio_;
                rotState_ = Vertical;
            }
            else if (angle > 179 || angle < 1)
            {
                tempAspect = this->windowAspectRatio_;
                rotState_ = Horizontal;
            }
            else
            {
                tempAspect = 1.0f;
                rotState_ = Inbetween;
            }

            // note: this is only an approximation that is mostly valid when the
            // magnitude of the width is about the magnitude of the height.
            // Correctly we would have to take the square root of width*height
            this->sizeCorrection_.x = 2.0f / (tempAspect + 1.0f);
            this->sizeCorrection_.y = tempAspect * this->sizeCorrection_.x;
        }
        else
        {
            this->sizeCorrection_ = Vector2::UNIT_SCALE;
        }

        this->sizeChanged();
    }

    /**
    @brief
        Sets the overlay size using the actual corrected size.
    */
    void OrxonoxOverlay::sizeChanged()
    {
        if (!this->overlay_)
            return;

        this->overlay_->setScale(size_.x * sizeCorrection_.x, size_.y * sizeCorrection_.y);
        positionChanged();
    }

    /**
    @brief
        Determines the position of the overlay.
        This works also well when a rotation angle is applied. The overlay always
        gets aligned correctly as well as possible.
    */
    void OrxonoxOverlay::positionChanged()
    {
        if (!this->overlay_)
            return;

        // transform the angle to a range of 0 - pi/2 first.
        float angle = this->angle_.valueRadians();
        if (angle < 0.0)
            angle = -angle;
        angle -= math::pi * static_cast<int>(angle / (math::pi));
        if (angle > math::pi_2)
            angle = math::pi - angle;

        // do some mathematical fiddling for a bounding box
        Vector2 actualSize = size_ * sizeCorrection_;
        float radius = actualSize.length();
        float phi = atan(actualSize.y / actualSize.x);
        Vector2 boundingBox(radius * cos(angle - phi), radius * sin(angle + phi));

        // calculate the scrolling offset
        Vector2 scroll = (position_ - 0.5 - boundingBox * (pickPoint_ - 0.5)) * 2.0;
        this->overlay_->setScroll(scroll.x, -scroll.y);
    }


    //########### Console commands ############

    /**
    @brief
        Scales an overlay by its name.
    @param name
        The name of the overlay defined BaseObject::setName() (usually done with the "name"
        attribute in the xml file).
    @param scale
        The scaling factor
    */
    /*static*/ void OrxonoxOverlay::scaleOverlay(const std::string& name, float scale)
    {
        std::map<std::string, OrxonoxOverlay*>::const_iterator it = overlays_s.find(name);
        if (it != overlays_s.end())
            it->second->scale(Vector2(scale, scale));
    }

    /**
    @brief
        Toggles the visibility of an Overlay by it's name.
    @param name
        The name of the overlay defined BaseObject::setName() (usually done with the "name"
        attribute in the xml file).
    */
    /*static*/ void OrxonoxOverlay::toggleVisibility(const std::string& name)
    {
        std::map<std::string, OrxonoxOverlay*>::const_iterator it = overlays_s.find(name);
        if (it != overlays_s.end())
        {
            OrxonoxOverlay* overlay= it->second;
            if(overlay->isVisible())
            {
                overlay->hide();
                orxout(verbose, context::misc::overlays) << "HIDE " << name << endl;
            }
            else
            {
                overlay->show();
                orxout(verbose, context::misc::overlays) << "SHOW " << name << endl;
            }
        }
    }
    
    /**
    @brief
        Shows Overlay by it's name.
    @param name
        The name of the overlay defined BaseObject::setName() (usually done with the "name"
        attribute in the xml file).
    */
    /*static*/ void OrxonoxOverlay::showOverlay(const std::string& name)
    {
        std::map<std::string, OrxonoxOverlay*>::const_iterator it = overlays_s.find(name);
        if (it != overlays_s.end())
        {
            OrxonoxOverlay* overlay= it->second;
            if(overlay->isVisible())
                overlay->changedVisibility();
            else
                overlay->show();
        }
    }

    /**
    @brief
        Scrolls an overlay by its name.
    @param name
        The name of the overlay defined BaseObject::setName() (usually done with the "name"
        attribute in the xml file).
    @param scroll
        The relative translation of the overlay
    */
    /*static*/ void OrxonoxOverlay::scrollOverlay(const std::string& name, const Vector2& scroll)
    {
        std::map<std::string, OrxonoxOverlay*>::const_iterator it = overlays_s.find(name);
        if (it != overlays_s.end())
            it->second->scroll(scroll);
    }

    /**
    @brief
        Rotates an overlay by its name.
    @param name
        The name of the overlay defined BaseObject::setName() (usually done with the "name"
        attribute in the xml file).
    @param angle
        The rotation angle in degree
    */
    /*static*/ void OrxonoxOverlay::rotateOverlay(const std::string& name, const Degree& angle)
    {
        std::map<std::string, OrxonoxOverlay*>::const_iterator it = overlays_s.find(name);
        if (it != overlays_s.end())
            it->second->rotate(angle);
    }

    void OrxonoxOverlay::setOverlayGroup(OverlayGroup* group)
    {
        if (group != this->group_)
        {
            if (this->group_)
                this->group_->removeElement(this);
            this->group_ = group;
            this->changedOverlayGroup();
        }
    }

    void OrxonoxOverlay::setBackgroundAlpha(float alpha)
    {
        Ogre::MaterialPtr ptr = this->background_->getMaterial();
        Ogre::TextureUnitState* tempTx = ptr->getTechnique(0)->getPass(0)->getTextureUnitState(0);
        tempTx->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, alpha);
    }

    void OrxonoxOverlay::setBackgroundColour(ColourValue colour)
    {
        Ogre::MaterialPtr ptr = this->background_->getMaterial();
        Ogre::TextureUnitState* tempTx = ptr->getTechnique(0)->getPass(0)->getTextureUnitState(0);
        tempTx->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, colour);
    }
}
