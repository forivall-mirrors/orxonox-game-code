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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
    @defgroup CmdArgs Commandline arguments
    @ingroup Config
    @brief For a reference of all commandline arguments see @ref cmdargspage
*/

/**
    @file
    @ingroup Config CmdArgs
    @brief Declaration of CommandLineParser and CommandLineArgument, definition of the SetCommandLineArgument() macros.
*/

#ifndef _CommandLineIncludes_H__
#define _CommandLineIncludes_H__

#include "core/CorePrereqs.h"

#include "CommandLineParser.h"
#include "core/module/StaticallyInitializedInstance.h"

#define SetCommandLineArgument(name, defaultValue) \
    orxonox::CommandLineArgument& CmdArgumentDummyBoolVar##name \
        = (new orxonox::SI_CLA(new orxonox::CommandLineArgument(#name, defaultValue)))->getArgument()
#define SetCommandLineSwitch(name) \
    SetCommandLineArgument(name, false)

namespace orxonox
{
    class _CoreExport StaticallyInitializedCommandLineArgument : public StaticallyInitializedInstance
    {
        public:
            StaticallyInitializedCommandLineArgument(CommandLineArgument* argument) : argument_(argument) {}

            virtual void load()
                { CommandLineParser::addArgument(this->argument_); }

            virtual void unload()
                { CommandLineParser::removeArgument(this->argument_); }

            inline CommandLineArgument& getArgument()
                { return *this->argument_; }

        private:
            CommandLineArgument* argument_;
    };

    typedef StaticallyInitializedCommandLineArgument SI_CLA;
}

#endif /* _CommandLineIncludes_H__ */
