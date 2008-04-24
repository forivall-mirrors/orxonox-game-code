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
#include "core/InputBuffer.h"


namespace orxonox
{
    class _OrxonoxExport InGameConsole : public InputBufferListener
    {
        public:
            InGameConsole(InputBuffer* ib);
            ~InGameConsole();
            void listen();
            void execute();
            void hintandcomplete();
            void clear();
            void removeLast();
            void exit();
            void init();
            void tick(float dt);
            void activate();
            void deactivate();

        private:
            void resize();
            void print(std::string s);
            void newline();
            Ogre::UTFString convert2UTF(std::string s);

            int windowW;
            int windowH;
            int scroll;
            float scrollTimer;
            float cursor;
            bool active;
            InputBuffer* ib_;
            Ogre::OverlayManager* om;
            Ogre::Overlay* consoleOverlay;
            Ogre::OverlayContainer* consoleOverlayContainer;
            Ogre::PanelOverlayElement* consoleOverlayNoise;
            Ogre::BorderPanelOverlayElement* consoleOverlayBorder;
            Ogre::TextAreaOverlayElement** consoleOverlayTextAreas;
    };
}

#endif /* _InGameConsole_H__ */
