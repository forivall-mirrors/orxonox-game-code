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
 *      Reto Grieder
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
#include <cassert>

#include "util/OrxonoxPlatform.h"
#include "util/Debug.h"
#include "core/ConfigFileManager.h"
#include "SignalHandler.h"

#include "gamestates/GSRoot.h"
#include "gamestates/GSGraphics.h"
#include "gamestates/GSStandalone.h"
#include "gamestates/GSServer.h"
#include "gamestates/GSClient.h"
#include "gamestates/GSDedicated.h"
#include "gamestates/GSGUI.h"
#include "gamestates/GSIOConsole.h"

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


//#ifdef __cplusplus
//extern "C" {
//#endif

int main(int argc, char** argv)
{
    // create a signal handler (only works for linux)
    SignalHandler::getInstance()->doCatch(argv[0], "orxonox.log");

    // Specifiy config file before creating the GameStates in order to have
    // setConfigValues() in the constructor (required).
    ConfigFileManager::getInstance().setFile(CFT_Settings, "orxonox.ini");

    // create the gamestates
    GSRoot root;
    GSGraphics graphics;
    GSStandalone standalone;
    GSServer server;
    GSClient client;
    GSDedicated dedicated;
    GSGUI gui;
    GSIOConsole ioConsole;

    // make the hierarchy
    root.addChild(&graphics);
    graphics.addChild(&standalone);
    graphics.addChild(&server);
    graphics.addChild(&client);
    graphics.addChild(&gui);
    root.addChild(&ioConsole);
    root.addChild(&dedicated);

    // Here happens the game
    root.start(argc, argv);

    return 0;
}

//#ifdef __cplusplus
//}
//#endif
