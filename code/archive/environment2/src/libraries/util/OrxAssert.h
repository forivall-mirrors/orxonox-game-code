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
#include "OutputHandler.h"

#ifndef NDEBUG
/** Run time assertion like assert(), but with an embedded message.
@details
    The message will be printed as error with COUT(1). <br>
    You can use the same magic here as you can with \ref ThrowException
    @code
        OrxAssert(condition, "Text: " << number << " more text");
    @endcode
*/
#define OrxAssert(Assertion, ErrorMessage) \
    Assertion ? ((void)0) : (void)(orxonox::OutputHandler::getOutStream(1) << ErrorMessage << std::endl); \
    assert(Assertion)
#else
#define OrxAssert(condition, errorMessage)  ((void)0)
#endif

#endif /* _OrxAssert_H__ */
