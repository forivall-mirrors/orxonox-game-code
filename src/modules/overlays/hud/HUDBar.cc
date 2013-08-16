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
 *      Yuning Chai
 *   Co-authors:
 *      Felix Schulthess
 *      Fabian 'x3n' Landau
 *      Reto Grieder
 *      Benjamin Knecht
 *
 */

#include "HUDBar.h"

#include <OgreOverlayManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgrePanelOverlayElement.h>

#include "util/Convert.h"
#include "util/StringUtils.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(BarColour);

    BarColour::BarColour(Context* context)
        : BaseObject(context)
    {
        RegisterObject(BarColour);

        this->setColour(ColourValue(1.0, 1.0, 1.0, 1.0));
        this->setPosition(0.0);
    }

    void BarColour::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(BarColour, XMLPort, xmlelement, mode);

        XMLPortParam(BarColour, "colour", setColour, getColour, xmlelement, mode);
        XMLPortParam(BarColour, "position", setPosition, getPosition, xmlelement, mode);
    }


    unsigned int HUDBar::materialcount_s = 0;

    RegisterClass(HUDBar);

    HUDBar::HUDBar(Context* context)
        : OrxonoxOverlay(context), right2Left_(false), autoColour_(false)
    {
        RegisterObject(HUDBar);

        // create new material
        const std::string& materialname = "barmaterial" + multi_cast<std::string>(materialcount_s++);
        Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(materialname, "General"));
        material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        this->textureUnitState_ = material->getTechnique(0)->getPass(0)->createTextureUnitState();
        this->textureUnitState_->setTextureName("bar2.png");
        this->textureUnitState_->setNumMipmaps(0);
        // use the default colour
        this->textureUnitState_->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, ColourValue(0.2f, 0.7f, 0.2f));

        this->bar_ = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton()
            .createOverlayElement("Panel", "HUDBar_bar_" + getUniqueNumberString()));
        this->bar_->setMaterialName(materialname);

        this->value_ = 1.0f;  // initialize with 1.0f to trigger a change when calling setValue(0.0f) on the line below
        this->setAutoColour(true);
        this->setValue(0.0f); // <--
        this->setRightToLeft(false);
        this->currentColour_ = ColourValue::White;

        this->background_->addChild(bar_);
    }

    HUDBar::~HUDBar()
    {
        if (this->isInitialized())
        {
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->bar_);
            for (std::vector<BarColour*>::const_iterator it = this->barColours_.begin(); it != this->barColours_.end(); )
                (*it++)->destroy();
        }
    }

    void HUDBar::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HUDBar, XMLPort, xmlelement, mode);

        XMLPortParam(HUDBar, "initialvalue", setValue,       getValue,       xmlelement, mode);
        XMLPortParam(HUDBar, "righttoleft",  setRightToLeft, getRightToLeft, xmlelement, mode);
        XMLPortParam(HUDBar, "autocolour",   setAutoColour,  getAutoColour,  xmlelement, mode);
        XMLPortParam(HUDBar, "bartexture",   setBarTexture,  getBarTexture, xmlelement, mode);
        XMLPortObject(HUDBar, BarColour, "", addColour, getColour, xmlelement, mode);
    }

    void HUDBar::valueChanged()
    {
        if (this->autoColour_ && this->textureUnitState_)
        {
            // set colour
            if (this->colours_.size() > 0)
            {
                ColourValue colour1(0, 0, 0, 1);
                ColourValue colour2 = this->colours_.rbegin()->second;
                float value1(0);
                float value2 = this->colours_.rbegin()->first;
                for (std::map<float, ColourValue>::reverse_iterator it = this->colours_.rbegin(); it != this->colours_.rend(); ++it)
                {
                    colour1 = colour2;
                    value1 = value2;
                    colour2 = it->second;
                    value2 = it->first;

                    if (value2 < this->value_)
                        break;
                }

                if (value2 > this->value_)
                {
                    this->textureUnitState_->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, colour2);
                    this->currentColour_ = colour2;
                }
                else if (value1 < this->value_)
                {
                    this->textureUnitState_->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, colour1);
                    this->currentColour_ = colour1;
                }
                else
                {
                    //float interpolationfactor = (this->value_ - value2) / (value1 - value2);
                    float interpolationfactor = interpolateSmooth((this->value_ - value2) / (value1 - value2), 0.0f, 1.0f);
                    this->currentColour_ = colour1 * interpolationfactor + colour2 * (1 - interpolationfactor);
                    this->textureUnitState_->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, this->currentColour_);

                }
            }
        }

        // set value
        if (this->right2Left_)
        {
            // backward case
            this->bar_->setPosition(0.06f + 0.88f * (1 - this->value_), 0.0f);
            this->bar_->setDimensions(0.88f * this->value_, 1.0f);
        }
        else
        {
            // default case
            this->bar_->setPosition(0.06f, 0.0f);
            this->bar_->setDimensions(0.88f * this->value_, 1.0f);
        }
        if (this->value_ != 0)
            this->bar_->setTiling(this->value_, 1.0);
    }

    void HUDBar::addColour(BarColour* colour)
    {
        float value = clamp<float>(colour->getPosition(), 0.0, 1.0);
        this->colours_[value] = colour->getColour();

        this->barColours_.push_back(colour);
    }

    BarColour* HUDBar::getColour(unsigned int index)
    {
        if (index < this->barColours_.size())
            return barColours_[index];
        else
            return 0;
    }

    void HUDBar::clearColours()
    {
        this->colours_.clear();
    }

    void HUDBar::setBarTexture(const std::string& texture)
    {
        this->textureUnitState_->setTextureName(texture);
    }

    const std::string& HUDBar::getBarTexture() const
    {
        return this->textureUnitState_->getTextureName();
    }
}
