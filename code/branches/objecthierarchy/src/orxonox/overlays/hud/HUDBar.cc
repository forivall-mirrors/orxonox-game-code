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
#include <OgrePanelOverlayElement.h>

#include "util/Convert.h"
#include "util/String.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(BarColour);

    BarColour::BarColour(BaseObject* creator) : BaseObject(creator), position_(0.0)
    {
        RegisterObject(BarColour);
    }

    void BarColour::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(BarColour, XMLPort, xmlElement, mode);

        XMLPortParam(BarColour, "colour", setColour, getColour, xmlElement, mode)
            .defaultValues(ColourValue(1.0, 1.0, 1.0, 1.0));
        XMLPortParam(BarColour, "position", setPosition, getPosition, xmlElement, mode).defaultValues(0.0f);
    }


    unsigned int HUDBar::materialcount_s = 0;

    HUDBar::HUDBar(BaseObject* creator) : OrxonoxOverlay(creator), bar_(0), textureUnitState_(0)
    {
        RegisterObject(HUDBar);
    }

    HUDBar::~HUDBar()
    {
        if (this->bar_)
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(this->bar_);
    }

    void HUDBar::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(HUDBar, XMLPort, xmlElement, mode);

        if (mode == XMLPort::LoadObject)
        {
            // create new material
            std::string materialname = "barmaterial" + getConvertedValue<unsigned int, std::string>(materialcount_s++);
            Ogre::MaterialPtr material = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().create(materialname, "General");
            material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
            this->textureUnitState_ = material->getTechnique(0)->getPass(0)->createTextureUnitState();
            this->textureUnitState_->setTextureName("bar2.tga");
            // use the default colour
            this->textureUnitState_->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, ColourValue(0.2, 0.7, 0.2));

            this->bar_ = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton()
                .createOverlayElement("Panel", "HUDBar_bar_" + getUniqueNumberString()));
            this->bar_->setMaterialName(materialname);
            this->background_->addChild(bar_);
        }

        XMLPortParam(HUDBar, "initialValue", setValue,       getValue,       xmlElement, mode).defaultValues(0.4567654f);
        XMLPortParam(HUDBar, "rightToLeft",  setRightToLeft, getRightToLeft, xmlElement, mode).defaultValues(false);
        XMLPortParam(HUDBar, "autoColour",   setAutoColour,  getAutoColour,  xmlElement, mode).defaultValues(true);
        XMLPortObject(HUDBar, BarColour, "", addColour, getColour, xmlElement, mode);
    }

    void HUDBar::valueChanged()
    {
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
}
