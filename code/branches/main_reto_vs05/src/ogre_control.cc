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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
* Ogre control class.
* This is merely a convenient way to handle Ogre. It only holds the Root
* object and the render Window. These are the objects, that are independant
* of the game state (playing, menu browsing, loading, etc.).
* This class could easily be merged into the Orxnox class.
*/


#include "ogre_control.h"


/**
* Provide support for mac users.
*/
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
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

	CFStringRef cfStringRef =
        CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
	assert(cfStringRef);

	CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

	CFRelease(mainBundleURL);
	CFRelease(cfStringRef);

	return std::string(path);
}
#endif


/**
* Constructor that determines the resource path platform dependant.
*/
OgreControl::OgreControl() : root_(0)
{
	// Provide a nice cross platform solution for locating the configuration
	// files. On windows files are searched for in the current working
  // directory, on OS X however you must provide the full path, the helper
  // function macBundlePath does this for us.
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	resourcePath_ = macBundlePath() + "/Contents/Resources/";
#else
	resourcePath_ = "";
#endif
}


/**
* Standard Destructor.
*/
OgreControl::~OgreControl()
{
	if (root_)
		delete root_;
}


/* Sets up Ogre.
* First, the Root object is being created, then the resources are defined
* (not loaded!). And last but not least, the render settings (like resolution
* or AA level) are prompted to the user.
*/
bool OgreControl::initialise(void)
{
	String pluginsPath;
	// only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
	pluginsPath = resourcePath_ + "plugins.cfg";
#endif

	root_ = new Root(pluginsPath, 
		resourcePath_ + "ogre.cfg", resourcePath_ + "Ogre.log");

	setupResources();

	if (!configure())
		return false;

	return true;
}


/**
* Defines the source of the resources.
*/
void OgreControl::setupResources(void)
{
	// Load resource paths from config file
	ConfigFile cf;
	cf.load(resourcePath_ + "resources.cfg");

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app,
			// In order to make things portable on OS X we need to provide
			// the loading with it's own bundle path location
			ResourceGroupManager::getSingleton().addResourceLocation(
				String(macBundlePath() + "/" + archName), typeName, secName);
#else
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
#endif
		}
	}
}


/**
* Prompts a setting window for the render engine if that has not already
* been done.
* The method also calls the root initialiser in order to get a render window.
*/
bool OgreControl::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(!root_->restoreConfig() && !root_->showConfigDialog())
		return false;

	// user clicked OK so initialise
	// Here we choose to let the system create a default
  // rendering window by passing 'true'
	root_->saveConfig();
	window_ = root_->initialise(true);
	return true;
}


/**
* Returns the root object.
* @return Root object.
*/
Root* OgreControl::getRoot(void)
{
	return root_;
}


/**
* Returns the render window.
* @return Render window.
*/
RenderWindow* OgreControl::getRenderWindow(void)
{
	return window_;
}


/**
* Returns the resource path.
* @return Resource path.
*/
Ogre::String OgreControl::getResourcePath(void)
{
	return resourcePath_;
}
