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

#ifndef _GSGraphics_H__
#define _GSGraphics_H__

#include "OrxonoxPrereqs.h"
#include "core/GameState.h"
#include "tools/WindowEventListener.h"

namespace orxonox
{
    class _OrxonoxExport GSGraphics : public GameState, public WindowEventListener
    {
    public:
        GSGraphics(const std::string& name);
        ~GSGraphics();
        void setConfigValues();

        void activate();
        void deactivate();
        void update(const Clock& time);

    private:
        // Window events from WindowEventListener
        void windowResized(unsigned int newWidth, unsigned int newHeight);
        void windowFocusChanged();

        // managed singletons
        InputManager*         inputManager_;
        InGameConsole*        console_;
        GUIManager*           guiManager_;
        GraphicsManager*      graphicsManager_;       //!< Interface to Ogre

        KeyBinder*            masterKeyBinder_;
        SimpleInputState*     masterInputState_;
        XMLFile*              debugOverlay_;
    };
}

#endif /* _GSGraphics_H__ */
