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
 *      Fabian 'x3n' Landau
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    Handles different output-levels of errors, warnings, infos and debug information.

    The COUT(level) macro acts like std::cout, but the output is only performed if the given
    level is <= the soft debug level.

    There are two used values in this file:
     - The hard debug level is used during compile time. It describes the highest allowed output level.
     - The soft debug level is used during runtime and is the maximum of the three configurable
       output-levels for console, log file and in game shell.

    The separation between the three devices is done by the OutputHandler.

    Possible levels are:
     0: Very important output
     1: Errors
     2: Warnings
     3: Information
     4: Debug information
     5: More debug information
     6: Crazy debug information

@example
    COUT(0) << "Very important output" << std::endl;
    COUT(1) << "Error: Something went wrong!" << std::endl;
    COUT(2) << "Warning: There might be a problem." << std::endl;
    COUT(3) << "Info: It's Monday" << std::endl;
    COUT(4) << "Debug: x is 1.23456" << std::endl;
*/

#ifndef _Util_Debug_H__
#define _Util_Debug_H__

#include "UtilPrereqs.h"
#include "OutputHandler.h"

namespace orxonox
{
    // Just for convenience
    using std::endl;

    //! Adjust to discard certain output with level > hardDebugLevel at compile time
    const int hardDebugLevel = OutputLevel::Verbose;
}

/**
@def COUT
    Logs text output: use exactly like std::cout, but specify an output
    level as argument. For example COUT(3) << "Some info" << std::endl;
@note
    The ? : operator requires both possible results to have the type of
    the first. This is achieved by the int conversion operator dummy
    in the OutputHandler.
*/
#define COUT(level)                                                    \
    /*if*/ (level > orxonox::hardDebugLevel) ?                         \
        0                                                              \
    /*else*/ :                                                         \
        /*if*/ (level > orxonox::OutputHandler::getSoftDebugLevel()) ? \
            0                                                          \
        /*else*/ :                                                     \
            orxonox::OutputHandler::getOutStream(level)

#endif /* _Util_Debug_H__ */
