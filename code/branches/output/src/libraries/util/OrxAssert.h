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
@file
@ingroup ExceptionAssertion
@brief
    Declaration of custom assertion facilities
*/

#ifndef _OrxAssert_H__
#define _OrxAssert_H__

#include "UtilPrereqs.h"

#include <cassert>
#include "Output.h"

#ifndef NDEBUG

/** Run time assertion like assert(), but with an embedded message.
@details
    The message will be printed as error with orxout(user_error). <br>
    You can use the same magic here as you can with \ref ThrowException
    @code
        OrxAssert(condition, "Text: " << number << " more text");
    @endcode
*/
#define OrxAssert(condition, errorMessage) \
    do \
    { \
        if (!(condition)) \
        { \
            orxout(user_error) << __FILE__ << "(" << __LINE__ << "): "; \
            orxout(user_error) << "Assertion failed in " << __FUNCTIONNAME__ << endl; \
            orxout(user_error) << "Expression: " << #condition << endl; \
            orxout(user_error) << "Message   : " << errorMessage << endl; \
            /* Don't use the condition again to avoid double evaluation */ \
            /* Instead, stringify the expression and negate it */ \
            assert(!#condition); \
        } \
    } while (false)

/** Works like OrxAssert in debug mode, but also checks the condition in release
    mode (no abort() triggered then).
@details
    The message will be printed as error with orxout(user_error). <br>
    You can use the same magic here as you can with \ref ThrowException
    @code
        OrxVerify(condition, "Text: " << number << " more text");
    @endcode
*/
#define OrxVerify(condition, errorMessage) \
    do \
    { \
        if (!(condition)) \
        { \
            orxout(user_error) << __FILE__ << "(" << __LINE__ << "): "; \
            orxout(user_error) << "Verification failed in " << __FUNCTIONNAME__ << endl; \
            orxout(user_error) << "Expression: " << #condition << endl; \
            orxout(user_error) << "Message   : " << errorMessage << endl; \
            /* Don't use the condition again to avoid double evaluation */ \
            /* Instead, stringify the expression and negate it */ \
            assert(!#condition); \
        } \
    } while (false)

#else

#define OrxAssert(condition, errorMessage)  ((void)0)

#define OrxVerify(condition, errorMessage) \
    do \
    { \
        if (!(condition)) \
        { \
            orxout(user_error) << __FILE__ << "(" << __LINE__ << "): "; \
            orxout(user_error) << "Verification failed in " << __FUNCTIONNAME__ << endl; \
            orxout(user_error) << "Expression: " << #condition << endl; \
            orxout(user_error) << "Message   : " << errorMessage << endl; \
            /* No assert() in release configuration */ \
        } \
    } while (false)

#endif

#endif /* _OrxAssert_H__ */
