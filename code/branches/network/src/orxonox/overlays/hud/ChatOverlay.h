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

#include "OrxonoxPrereqs.h"

#include <OgreTextAreaOverlayElement.h>

#include "network/ChatListener.h"
#include "overlays/OverlayText.h"

namespace orxonox
{
    class _OrxonoxExport ChatOverlay : public OverlayText, public network::ChatListener
    {
        public:
            ChatOverlay(BaseObject* creator);
            ~ChatOverlay();

            void setConfigValues();

        protected:
            virtual void incomingChat(const std::string& message, unsigned int senderID);

            std::list<Ogre::UTFString> messages_;

        private:
            void updateOverlayText();
            void dropMessage();

            float displayTime_;
    };
}
#endif /* _DebugFPSText_H__ */
