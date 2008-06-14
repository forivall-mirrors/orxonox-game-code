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
 *
 */

#include "OrxonoxStableHeaders.h"
#include "HUDBar.h"

#include <OgreOverlayManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>

#include "util/Convert.h"

namespace orxonox
{
    unsigned int HUDBar::materialcount_s = 0;

    using namespace Ogre;

    HUDBar::HUDBar()
    {
        RegisterObject(HUDBar);

        this->bar_ = 0;
        this->background_ = 0;
        this->textureUnitState_ = 0;

        barWidth_s = 0.88f;
        barHeight_s = 1.0f;
        barOffsetLeft_s = 0.06f;
        barOffsetTop_s = 0.0f;

        this->value_ = -1;
        this->autoColour_ = true;
        this->right2Left_ = false; // default is left to right progress
    }

    HUDBar::~HUDBar()
    {
        if (this->isInitialized())
        {
            if (this->bar_)
                OverlayManager::getSingleton().destroyOverlayElement(this->bar_);
        }
    }

    void HUDBar::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        OrxonoxOverlay::XMLPort(xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            // create background
            this->background_ = static_cast<PanelOverlayElement*>(
                    OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "_Background"));
            this->background_->setMaterialName("Orxonox/BarBackground");
            this->overlay_->add2D(this->background_);

            // create new material
            std::string materialname = "barmaterial" + getConvertedValue<unsigned int, std::string>(materialcount_s++);
            Ogre::MaterialPtr material = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().create(materialname, "General");
            material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
            this->textureUnitState_ = material->getTechnique(0)->getPass(0)->createTextureUnitState();
            this->textureUnitState_->setTextureName("bar2.tga");
            // use the default colour
            this->textureUnitState_->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, ColourValue(0.2, 0.7, 0.2));

            // create bar
            this->bar_ = static_cast<PanelOverlayElement*>(OverlayManager::getSingleton().createOverlayElement("Panel", getName() + "Bar"));
            this->bar_->setMaterialName(materialname);
            this->background_->addChild(bar_);
        }

        XMLPortParamLoadOnly(HUDBar, "value", setValue, xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            this->addColour(0.7, ColourValue(0.2, 0.7, 0.2));
            this->addColour(0.4, ColourValue(0.7, 0.5, 0.2));
            this->addColour(0.1, ColourValue(0.7, 0.2, 0.2));
        }
    }

    void HUDBar::setValue(float value)
    {
        if (value == this->value_)
            return;

        this->value_ = clamp<float>(value, 0, 1);
        if (this->autoColour_ && this->textureUnitState_)
        {
            // set colour
            if (this->colours_.size() > 0)
            {
                ColourValue colour1, colour2 = (*this->colours_.rbegin()).second;
                float value1, value2 = (*this->colours_.rbegin()).first;
                for (std::map<float, ColourValue>::reverse_iterator it = this->colours_.rbegin(); it != this->colours_.rend(); ++it)
                {
                    colour1 = colour2;
                    value1 = value2;
                    colour2 = (*it).second;
                    value2 = (*it).first;

                    if (value2 < this->value_)
                        break;
                }

                if (value2 > this->value_)
                {
                    this->textureUnitState_->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, colour2);
                }
                else if (value1 < this->value_)
                {
                    this->textureUnitState_->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, colour1);
                }
                else
                {
                    //float interpolationfactor = (this->value_ - value2) / (value1 - value2);
                    float interpolationfactor = interpolateSmooth((this->value_ - value2) / (value1 - value2), 0.0f, 1.0f);
                    this->textureUnitState_->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, colour1 * interpolationfactor + colour2 * (1 - interpolationfactor));
                }
            }
        }

        // set value
        if (this->right2Left_)
        {
            // backward casew
            this->bar_->setPosition(barOffsetLeft_s + barWidth_s * (1 - this->value_), barOffsetTop_s);
            this->bar_->setDimensions(barWidth_s * this->value_, barHeight_s);
        }
        else
        {
            // default case
            this->bar_->setPosition(barOffsetLeft_s, barOffsetTop_s);
            this->bar_->setDimensions(barWidth_s * this->value_, barHeight_s);
        }
        if (this->value_ != 0)
            this->bar_->setTiling(this->value_, 1.0);
    }

    void HUDBar::addColour(float value, const ColourValue& colour)
    {
        value = clamp<float>(value, 0, 1);
        this->colours_[value] = colour;
    }

    void HUDBar::clearColours()
    {
        this->colours_.clear();
    }
}
