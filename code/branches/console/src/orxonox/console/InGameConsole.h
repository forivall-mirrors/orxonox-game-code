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


#include "OrxonoxStableHeaders.h"
#include "OrxonoxPrereqs.h"
#include "core/Debug.h"
#include "core/InputBuffer.h"
#include "core/Tickable.h"
#include "core/ConsoleCommand.h"
#include <string>
#include <GraphicsEngine.h>
#include <OgrePrerequisites.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreBorderPanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayElement.h>
#include <OgreStringConverter.h>


namespace orxonox
{
    class _OrxonoxExport InGameConsole : public InputBufferListener, public Tickable{
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

            int windowW;
            int windowH;
            int scroll;
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
