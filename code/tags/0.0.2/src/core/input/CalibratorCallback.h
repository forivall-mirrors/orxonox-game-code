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
@brief
    Different definitions of input processing.
*/

#ifndef _CalibratorCallback_H__
#define _CalibratorCallback_H__

#include "core/CorePrereqs.h"
#include "InputInterfaces.h"

namespace orxonox
{
    class _CoreExport CalibratorCallback : public KeyHandler
    {
    public:
        CalibratorCallback()  { }
        ~CalibratorCallback() { }

    private:
        void keyPressed (const KeyEvent& evt);
        void keyReleased(const KeyEvent& evt) { }
        void keyHeld    (const KeyEvent& evt) { }

        void tickInput(float dt) { }
    };
}

#endif /* _CalibratorCallback_H__ */