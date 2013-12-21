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
 *      Florian Zinggeler
 *
 */

#ifndef _InvaderHUDinfo_H__
#define _InvaderHUDinfo_H__

#include "overlays/OverlaysPrereqs.h"

#include "tools/interfaces/Tickable.h"
#include "overlays/OverlayText.h"

namespace orxonox
{
    class _OverlaysExport InvaderHUDinfo : public OverlayText, public Tickable
    {
        public:
            InvaderHUDinfo(Context* context);

            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedOwner();

            inline void setShowLives(bool value)
                { this->bShowLives_ = value; }
            inline bool getShowLives() const
                { return this->bShowLives_; }

            inline void setShowLevel(bool value)
                { this->bShowLevel_ = value; }
            inline bool getShowLevel() const
                { return this->bShowLevel_; }

            inline void setShowPoints(bool value)
                { this->bShowPoints_ = value; }
            inline bool getShowPoints() const
                { return this->bShowPoints_; }

            inline void setShowMultiplier(bool value)
                { this->bShowMultiplier_ = value; }
            inline bool getShowMultiplier() const
                { return this->bShowMultiplier_; }


        private:
            Invader* InvaderGame;
            bool bShowLives_;
            bool bShowLevel_;
            bool bShowPoints_;
            bool bShowMultiplier_;
    };
}
#endif /* _InvaderHUDinfo_H__ */
