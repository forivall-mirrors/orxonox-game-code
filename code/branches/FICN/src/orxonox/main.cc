/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

 /**
 @file  main.cc
 @brief main file handling most of the machine specific code
  */

#include <OgrePlatform.h>
#include <OgreException.h>


#include "core/SignalHandler.h"
#include "orxonox.h"

using namespace orxonox;
using namespace Ogre;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

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

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
  INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
  {
    // something like this would be less hacky
    // maybe one can work with trailing '\0'
    // or maybe use string based functions
    // I was unable to test it without working windows version
    /*char* cmd = strCmdLine;
    int argc = 1;
    for(int i = 0; cmd[i] != NULL; i++)
    {
      if(cmd[i] == ' ') argc++;
    }
    char* argv[argc];
    int j = 0;
    int k = 0;
    for(int i = 0; cmd[i] != NULL; i++)
    {
      if(cmd[i] != ' ') {
        argv[j][k];
        k++;
      }
      else {
        k = 0;
        j++;
      }
    }*/
    char *argv[2];
    argv[0] = "asdfProgram";
    argv[1] =  strCmdLine;
    int argc = 2;
#else
  int main(int argc, char **argv)
  {
#endif
    try {
      SignalHandler::getInstance()->doCatch(argv[0], "orxonox.log");
      Orxonox* orx = Orxonox::getSingleton();
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
      orx->init(argc, argv, macBundlePath());
      orx->start();
#else
      orx->init(argc, argv, "");
      orx->start();
#endif
    }
    catch (Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBoxA(NULL, e.getFullDescription().c_str(),
            "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "Exception:\n";
      std::cerr << e.getFullDescription().c_str() << "\n";
#endif
      return 1;
    }
    return 0;
  }

#ifdef __cplusplus
}
#endif


/*int main(int argc, char **argv)
{
  try
  {
    SignalHandler::getInstance()->doCatch(argv[0], "orxonox.log");
    Orxonox* orx = Orxonox::getSingleton();
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    orx->init(argc, argv, macBundlePath());
    orx->start();
#else
    orx->init(argc, argv, "");
    orx->start();
#endif

  }
  catch(Ogre::Exception& e)
  {
    fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
    return 1;
  }

  return 0;
}

*/