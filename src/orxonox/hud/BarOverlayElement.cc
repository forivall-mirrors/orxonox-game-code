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
 *
 */

#include "OrxonoxStableHeaders.h"
#include "BarOverlayElement.h"

#include <OgreOverlayManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>

#include "GraphicsEngine.h"
#include "util/Convert.h"

namespace orxonox
{
    using namespace Ogre;

    unsigned int BarOverlayElement::materialcount_s = 0;

    BarOverlayElement::BarOverlayElement(const String& name) : PanelOverlayElement(name)
    {
        this->textureUnitState_ = 0;
        this->name_ = name;
        this->widthratio_ = 100.0f / 800.0f; // calculates the ratio (backgroundwidth - barwidth) / backgroundwidth
    }

    BarOverlayElement::~BarOverlayElement()
    {
        OverlayManager::getSingleton().destroyOverlayElement(this->background_);
    }

    void BarOverlayElement::init(Real leftRel, Real topRel, Real dimRel, OverlayContainer* container)
    {
        // init some values...
        this->value_ = -1;
        this->autoColour_ = true;
        this->right2Left_ = false; // default is left to right progress
        this->leftRel_ = leftRel;
        this->topRel_ = topRel;
        this->dimRel_ = dimRel;

        // create background...
        this->background_ = static_cast<OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement("Panel", name_+"container"));
        this->background_->show();
        container->addChild(background_);
        this->background_->setMetricsMode(GMM_PIXELS);
        this->background_->setMaterialName("Orxonox/BarBackground");

        // calculate absolute coordinates...
        this->resize();
        this->show();

        // create new material
        std::string materialname = "barmaterial" + getConvertedValue<unsigned int, std::string>(BarOverlayElement::materialcount_s++);
        Ogre::MaterialPtr material = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().create(materialname, "General");
        material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        this->textureUnitState_ = material->getTechnique(0)->getPass(0)->createTextureUnitState();
        this->textureUnitState_->setTextureName("bar2.tga");
        // use the default colour
        this->textureUnitState_->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, ColourValue(0.2, 0.7, 0.2));
        this->setMaterialName(materialname);
        this->setMetricsMode(GMM_PIXELS);

        this->background_->addChild(this);
    }

    void BarOverlayElement::resize()
    {
        // calculate new absolute coordinates...
        this->left_ = (int) (this->leftRel_ * GraphicsEngine::getSingleton().getWindowWidth());
        this->top_ = (int) (this->topRel_ * GraphicsEngine::getSingleton().getWindowHeight());
        this->width_ = (int) (this->dimRel_ * GraphicsEngine::getSingleton().getWindowWidth());
        this->height_ = (int) (0.1 * this->width_);	// the texture has dimensions height:length = 1:10
        this->offset_ = (int) (this->width_ * this->widthratio_ * 0.5);
        this->barwidth_ = (int) (this->width_ * (1.0f - this->widthratio_));

        // adapt background
        this->background_->setPosition(this->left_, this->top_);
        this->background_->setDimensions(this->width_,this-> height_);
        // adapt bar
        this->setValue(this->value_);
    }

    void BarOverlayElement::setValue(float value)
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
            // backward case
            this->setPosition(this->offset_ + this->barwidth_ * (1 - this->value_), 0);
            this->setDimensions(this->barwidth_ * this->value_, this->height_);
        }
        else
        {
            // default case
            this->setPosition(this->offset_, 0);
            this->setDimensions(this->barwidth_ * this->value_, this->height_);
        }
        if (this->value_ != 0)
            this->setTiling(this->value_, 1.0);
    }

    void BarOverlayElement::addColour(float value, const ColourValue& colour)
    {
        value = clamp<float>(value, 0, 1);
        this->colours_[value] = colour;
    }

    void BarOverlayElement::clearColours()
    {
        this->colours_.clear();
    }
}
