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

#include "OrxonoxStableHeaders.h"
#include "Stats.h"

#include <string>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreFontManager.h>
#include <OgreFont.h>

#include "util/Math.h"
#include "util/Convert.h"
#include "util/Debug.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "core/input/InputBuffer.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    //CreateFactory(Stats);

    /**
        @brief Constructor: Creates and initializes the Stats panel.
    */
    Stats::Stats(BaseObject* creator)
        : OrxonoxOverlay(creator)
        , statsOverlayNoise_(0)
        , statsOverlayBorder_(0)
    {
        RegisterObject(Stats);

        this->setConfigValues();
    }

    /**
        @brief Sets the config values, describing the size of the stats panel.
    */
    void Stats::setConfigValues()
    {
        SetConfigValue(noiseSize_, 1.0f);
    }

    /**
        @brief Initializes the Stats panel.
    */
    void Stats::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        OrxonoxOverlay::XMLPort(xmlElement, mode);

        // create overlay and elements
        Ogre::OverlayManager* ovMan = Ogre::OverlayManager::getSingletonPtr();

        // create BorderPanel
        this->statsOverlayBorder_ = static_cast<Ogre::BorderPanelOverlayElement*>(ovMan->createOverlayElement("BorderPanel", "StatsBorderPanel"));
        this->statsOverlayBorder_->setMaterialName("StatsCenter");
        this->statsOverlayBorder_->setBorderSize(0.05, 0.05, 0.05, 0.05);
        this->statsOverlayBorder_->setBorderMaterialName("StatsBorder");
        this->statsOverlayBorder_->setTopBorderUV(0.49, 0.0, 0.51, 0.5);
        this->statsOverlayBorder_->setTopLeftBorderUV(0.0, 0.0, 0.5, 0.5);
        this->statsOverlayBorder_->setTopRightBorderUV(0.5, 0.0, 1.0, 0.5);
        this->statsOverlayBorder_->setLeftBorderUV(0.0, 0.49, 0.5, 0.51);
        this->statsOverlayBorder_->setRightBorderUV(0.5, 0.49, 1.0, 0.5);
        this->statsOverlayBorder_->setBottomBorderUV(0.49, 0.5, 0.51, 1.0);
        this->statsOverlayBorder_->setBottomLeftBorderUV(0.0, 0.5, 0.5, 1.0);
        this->statsOverlayBorder_->setBottomRightBorderUV(0.5, 0.5, 1.0, 1.0);

        background_->addChild(statsOverlayBorder_);

        // create noise
        this->statsOverlayNoise_ = static_cast<Ogre::PanelOverlayElement*>(ovMan->createOverlayElement("Panel", "StatsNoise"));
        this->statsOverlayNoise_->setPosition(0.02,0);
        this->statsOverlayNoise_->setMaterialName("StatsNoiseSmall");
        // comment following line to disable noise
        background_->addChild(this->statsOverlayNoise_);

    }

    /**
        @brief Used to control the actual scrolling and the cursor.
    */

    void Stats::tick(float dt)
    {
        // SUPER(Stats, tick, dt);

            // this creates a flickering effect (extracts exactly 80% of the texture at a random location)
            float uRand = (rand() & 1023) / 1023.0f * 0.2f;
            float vRand = (rand() & 1023) / 1023.0f * 0.2f;
            this->statsOverlayNoise_->setUV(uRand, vRand, 0.8f + uRand, 0.8f + vRand);
    }

}
