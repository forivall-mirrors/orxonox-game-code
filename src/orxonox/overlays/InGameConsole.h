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
 *      Felix Schulthess
 *   Co-authors:
 *      Fabian 'x3n' Landau
 *
 */

#ifndef _InGameConsole_H__
#define _InGameConsole_H__


#include "OrxonoxPrereqs.h"

#include <string>

#include "util/OgreForwardRefs.h"
#include "util/Singleton.h"
#include "core/WindowEventListener.h"
#include "core/command/Shell.h"

namespace orxonox
{
    class _OrxonoxExport InGameConsole : public Singleton<InGameConsole>, public ShellListener, public WindowEventListener
    {
        friend class Singleton<InGameConsole>;
    public: // functions
        InGameConsole();
        ~InGameConsole();

        void initialise();
        void setConfigValues();

        void preUpdate(const Clock& time);

        static void openConsole();
        static void closeConsole();

    private: // functions
        InGameConsole(const InGameConsole& other);

        void activate();
        void deactivate();

        void linesChanged();
        void lineAdded();
        void inputChanged();
        void cursorChanged();
        void executed();
        void exit();

        void shiftLines();
        void colourLine(Shell::LineType type, int index);
        void setCursorPosition(unsigned int pos);
        void print(const std::string& text, Shell::LineType type, int index, bool alwaysShift = false);

        void windowResized(unsigned int newWidth, unsigned int newHeight);

        // config value related
        void bHidesAllInputChanged();

    private: // variables
        Shell* shell_;
        bool bActive_;
        int windowW_;
        int windowH_;
        int desiredTextWidth_;
        unsigned int maxCharsPerLine_;
        unsigned int numLinesShifted_;
        int scroll_;
        float cursor_;
        unsigned int inputWindowStart_;
        bool bShowCursor_;
        std::string displayedText_;
        Ogre::Overlay* consoleOverlay_;
        Ogre::OverlayContainer* consoleOverlayContainer_;
        Ogre::PanelOverlayElement* consoleOverlayNoise_;
        Ogre::TextAreaOverlayElement* consoleOverlayCursor_;
        Ogre::BorderPanelOverlayElement* consoleOverlayBorder_;
        Ogre::TextAreaOverlayElement** consoleOverlayTextAreas_;

        // input related
        InputState* inputState_;

        // config values
        float relativeWidth;
        float relativeHeight;
        float blinkTime;
        float scrollSpeed_;
        float noiseSize_;
        char cursorSymbol_;
        bool bHidesAllInput_;

        static InGameConsole* singletonPtr_s;
    };
}

#endif /* _InGameConsole_H__ */
