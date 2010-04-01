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
 *      Gion-Andri Cantieni
 *   Co-authors:
 *      ...
 *
 */

#include "SkyboxGenerator.h"

#include <string>
#include <cassert>
#include <OgreRenderWindow.h>

#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/ScopedSingletonManager.h"
#include "controllers/HumanController.h"
#include "worldentities/CameraPosition.h"
#include "worldentities/ControllableEntity.h"
#include "core/GraphicsManager.h"
#include "core/CommandExecutor.h"


 
namespace orxonox
{

    SetConsoleCommand(SkyboxGenerator, createSkybox, true);

    ManageScopedSingleton(SkyboxGenerator, ScopeID::Graphics, false);

    SkyboxGenerator::SkyboxGenerator() : iterateOverDirections_(0)
    {
        RegisterRootObject(SkyboxGenerator);

        this->setConfigValues();
        takeScreenshot_ = false;
    }

    SkyboxGenerator::~SkyboxGenerator()
    {

    }

    void SkyboxGenerator::setConfigValues( ) 
    {
        SetConfigValue(skyboxPrefix_, "SkyboxFile_");
    }

    void SkyboxGenerator::tick(float dt)
    {
        if( takeScreenshot_ == true )
        {
            ControllableEntity* ce = HumanController::getLocalControllerSingleton()->getControllableEntity();
            assert(ce);
        
            Ogre::RenderWindow* w = GraphicsManager::getInstance().getRenderWindow();



            switch (iterateOverDirections_) 
            {
            case 0 :
                CommandExecutor::execute("pause");
                //w->writeContentsToFile(skyboxPrefix_+"FR.png");
                w->writeContentsToFile(skyboxPrefix_+"0.png");
                ce->yaw(Degree(90));
                iterateOverDirections_++;
                break;
                
            case 1 :
                //w->writeContentsToFile(skyboxPrefix_+"LF.png");
                w->writeContentsToFile(skyboxPrefix_+"1.png");
                ce->yaw(Degree(90)); 
                iterateOverDirections_++;
                break;

            case 2 :
                //w->writeContentsToFile(skyboxPrefix_+"BK.png");
                w->writeContentsToFile(skyboxPrefix_+"2.png");
                ce->yaw(Degree(90)); 
                iterateOverDirections_++;
                break;

            case 3 :
                //w->writeContentsToFile(skyboxPrefix_+"RT.png");
                w->writeContentsToFile(skyboxPrefix_+"3.png");
                ce->yaw(Degree(90)); 
                ce->pitch(Degree(90)); 
                iterateOverDirections_++;
                break;

            case 4 :
                //w->writeContentsToFile(skyboxPrefix_+"UP.png");
                w->writeContentsToFile(skyboxPrefix_+"4.png");
                ce->pitch(Degree(180)); 
                iterateOverDirections_++;
                break;

            case 5 :
                //w->writeContentsToFile(skyboxPrefix_+"DN.png");
                w->writeContentsToFile(skyboxPrefix_+"5.png");
                ce->pitch(Degree(90));
                iterateOverDirections_ =0;
                takeScreenshot_ = false;
                CommandExecutor::execute("pause");

            }
        }
    }
	
	void SkyboxGenerator::createSkybox( ) 
	{

        SkyboxGenerator::getInstance().takeScreenshot_ = true;
        

    
	}
}
