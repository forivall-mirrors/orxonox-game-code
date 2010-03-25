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
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/ScopedSingletonManager.h"
 
namespace orxonox
{

    SetConsoleCommand(SkyboxGenerator, createSkybox, true);

    ManageScopedSingleton(SkyboxGenerator, ScopeID::Graphics, false);

    SkyboxGenerator::SkyboxGenerator()
    {
        RegisterRootObject(SkyboxGenerator);

        this->setConfigValues();
    }

    SkyboxGenerator::~SkyboxGenerator()
    {

    }

    void SkyboxGenerator::setConfigValues( ) 
    {
        SetConfigValue(skyboxPrefix_, "SkyboxFile_");
    }
	
	void SkyboxGenerator::createSkybox( ) 
	{
        //SkyboxGenerator* sbg = SkyboxGenerator::getInstancePtr();
        //sbg->skyboxPrefix_;

		/*unsigned int indice = 1; 
	    char filename[30]; 
	    char fn[1]; 
	    // generate new names... 
	    //sprintf(filename, "screenshot_%d.png", ++indice); 
	    //sprintf(filename, fn, ++indice); 
	    */ 

	     /*
	    if(stopper == 1){ 
	    //sprintf(filename, "stevecube_FR.bmp"); 
	    mWindow->writeContentsToFile("MySky_FR.bmp");    
	    } 
	    else if(stopper == 2){ 
	    //sprintf(filename, "stevecube_LF.bmp"); 
	        cam->pitch(Ogre::Degree(90)); 
	    mWindow->writeContentsToFile("MySky_LF.bmp"); 
	    } 
	    else if(stopper == 3){ 
	    //sprintf(filename, "stevecube_BK.bmp"); 
	        cam->pitch(Ogre::Degree(90)); 
	    mWindow->writeContentsToFile("MySky_BK.bmp"); 
	    } 
	    else if(stopper == 4){ 
	    //sprintf(filename, "stevecube_RT.bmp"); 
	        cam->pitch(Ogre::Degree(90)); 
	    mWindow->writeContentsToFile("MySky_RT.bmp"); 
	    } 
	    else if(stopper == 5){ 
	    //sprintf(filename, "stevecube_UP.bmp"); 
	        cam->yaw(Ogre::Degree(90)); 
	    mWindow->writeContentsToFile("MySky_UP.bmp"); 
	    } 
	    else if(stopper == 6){ 
	        cam->yaw(Ogre::Degree(-90)); 
	    mWindow->writeContentsToFile("MySky_DN.bmp"); 
	    } 
	 
	    stopper+=1; 
	    if(stopper >= 7) 
	    stopper = 1; 
            */
	}
}
