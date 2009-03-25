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
 *      Fabian 'x3n' Landau
 *
 */

#include "OrxonoxStableHeaders.h"
#include "GSStandalone.h"

#include <OgreViewport.h>
#include <OgreCamera.h>
#include "core/Game.h"
#include "core/GameMode.h"
#include "core/ConsoleCommand.h"
#include "gui/GUIManager.h"
#include "GraphicsManager.h"

namespace orxonox
{
    AddGameState(GSStandalone, "standalone");

    GSStandalone::GSStandalone(const std::string& name)
        : GameState(name)
    {
    }

    GSStandalone::~GSStandalone()
    {
    }


    void GSStandalone::activate()
    {
        GameMode::setIsStandalone(true);

        guiManager_ = GUIManager::getInstancePtr();
        // not sure if necessary
        // guiManager_->loadScene("IngameMenu");
    }

    void GSStandalone::deactivate()
    {
        GameMode::setIsStandalone(false);
    }

    void GSStandalone::update(const Clock& time)
    {
        //Ogre::Viewport* viewport = GraphicsManager::getInstance().getViewport();
        //COUT(0) << "** " << viewport->getCamera()->getSceneManager() << std::endl;
        //guiManager_->testFct();
        //Ogre::Viewport* viewport = GraphicsManager::getInstance().getViewport();
        //guiManager_->showGUI("IngameMenu", viewport->getCamera()->getSceneManager());

        // tick CEGUI
        guiManager_->update(time);
    }
}
