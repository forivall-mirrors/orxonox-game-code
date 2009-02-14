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

/**
    @file
    @brief OS-specific implementations of the clipboard functions.
*/

#include "Clipboard.h"

#ifdef ORXONOX_PLATFORM_WINDOWS

/////////////
// Windows //
/////////////

#include <windows.h>
#include "Debug.h"

namespace orxonox
{

    /**
        @brief Puts text into the windows-clipboard
        @param text The text
        @return True if the action was successful
    */
    bool toClipboard(const std::string& text)
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
            COUT(1) << "Error: Unable to copy the following text to the clipboard:" << std::endl;
            COUT(1) << "       \"" << text << "\"" << std::endl;
        }
        return false;
    }

    /**
        @brief Gets text from the windows-clipboard if there is any text.
        @return The retrieved text
    */
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
            COUT(1) << "Error: Unable to retrieve text from the clipboard." << std::endl;
        }
        return "";
    }
}

#else /* ORXONOX_PLATFORM_WINDOWS */

/////////////
// Default //
/////////////

namespace orxonox
{
    std::string clipboard = ""; //!< Keeps the text of our internal clipboard

    /**
        @brief Default implementation if there is no OS-specific implementation or no clipboard. Copies the text into an internal clipboard.
        @param text The text
        @return True
    */
    bool toClipboard(const std::string& text)
    {
        clipboard = text;
        return true;
    }

    /**
        @brief Default implementation if there is no OS-specific implementation or no clipboard. Gets the text from the internal clipboard.
        @return The text
    */
    std::string fromClipboard()
    {
        return clipboard;
    }
}

#endif /* ORXONOX_PLATFORM_WINDOWS */
