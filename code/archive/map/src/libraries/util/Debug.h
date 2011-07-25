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

    // Adjust this to discard certain output with level > hardDebugLevel at compile time already
#ifdef ORXONOX_RELEASE
    const int hardDebugLevel = OutputLevel::Verbose;
#elif defined(NDEBUG)
    const int hardDebugLevel = OutputLevel::Verbose;
#else
    //! Maximum level for debug output that should be even processed at run time
    const int hardDebugLevel = OutputLevel::Ultra;
#endif

    //! This function simply returns 0 and helps to suppress the "statement has no effect" compiler warning
    inline int debugDummyFunction()
    {
        return 0;
    }
}

/**
@brief
    Logs text output: use exactly like std::cout, but specify an output
    level as argument.
@details
    (a > b ? 0 : c << "text") is equivalent to (a > b ? 0 : (c << "text"))
    where (a > b ? 0 : ) stands for COUT(x). This should explain how
    this macro magic can possibly even work ;)
@example
    COUT(3) << "Some info" << std::endl;
@note
    The ? : operator requires both possible results to have the type of
    the first. This is achieved by the int conversion operator dummy
    in the OutputHandler.
*/
#define COUT(level)                                                    \
    /*if*/ (level > orxonox::hardDebugLevel) ?                         \
        orxonox::debugDummyFunction()                                  \
    /*else*/ :                                                         \
        /*if*/ (level > orxonox::OutputHandler::getSoftDebugLevel()) ? \
            orxonox::debugDummyFunction()                              \
        /*else*/ :                                                     \
            orxonox::OutputHandler::getOutStream(level)

#endif /* _Util_Debug_H__ */
