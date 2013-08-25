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

#ifndef _GametypeInfoMessage_H__
#define _GametypeInfoMessage_H__

#include "overlays/OverlaysPrereqs.h"

#include "interfaces/GametypeMessageListener.h"
#include "overlays/OverlayText.h"
/*Goal: Every Gametype could add a hud-element that displays text in the gametype. For example instructions for newbies.
-> you have to add a "GametypeInfoMessage"-object that displays text. How the message change has to be defined in the gametype.
*/

namespace orxonox
{
    class _OverlaysExport GametypeStaticMessage : public OverlayText, GametypeMessageListener
    {
        public:
            GametypeStaticMessage(Context* context);
            virtual ~GametypeStaticMessage();

            virtual void changedOwner();

            void staticmessage(const GametypeInfo* gtinfo, const std::string& message, const ColourValue& colour);

        private:
            PlayerInfo* owner_;
    };
}
#endif /* _GametypeInfoMessage_H__ */
