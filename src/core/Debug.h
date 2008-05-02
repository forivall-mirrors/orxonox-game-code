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

#include "CorePrereqs.h"

#include <stdio.h>

#include "OutputHandler.h"

extern "C" _CoreExport int getSoftDebugLevel();

// DEFINE ERROR MODES
#define ORX_NONE            0
#define ORX_ERROR           1
#define ORX_WARNING         2
#define ORX_INFO            3
#define ORX_DEBUG           4
#define ORX_vDEBUG          5

//definitions
#define ORX_PRINT_DEBUG_OUTPUT 1 // <-- fix that! should be a configurable setting

#define ORX_HARD_DEBUG_LEVEL ORX_vDEBUG
//#define ORX_SOFT_DEBUG_LEVEL ORX_WARNING // <-- fix that! should be a configurable setting

///////////////////////////////////////////////////
/// PRINTF: prints with filename and linenumber ///
///////////////////////////////////////////////////
#define PRINTFORX_NONE    PRINTF0
#define PRINTFORX_ERROR   PRINTF1
#define PRINTFORX_WARNING PRINTF2
#define PRINTFORX_INFO    PRINTF3
#define PRINTFORX_DEBUG   PRINTF4
#define PRINTFORX_vDEBUG  PRINTF5

#define PRINT_EXEC  printf
#define COUT_EXEC(x) \
  orxonox::OutputHandler::getOutStream().setOutputLevel(x)

#ifndef PRINTF
 #if ORX_PRINT_DEBUG_OUTPUT

  #define PRINTF(x) \
   PRINTF ## x

  #if ORX_HARD_DEBUG_LEVEL >= ORX_ERROR
   #define PRINTF1 \
    if (getSoftDebugLevel() >= ORX_ERROR) \
     printf("Error (in %s, line %d): ", __FILE__, __LINE__), PRINT_EXEC
  #else
   #define PRINTF1 if (ORX_NONE) PRINT_EXEC
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_WARNING
   #define PRINTF2 \
    if (getSoftDebugLevel() >= ORX_WARNING) \
     printf("Warning (in %s, line %d): ", __FILE__, __LINE__), PRINT_EXEC
  #else
   #define PRINTF2 if (ORX_NONE) PRINT_EXEC
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_INFO
   #define PRINTF3 \
    if (getSoftDebugLevel() >= ORX_INFO) \
     printf("Info (in %s, line %d): ", __FILE__, __LINE__), PRINT_EXEC
  #else
   #define PRINTF3 if (ORX_NONE) PRINT_EXEC
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_DEBUG
   #define PRINTF4 \
    if (getSoftDebugLevel() >= ORX_DEBUG) \
     printf("Debug (in %s, line %d): ", __FILE__, __LINE__), PRINT_EXEC
  #else
   #define PRINTF4 if (ORX_NONE) PRINT_EXEC
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_vDEBUG
   #define PRINTF5 \
    if (getSoftDebugLevel() >= ORX_vDEBUG) \
     printf("vDebug (in %s, line %d): ", __FILE__, __LINE__), PRINT_EXEC
  #else
   #define PRINTF5 if (ORX_NONE) PRINT_EXEC
  #endif

 #else /* if ORX_PRINT_DEBUG_OUTPUT */
  #define PRINTF(x) if (ORX_NONE) PRINT_EXEC
 #endif /* if ORX_PRINT_DEBUG_OUTPUT */

 #define PRINTF0 \
  PRINT_EXEC
#endif /* ifndef PRINTF */

///////////////////////////////////////////////////
///  PRINT: just prints output as is            ///
///////////////////////////////////////////////////
#define PRINTORX_NONE    PRINT0
#define PRINTORX_ERROR   PRINT1
#define PRINTORX_WARNING PRINT2
#define PRINTORX_INFO    PRINT3
#define PRINTORX_DEBUG   PRINT4
#define PRINTORX_vDEBUG  PRINT5

#ifndef PRINT
 #if ORX_PRINT_DEBUG_OUTPUT
  #define PRINT(x) \
   PRINT ## x

  #if ORX_HARD_DEBUG_LEVEL >= ORX_ERROR
   #define PRINT1  \
    if (getSoftDebugLevel() >= ORX_ERROR)  \
     PRINT_EXEC
  #else
   #define PRINT1 if (ORX_NONE) PRINT_EXEC
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_WARNING
   #define PRINT2 \
    if (getSoftDebugLevel() >= ORX_WARNING) \
     PRINT_EXEC
  #else
   #define PRINT2 if (ORX_NONE) PRINT_EXEC
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_INFO
   #define PRINT3 \
    if (getSoftDebugLevel() >= ORX_INFO) \
     PRINT_EXEC
  #else
   #define PRINT3 if (ORX_NONE) PRINT_EXEC
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_DEBUG
   #define PRINT4 \
    if (getSoftDebugLevel() >= ORX_DEBUG) \
     PRINT_EXEC
  #else
   #define PRINT4 if (ORX_NONE) PRINT_EXEC
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_vDEBUG
   #define PRINT5 \
    if (getSoftDebugLevel() >= ORX_vDEBUG) \
     PRINT_EXEC
  #else
   #define PRINT5 if (ORX_NONE) PRINT_EXEC
  #endif

 #else /* if ORX_PRINT_DEBUG_OUTPUT */
  #define PRINT(x) if (ORX_NONE) PRINT_EXEC
 #endif /* if ORX_PRINT_DEBUG_OUTPUT */

 #define PRINT0 \
  PRINT_EXEC
