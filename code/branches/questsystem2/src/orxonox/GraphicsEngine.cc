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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      Felix Schulthess
 *
 */

/**
@file
@brief
    Implementation of an partial interface to Ogre.
*/

#include "OrxonoxStableHeaders.h"
#include "GraphicsEngine.h"

#include <OgreRenderWindow.h>

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "util/Debug.h"

#include "tools/ParticleInterface.h"

namespace orxonox
{
    //SetConsoleCommand(GraphicsEngine, printScreen, true).setKeybindMode(KeybindMode::OnPress);

    GraphicsEngine* GraphicsEngine::singletonRef_s = 0;

    /**
    @brief
        Returns the singleton instance.
    @return
        The only instance of GraphicsEngine.
    */
    /*static*/ GraphicsEngine& GraphicsEngine::getInstance()
    {
        assert(singletonRef_s);
        return *singletonRef_s;
    }

    /**
    @brief
        Non-initialising constructor.
    */
    GraphicsEngine::GraphicsEngine()
//        : root_(0)
//        , renderWindow_(0)
//        , viewport_(0)
    {
        RegisterObject(GraphicsEngine);

        assert(singletonRef_s == 0);
        singletonRef_s = this;

        this->detailLevelParticle_ = 0;

        this->setConfigValues();
        CCOUT(4) << "Constructed" << std::endl;
    }

    void GraphicsEngine::setConfigValues()
    {
        SetConfigValue(detailLevelParticle_, 2).description("O: off, 1: low, 2: normal, 3: high").callback(this, &GraphicsEngine::detailLevelParticleChanged);
    }

    void GraphicsEngine::detailLevelParticleChanged()
    {
        for (ObjectList<ParticleInterface>::iterator it = ObjectList<ParticleInterface>::begin(); it; ++it)
            it->detailLevelChanged(this->detailLevelParticle_);
    }

    /**
    @brief
        Destroys all the Ogre related objects
    */
    GraphicsEngine::~GraphicsEngine()
    {
        singletonRef_s = 0;
    }
}
