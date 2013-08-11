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
 *      Felix Schulthess
 *   Co-authors:
 *      Fabian 'x3n' Landau, Benjamin Hildebrandt
 *
 */

#include "Stats.h"

#include <string>
#include <OgreOverlayManager.h>
#include <OgreBorderPanelOverlayElement.h>

#include "util/StringUtils.h"
#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"

namespace orxonox
{
    RegisterClass(Stats);

    /**
        @brief Constructor: Creates and initializes the Stats panel.
    */
    Stats::Stats(Context* context)
        : OrxonoxOverlay(context)
        , statsOverlayNoise_(0)
        , statsOverlayBorder_(0)
    {
        RegisterObject(Stats);

        this->setConfigValues();

        // create overlay and elements
        Ogre::OverlayManager* ovMan = Ogre::OverlayManager::getSingletonPtr();

        // create BorderPanel
        this->statsOverlayBorder_ = static_cast<Ogre::BorderPanelOverlayElement*>(ovMan->createOverlayElement("BorderPanel", "StatsBorderPanel" + getUniqueNumberString()));
        //this->statsOverlayBorder_->setMaterialName("StatsCenter");
        this->statsOverlayBorder_->setBorderSize(0.003f, 16.0f * 0.003f);
        this->statsOverlayBorder_->setBorderMaterialName("StatsBorder");
        this->statsOverlayBorder_->setTopBorderUV(0.49f, 0.0f, 0.51f, 0.5f);
        this->statsOverlayBorder_->setTopLeftBorderUV(0.0f, 0.0f, 0.5f, 0.5f);
        this->statsOverlayBorder_->setTopRightBorderUV(0.5f, 0.0f, 1.0f, 0.5f);
        this->statsOverlayBorder_->setLeftBorderUV(0.0f, 0.49f, 0.5f, 0.51f);
        this->statsOverlayBorder_->setRightBorderUV(0.5f, 0.49f, 1.0f, 0.5f);
        this->statsOverlayBorder_->setBottomBorderUV(0.49f, 0.5f, 0.51f, 1.0f);
        this->statsOverlayBorder_->setBottomLeftBorderUV(0.0f, 0.5f, 0.5f, 1.0f);
        this->statsOverlayBorder_->setBottomRightBorderUV(0.5f, 0.5f, 1.0f, 1.0f);

        background_->addChild(statsOverlayBorder_);

        // create noise
        //this->statsOverlayNoise_ = static_cast<Ogre::PanelOverlayElement*>(ovMan->createOverlayElement("Panel", "StatsNoise" + getUniqueNumberString()));
        //this->statsOverlayNoise_->setPosition(0,0);
        //this->statsOverlayNoise_->setMaterialName("StatsNoiseSmall");
        // comment following line to disable noise
        //background_->addChild(this->statsOverlayNoise_);
    }

    Stats::~Stats()
    {
        if (this->isInitialized())
        {
            Ogre::OverlayManager* ovMan = Ogre::OverlayManager::getSingletonPtr();
            ovMan->destroyOverlayElement(this->statsOverlayBorder_);
            //ovMan->destroyOverlayElement(this->statsOverlayNoise_);
        }
    }

    /**
        @brief Sets the config values, describing the size of the Stats panel.
    */
    void Stats::setConfigValues()
    {
        SetConfigValue(noiseSize_, 1.0f);
    }

    void Stats::tick(float dt)
    {
        // SUPER(Stats, tick, dt);

            // this creates a flickering effect (extracts exactly 80% of the texture at a random location)
            //float uRand = (rand() & 1023) / 1023.0f * 0.2f;
            //float vRand = (rand() & 1023) / 1023.0f * 0.2f;
            //this->statsOverlayNoise_->setUV(uRand, vRand, 7.5f + uRand, 7.5f + vRand);
    }

}
