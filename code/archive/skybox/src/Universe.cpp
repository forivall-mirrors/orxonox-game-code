/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  orxonox.cc
 @brief Orxonox Main File
 */

#include <Ogre.h>
#include <OIS/OIS.h>
//#include <CEGUI/CEGUI.h>
//#include <OgreCEGUIRenderer.h>

#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Universe.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
#include "windows.h"


// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
  char path[1024];
  CFBundleRef mainBundle = CFBundleGetMainBundle();
  assert(mainBundle);

  CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
  assert(mainBundleURL);

  CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
  assert(cfStringRef);

  CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

  CFRelease(mainBundleURL);
  CFRelease(cfStringRef);

  return std::string(path);
}
#endif


// Wir brauchen beliebig viele koordinaten fuer das Licht => Billboards haben gleiche Koordinaten Ogre::Vector3

Universe::Universe(int number){
	this->numberOfStars = number;
}


void Universe::createCoordinates()
			{
				int x, y, z;
				unsigned int bigsum;
				for(int u = 0; u < numberOfStars; u++){
				do{
					x = (rand() % 100000) - 50000;
					y = (rand() % 100000) - 50000;
					z = (rand() % 100000) - 50000;
					bigsum = x*x + y*y + z*z;

				 }while(bigsum < 6000*6000 or bigsum > 50000*50000);
				lightcoordinates[u][0] = x;
				lightcoordinates[u][1] = y;
				lightcoordinates[u][2] = z;
				
									}
}


	void Universe::createBillboards(Ogre::SceneManager *mgrtemp, Ogre::Camera * camtemp, Ogre::Root *mRoottemp)
	{	// wir haben viele coordinates, die wir gebrauchen koennen

	char lightnames[30];
	char lightnodenames[30];
	char billboardnames[30];
	char flarenames[30];


	for(unsigned int x = 0; x < numberOfStars; x++){
		sprintf(lightnames, "lights_%d", x);
		sprintf(lightnodenames, "lightnodes_%d", x);
		sprintf(billboardnames, "billboards_%d", x);
		sprintf(flarenames, "Flare%d", x%2);
	

		/*Ogre::Light *light = mgrtemp->createLight(lightnames);
		light->setType(Ogre::Light::LT_POINT);
		light->setPosition(Ogre::Vector3(0, 0, 0));

		light->setDiffuseColour(1.0, 0.0, 0.0);
		light->setSpecularColour(1.0, 0.0, 0.0);
		*/
		Ogre::BillboardSet *bbs = mgrtemp->createBillboardSet(billboardnames, 1);
		bbs->createBillboard(Ogre::Vector3::ZERO, Ogre::ColourValue(1.0, 1.0, 1.0));
		bbs->setMaterialName(flarenames);

		Ogre::SceneNode *myNode;
		myNode = mgrtemp->getRootSceneNode()->createChildSceneNode(lightnodenames, Ogre::Vector3(lightcoordinates[x][0],lightcoordinates[x][1],lightcoordinates[x][2]));
		myNode->attachObject(bbs);
		//myNode->attachObject(light);
		//light->setPosition(0.0, 0.0, 0.0);
		}

	}