#endif /* ifndef PRINT */


////////////////////////////////////////////////////////
///  COUT: just prints output as is with std::cout   ///
////////////////////////////////////////////////////////
#define COUTORX_NONE    COUT0
#define COUTORX_ERROR   COUT1
#define COUTORX_WARNING COUT2
#define COUTORX_INFO    COUT3
#define COUTORX_DEBUG   COUT4
#define COUTORX_vDEBUG  COUT5

#ifndef COUT
 #if ORX_PRINT_DEBUG_OUTPUT
  #define COUT(x) \
   COUT ## x

  #if ORX_HARD_DEBUG_LEVEL >= ORX_ERROR
   #define COUT1  \
    if (getSoftDebugLevel() >= ORX_ERROR)  \
     COUT_EXEC(1)
  #else
   #define COUT1 if (ORX_NONE)\
    COUT_EXEC(1)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_WARNING
   #define COUT2 \
    if (getSoftDebugLevel() >= ORX_WARNING) \
     COUT_EXEC(2)
  #else
   #define COUT2 if (ORX_NONE) \
    COUT_EXEC(2)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_INFO
   #define COUT3 \
    if (getSoftDebugLevel() >= ORX_INFO) \
     COUT_EXEC(3)
  #else
   #define COUT3 if (ORX_NONE) \
    COUT_EXEC(3)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_DEBUG
   #define COUT4 \
    if (getSoftDebugLevel() >= ORX_DEBUG) \
     COUT_EXEC(4)
  #else
   #define COUT4 if (ORX_NONE) \
    COUT_EXEC(4)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_vDEBUG
   #define COUT5 \
    if (getSoftDebugLevel() >= ORX_vDEBUG) \
     COUT_EXEC(5)
  #else
   #define COUT5 if (ORX_NONE) \
    COUT_EXEC(5)
  #endif

 #else /* if ORX_PRINT_DEBUG_OUTPUT */
  #define COUT(x) if (ORX_NONE) \
   COUT_EXEC(5)
 #endif /* if ORX_PRINT_DEBUG_OUTPUT */

 #define COUT0 \
  COUT_EXEC(0)
#endif /* ifndef COUT */


/////////////////////////////////////////////////////////////////////
///  CCOUT: Prints output with std::cout and adds the classname   ///
/////////////////////////////////////////////////////////////////////
#define CCOUTORX_NONE    CCOUT0
#define CCOUTORX_ERROR   CCOUT1
#define CCOUTORX_WARNING CCOUT2
#define CCOUTORX_INFO    CCOUT3
#define CCOUTORX_DEBUG   CCOUT4
#define CCOUTORX_vDEBUG  CCOUT5

#define CCOUT_EXEC(x) \
  orxonox::OutputHandler::getOutStream().setOutputLevel(x) \
  << "*** " << this->getIdentifier()->getName() << ": "

#ifndef CCOUT
 #if ORX_PRINT_DEBUG_OUTPUT
  #define CCOUT(x) \
   CCOUT ## x

  #if ORX_HARD_DEBUG_LEVEL >= ORX_ERROR
   #define CCOUT1  \
    if (getSoftDebugLevel() >= ORX_ERROR)  \
     CCOUT_EXEC(1)
  #else
   #define CCOUT1 if (ORX_NONE)\
    CCOUT_EXEC(1)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_WARNING
   #define CCOUT2 \
    if (getSoftDebugLevel() >= ORX_WARNING) \
     CCOUT_EXEC(2)
  #else
   #define CCOUT2 if (ORX_NONE) \
    CCOUT_EXEC(2)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_INFO
   #define CCOUT3 \
    if (getSoftDebugLevel() >= ORX_INFO) \
     CCOUT_EXEC(3)
  #else
   #define CCOUT3 if (ORX_NONE) \
    CCOUT_EXEC(3)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_DEBUG
   #define CCOUT4 \
    if (getSoftDebugLevel() >= ORX_DEBUG) \
     CCOUT_EXEC(4)
  #else
   #define CCOUT4 if (ORX_NONE) \
    CCOUT_EXEC(4)
  #endif

  #if ORX_HARD_DEBUG_LEVEL >= ORX_vDEBUG
   #define CCOUT5 \
    if (getSoftDebugLevel() >= ORX_vDEBUG) \
     CCOUT_EXEC(5)
  #else
   #define CCOUT5 if (ORX_NONE) \
    CCOUT_EXEC(5)
  #endif

 #else /* if ORX_PRINT_DEBUG_OUTPUT */
  #define CCOUT(x) if (ORX_NONE) \
   CCOUT_EXEC(5)
 #endif /* if ORX_PRINT_DEBUG_OUTPUT */

 #define CCOUT0 \
  CCOUT_EXEC(0)
#endif /* ifndef CCOUT */

#endif /* _Debug_H__ */
