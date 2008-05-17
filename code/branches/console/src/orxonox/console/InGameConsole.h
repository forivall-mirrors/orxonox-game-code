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
 *      ...
 *
 */

#ifndef _InGameConsole_H__
#define _InGameConsole_H__


#include "OrxonoxPrereqs.h"
#include <OgrePrerequisites.h>
#include <OgreBorderPanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>

#include "core/Tickable.h"
#include "core/Shell.h"


namespace orxonox
{
    class _OrxonoxExport InGameConsole : public Tickable, public ShellListener
    {
        public:
            static InGameConsole& getInstance();

            void setConfigValues();

            virtual void linesChanged();
            virtual void onlyLastLineChanged();
            virtual void lineAdded();
            virtual void inputChanged();
            virtual void cursorChanged();
            virtual void exit();

            void tick(float dt);

            void activate();
            void deactivate();

            static void openConsole();
            static void closeConsole();

        private:
            InGameConsole();
            InGameConsole(const InGameConsole& other);
            ~InGameConsole();

            void init();
            void resize();
            void print(Ogre::UTFString s);
            void newline();
            static Ogre::UTFString convert2UTF(std::string s);

            static float REL_WIDTH;
            static float REL_HEIGHT;
            static float BLINK;

            int windowW_;
            int windowH_;
            int scroll_;
            float scrollTimer_;
            float cursor_;
            bool active_;
            Ogre::OverlayManager* om_;
            Ogre::Overlay* consoleOverlay_;
            Ogre::OverlayContainer* consoleOverlayContainer_;
            Ogre::PanelOverlayElement* consoleOverlayNoise_;
            Ogre::BorderPanelOverlayElement* consoleOverlayBorder_;
            Ogre::TextAreaOverlayElement** consoleOverlayTextAreas_;
    };
}

#endif /* _InGameConsole_H__ */
