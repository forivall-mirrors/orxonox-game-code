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
@brief
    The main function of Orxonox (but not the entry point of the program!)
*/

#include "Main.h"

#include <memory>
#include <QApplication>
#include <QCoreApplication>

#ifdef ORXONOX_PLATFORM_WINDOWS
#  include <windows.h>
#endif

#include "util/Debug.h"
#include "util/Exception.h"
#include "core/CommandLineParser.h"
#include "core/Core.h"
#include "Analyser.h"

namespace orxonox
{
    SetCommandLineArgument(generateDoc, "")
        .information("Generates a Doxygen file from things like SetConsoleCommand");

    /**
    @brief
        Starting point of orxonox (however not the entry point of the program!)
    */
    int main(int argc, char** argv)
    {
        QApplication app(argc, argv);

        std::auto_ptr<Core> core;
        try
        {
            QStringList arguments = QCoreApplication::arguments();
            if (!arguments.value(0).isEmpty() && arguments.value(0)[0] != '-')
                arguments.pop_front(); // Remove application path
            core.reset(new Core(arguments.join(" ").toStdString()));
        }
        catch (const Exception& ex)
        {
            COUT(0) << "Exception: " << ex.getDescription() << endl;
#ifdef ORXONOX_PLATFORM_WINDOWS
            MessageBox(NULL, ex.getDescription().c_str(), "Exception", MB_ICONERROR);
#endif
            return 1;
        }

        QCoreApplication::setOrganizationName("Orxonox");
        QCoreApplication::setOrganizationDomain("www.orxonox.net");
        QCoreApplication::setApplicationName("Orxonox Sandbox");
        QString versionString;
        versionString += QString::number(ORXONOX_VERSION_MAJOR);
        versionString += QString::number(ORXONOX_VERSION_MINOR);
        versionString += QString::number(ORXONOX_VERSION_PATCH);
        QCoreApplication::setApplicationVersion(versionString);

        // Define library path
        // Note: May not work (untested) because QApplication was already created.
        // However doing the beforehand is difficult because the Core is required.
        QStringList libraryPaths = QCoreApplication::libraryPaths();
        libraryPaths.prepend(PathConfig::getExecutablePath().path() + "/plugins");
        QCoreApplication::setLibraryPaths(libraryPaths);

        try
        {
            parse();
            return 0;
        }
        catch (const Exception& ex)
        {
            COUT(0) << "Exception: " << ex.getDescription() << endl;
#ifdef ORXONOX_PLATFORM_WINDOWS
            MessageBox(NULL, ex.getDescription().c_str(), "Exception", MB_ICONERROR);
#endif
            return 1;
        }
    }
}
