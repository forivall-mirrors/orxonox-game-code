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
#include "util/ArgReader.h"
#include "core/SignalHandler.h"
#include "core/Debug.h"
#include "network/ClientConnection.h"
#include "Settings.h"
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

bool parseCommandLine(int argc, char** argv)
{
    ArgReader args;
    std::string errorStr = args.parse(argc, argv);
    if (errorStr != "")
    {
        COUT(1) << "Command Line: Parsing failed.\n" << errorStr << std::endl;
        return false;
    }

    // Argument reader parses the command line to check syntax.
    // Settings Singleton then stores the arguments. It always
    // expects a default value.
    bool success = true;
    success &= Settings::addCommandLineArgument<std::string>
        ("mode",     args.getArgument("mode"),     "standalone");
    success &= Settings::addCommandLineArgument<std::string>
        ("dataPath", args.getArgument("dataPath"), "./");
    success &= Settings::addCommandLineArgument<std::string>
        ("ip",       args.getArgument("ip"),       "127.0.0.0");
    success &= Settings::addCommandLineArgument<int>
        ("port",     args.getArgument("port"),     NETWORK_PORT);

    if (!success)
        return false;

    if (!args.allChecked())
    {
        COUT(1) << "Command Line: Parsing failed.\nNot all arguments were matched." << std::endl;
        return false;
    }

    return true;
}

#ifdef __cplusplus
extern "C" {
#endif

int main(int argc, char** argv)
{
    // create a signal handler (only works for linux)
    SignalHandler::getInstance()->doCatch(argv[0], "orxonox.log");

    if (!parseCommandLine(argc, argv))
    {
        COUT(0) << "Usage:" << std::endl << "orxonox [--mode client|server|dedicated|standalone] "
                << "[--data PATH] [--ip IP] [--port PORT]" << std::endl;
        return 0;
    }

    Orxonox orxonoxInstance;

    try
    {
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_APPLE
        orxonoxInstance.start(macBundlePath());
#else
        orxonoxInstance.start();
#endif
    }
    catch (std::exception& ex)
    {
        COUT(1) << ex.what() << std::endl;
        COUT(1) << "Abort." << std::endl;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
