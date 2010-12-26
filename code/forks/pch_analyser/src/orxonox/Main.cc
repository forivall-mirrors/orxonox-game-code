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

#include <QApplication>
#include <QCoreApplication>

#include "util/Debug.h"
#include "core/CommandLineParser.h"
#include "core/Core.h"
#include "MainWindow.h"

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

        QStringList arguments = QCoreApplication::arguments();
        if (!arguments.value(0).isEmpty() && arguments.value(0)[0] != '-')
            arguments.pop_front(); // Remove application path
        Core core(arguments.join(" ").toStdString());

        QCoreApplication::setOrganizationName("Orxonox");
        QCoreApplication::setOrganizationDomain("www.orxonox.net");
        QCoreApplication::setApplicationName("Orxonox Sandbox");
        QString versionString;
        versionString += QString::number(ORXONOX_VERSION_MAJOR);
        versionString += QString::number(ORXONOX_VERSION_MINOR);
        versionString += QString::number(ORXONOX_VERSION_PATCH);
        QCoreApplication::setApplicationVersion(versionString);

        if (CommandLineParser::getValue("generateDoc").toString().isEmpty())
        {
            MainWindow window;
            window.show();
            return app.exec();
        }
        else
            return 0;
    }
}
