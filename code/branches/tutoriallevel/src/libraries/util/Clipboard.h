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
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @ingroup Util Command
    @brief Some functions to exchange text between the OS clipboard and the Shell in Orxonox.

    Use fromClipboard() to get text from the clipboard (if there is any text) and
    toClipboard() to put text into the clipboard.

    These functions can only work properly if there's an OS-specific implementation
    in Clipboard.cc. If a specific OS is not supported, the clipboard only works
    within Orxonox, but the exchange with other programs is not possible.
*/

#ifndef _Clipboard_H__
#define _Clipboard_H__

#include "UtilPrereqs.h"
#include <string>

namespace orxonox
{
    _UtilExport bool toClipboard(const std::string& text);
    _UtilExport std::string fromClipboard();
}

#endif /* _Clipboard_H__ */

