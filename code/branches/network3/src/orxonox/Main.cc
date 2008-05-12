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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

 /**
 @file
 @brief Entry point of the program. Platform specific code.
  */

#include "OrxonoxStableHeaders.h"

#include <exception>

#include "OrxonoxPlatform.h"
#include "core/SignalHandler.h"
#include "Orxonox.h"

using namespace orxonox;
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_APPLE
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

int main(int argc, char **argv)
{
  try {
    SignalHandler::getInstance()->doCatch(argv[0], "orxonox.log");
    Orxonox* orx = Orxonox::getSingleton();

#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_APPLE
    orx->init(argc, argv, macBundlePath());
#else
    orx->init(argc, argv, "");
#endif

    orx->start();
    orx->destroySingleton();
  }
  catch (std::exception &ex)
  {
    std::cerr << "Exception:\n";
    std::cerr << ex.what() << "\n";
    return 1;
  }

  return 0;
}

#ifdef __cplusplus
}
#endif
