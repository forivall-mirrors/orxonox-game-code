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

#ifndef _TeamBaseMatchScore_H__
#define _TeamBaseMatchScore_H__

#include "overlays/OverlaysPrereqs.h"

#include "tools/interfaces/Tickable.h"
#include "overlays/OverlayText.h"

namespace orxonox
{
    class _OverlaysExport TeamBaseMatchScore : public OverlayText, public Tickable
    {
        public:
            TeamBaseMatchScore(BaseObject* creator);
            virtual ~TeamBaseMatchScore();

            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedOwner();

            inline void setShowBases(bool value)
                { this->bShowBases_ = value; }
            inline bool getShowBases() const
                { return this->bShowBases_; }

            inline void setShowScore(bool value)
                { this->bShowScore_ = value; }
            inline bool getShowScore() const
                { return this->bShowScore_; }

            inline void setShowLeftTeam(bool value)
                { this->bShowLeftTeam_ = value; }
            inline bool getShowLeftTeam() const
                { return this->bShowLeftTeam_; }

            inline void setShowRightTeam(bool value)
                { this->bShowRightTeam_ = value; }
            inline bool getShowRightTeam() const
                { return this->bShowRightTeam_; }

        private:
            TeamBaseMatch* owner_;
            bool bShowBases_;
            bool bShowScore_;
            bool bShowLeftTeam_;
            bool bShowRightTeam_;
    };
}
#endif /* _TeamBaseMatchScore_H__ */
