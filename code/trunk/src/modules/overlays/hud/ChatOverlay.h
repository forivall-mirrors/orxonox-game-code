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

#ifndef _ChatOverlay_H__
#define _ChatOverlay_H__

#include "overlays/OverlaysPrereqs.h"

#include <list>
#include <OgreOverlayElement.h>

#include "chat/ChatListener.h"
#include "overlays/OverlayText.h"

namespace orxonox
{
    class _OverlaysExport ChatOverlay : public OverlayText, public ChatListener
    {
        public:
            ChatOverlay(Context* context);
            virtual ~ChatOverlay();

            void setConfigValues();

        protected:
            virtual void incomingChat(const std::string& message, const std::string& name);

            std::list<Ogre::DisplayString> messages_;

        private:
            void updateOverlayText();
            void dropMessage(Timer* timer);

            float displayTime_;
            std::set<Timer*> timers_;
    };
}
#endif /* _DebugFPSText_H__ */
