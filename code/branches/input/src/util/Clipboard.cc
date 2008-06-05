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
 *   Windows version inspired by "Copy Text To Clipboard" by Laszlo Szathmary, 2007
 *      http://www.loria.fr/~szathmar/off/projects/C/CopyTextToClipboard/index.php
 */

#include "Clipboard.h"

#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
    #include <windows.h>

    bool toClipboard(std::string text)
    {
        try
        {
            if (OpenClipboard(0))
            {
                EmptyClipboard();
                HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE, text.size() + 1);
                char* buffer = (char*)GlobalLock(clipbuffer);
                strcpy(buffer, text.c_str());
                GlobalUnlock(clipbuffer);
                SetClipboardData(CF_TEXT, clipbuffer);
                CloseClipboard();

                return true;
            }
        }
        catch (...)
        {
        }
        return false;
    }

    std::string fromClipboard()
    {
        try
        {
            if (OpenClipboard(0))
            {
                HANDLE hData = GetClipboardData(CF_TEXT);
                std::string output = (char*)GlobalLock(hData);
                GlobalUnlock(hData);
                CloseClipboard();

                return output;
            }
        }
        catch (...)
        {
        }
        return "";
    }
#else
    std::string clipboard = "";

    bool toClipboard(std::string text)
    {
        clipboard = text;
        return true;
    }

    std::string fromClipboard()
    {
        return clipboard;
    }
#endif
