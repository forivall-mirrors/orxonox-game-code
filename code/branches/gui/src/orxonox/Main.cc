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
#include "core/SignalHandler.h"
#include "core/Debug.h"

#include "gamestates/GSRoot.h"
#include "gamestates/GSGraphics.h"
#include "gamestates/GSLevel.h"
#include "gamestates/GSGUI.h"

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

int main(int argc, char** argv)
{
    // create a signal handler (only works for linux)
    SignalHandler::getInstance()->doCatch(argv[0], "orxonox.log");


    /*GameState* state1 = new GameState("state1");
    GameState* state2 = new GameState("state2");
    GameState* state3 = new GameState("state3");
    GameState* state4 = new GameState("state4");
    GameState* state5 = new GameState("state5");
    GameState* state6 = new GameState("state6");

    state1->addChild(state4);
    state1->addChild(state6);
    state2->addChild(state3);
    state2->addChild(state5);
    state6->addChild(state2);

    state6->removeChild("state2");

    state5->requestState("state3");
    COUT(2) << std::endl;
    state2->requestState("state2");
    COUT(2) << std::endl;
    state2->requestState("state1");
    COUT(2) << std::endl;
    state4->requestState("state3");
    COUT(2) << std::endl;
    state1->requestState("state4");
    COUT(2) << std::endl;
    state1->requestState("state2");
    COUT(2) << std::endl;
    state1->requestState("stat");
    COUT(2) << std::endl;
    state1->requestState("state5");
    COUT(2) << std::endl;*/


    GSRoot root;
    GSGraphics graphics;
    GSLevel level;
    GSGUI gui;

    root.addChild(&graphics);
    graphics.addChild(&level);
    graphics.addChild(&gui);

    root.feedCommandLine(argc, argv);
    root.requestState("root");
    root.tick(0.0f);
    root.requestState("");

    return 0;
}

#ifdef __cplusplus
}
#endif
