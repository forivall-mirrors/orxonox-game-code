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
 *      Benjamin Grauer
 *   Co-authors:
 *      Fabian 'x3n' Landau
 *
 */

/**
 * @file Debug.h
 * @brief Handles the output for different verbose-modes.
 *
 * There are two modes: HARD and SOFT. HARD is precessed during compiletime, while SOFT is for runtime.
 */

#ifndef _Debug_H__
#define _Debug_H__

#include "UtilPrereqs.h"

#include <stdio.h>

#include "OutputHandler.h"


/**
    @brief Returns the soft debug level, stored in the only existing instance of the OutputHandler class, configured in the config-file.
    @return The soft debug level
*/
static inline int getSoftDebugLevel()
{
    return orxonox::OutputHandler::getSoftDebugLevel();
}


// DEFINE ERROR MODES
#define ORX_NONE            0
#define ORX_ERROR           1
#define ORX_WARNING         2
#define ORX_INFO            3
#define ORX_DEBUG           4
#define ORX_VERBOSE         5
#define ORX_ULTRA           6

//definitions
#define ORX_PRINT_DEBUG_OUTPUT 1
#define ORX_HARD_DEBUG_LEVEL ORX_VERBOSE

#define COUT_EXEC(x) orxonox::OutputHandler::getOutStream().setOutputLevel(x)

////////////////////////////////////////////////////////
///  COUT: just prints output as is with std::cout   ///
////////////////////////////////////////////////////////
#define COUTORX_NONE    COUT0
#define COUTORX_ERROR   COUT1
#define COUTORX_WARNING COUT2
#define COUTORX_INFO    COUT3
#define COUTORX_DEBUG   COUT4
#define COUTORX_VERBOSE COUT5
#define COUTORX_ULTRA   COUT6

#ifndef COUT
 #if ORX_PRINT_DEBUG_OUTPUT
  #define COUT(x) \
   COUT ## x

  #if ORX_HARD_DEBUG_LEVEL >= ORX_NONE
   #define COUT0  \
    (getSoftDebugLevel() < ORX_NONE) ? COUT_EXEC(0) : COUT_EXEC(0)
  #else
   #define COUT0 \
    false ? COUT_EXEC(0) : COUT_EXEC(0)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_ERROR
   #define COUT1  \
    (getSoftDebugLevel() < ORX_ERROR) ? COUT_EXEC(1) : COUT_EXEC(1)
  #else
   #define COUT1 \
    false ? COUT_EXEC(1) : COUT_EXEC(1)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_WARNING
   #define COUT2 \
    (getSoftDebugLevel() < ORX_WARNING) ? COUT_EXEC(2) : COUT_EXEC(2)
  #else
   #define COUT2 \
    false ? COUT_EXEC(2) : COUT_EXEC(2)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_INFO
   #define COUT3 \
    (getSoftDebugLevel() < ORX_INFO) ? COUT_EXEC(3) : COUT_EXEC(3)
  #else
   #define COUT3 \
    false ? COUT_EXEC(3) : COUT_EXEC(3)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_DEBUG
   #define COUT4 \
    (getSoftDebugLevel() < ORX_DEBUG) ? COUT_EXEC(4) : COUT_EXEC(4)
  #else
   #define COUT4 \
    false ? COUT_EXEC(4) : COUT_EXEC(4)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_VERBOSE
   #define COUT5 \
    (getSoftDebugLevel() < ORX_VERBOSE) ? COUT_EXEC(5) : COUT_EXEC(5)
  #else
   #define COUT5 \
    false ? COUT_EXEC(5) : COUT_EXEC(5)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_ULTRA
   #define COUT6 \
    (getSoftDebugLevel() < ORX_ULTRA) ? COUT_EXEC(6) : COUT_EXEC(6)
  #else
   #define COUT6 \
    false ? COUT_EXEC(6) : COUT_EXEC(6)
  #endif

 #else /* if ORX_PRINT_DEBUG_OUTPUT */
  #define COUT(x) \
    false ? COUT_EXEC(6) : COUT_EXEC(6)
 #endif /* if ORX_PRINT_DEBUG_OUTPUT */

#endif /* ifndef COUT */


/////////////////////////////////////////////////////////////////////
///  CCOUT: Prints output with std::cout and adds the classname   ///
/////////////////////////////////////////////////////////////////////
#define CCOUTORX_NONE    CCOUT0
#define CCOUTORX_ERROR   CCOUT1
#define CCOUTORX_WARNING CCOUT2
#define CCOUTORX_INFO    CCOUT3
#define CCOUTORX_DEBUG   CCOUT4
#define CCOUTORX_VERBOSE CCOUT5
#define CCOUTORX_ULTRA   CCOUT6

#define CCOUT_EXEC(x) \
  orxonox::OutputHandler::getOutStream().setOutputLevel(x) \
  << this->getIdentifier()->getName() << ": "

#ifndef CCOUT
 #if ORX_PRINT_DEBUG_OUTPUT
  #define CCOUT(x) \
   CCOUT ## x

  #if ORX_HARD_DEBUG_LEVEL >= ORX_NONE
   #define CCOUT0  \
    (getSoftDebugLevel() < ORX_NONE) ? COUT_EXEC(0) : CCOUT_EXEC(0)
  #else
   #define CCOUT0 \
    false ? COUT_EXEC(0) : CCOUT_EXEC(0)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_ERROR
   #define CCOUT1  \
    (getSoftDebugLevel() < ORX_ERROR) ? COUT_EXEC(1) : CCOUT_EXEC(1)
  #else
   #define CCOUT1 \
    false ? COUT_EXEC(1) : CCOUT_EXEC(1)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_WARNING
   #define CCOUT2 \
    (getSoftDebugLevel() < ORX_WARNING) ? COUT_EXEC(2) : CCOUT_EXEC(2)
  #else
   #define CCOUT2 \
    false ? COUT_EXEC(2) : CCOUT_EXEC(2)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_INFO
   #define CCOUT3 \
    (getSoftDebugLevel() < ORX_INFO) ? COUT_EXEC(3) : CCOUT_EXEC(3)
  #else
   #define CCOUT3 \
    false ? COUT_EXEC(3) : CCOUT_EXEC(3)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_DEBUG
   #define CCOUT4 \
    (getSoftDebugLevel() < ORX_DEBUG) ? COUT_EXEC(4) : CCOUT_EXEC(4)
  #else
   #define CCOUT4 \
    false ? COUT_EXEC(4) : CCOUT_EXEC(4)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_VERBOSE
   #define CCOUT5 \
    (getSoftDebugLevel() < ORX_VERBOSE) ? COUT_EXEC(5) : CCOUT_EXEC(5)
  #else
   #define CCOUT5 \
    false ? COUT_EXEC(5) : CCOUT_EXEC(5)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_ULTRA
   #define CCOUT6 \
    (getSoftDebugLevel() < ORX_ULTRA) ? COUT_EXEC(6) : CCOUT_EXEC(6)
  #else
   #define CCOUT6 \
    false ? COUT_EXEC(6) : CCOUT_EXEC(6)
  #endif

 #else /* if ORX_PRINT_DEBUG_OUTPUT */
  #define CCOUT(x) \
   false ? CCOUT_EXEC(6) : CCOUT_EXEC(6)
 #endif /* if ORX_PRINT_DEBUG_OUTPUT */

#endif /* ifndef CCOUT */

#endif /* _Debug_H__ */
