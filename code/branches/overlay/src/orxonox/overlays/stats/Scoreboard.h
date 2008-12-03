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
 *      Benjamin Hildebrandt
 *
 */

#ifndef _Scoreboard_H__
#define _Scoreboard_H__


#include "OrxonoxPrereqs.h"
#include <OgrePrerequisites.h>
#include <OgreBorderPanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>

#include "overlays/OrxonoxOverlay.h"
#include "objects/Tickable.h"


namespace orxonox
{
    class _OrxonoxExport Scoreboard : public OrxonoxOverlay
    {

    public: // functions
        Scoreboard(BaseObject* creator);
        ~Scoreboard() {}

        inline void setGametype(Gametype* gt)
            { this->gametype_ = gt; }
        inline Gametype* getGametype() const
            { return this->gametype_; }

        inline void setCreateLines(CreateLines* cl)
            { this->createlines_ = cl; }
        inline CreateLines* getCreateLines() const
            { return this->createlines_; }

    private: // functions
        void printLines();

    private: // variables
        unsigned int numberOfColumns;
        unsigned int numberOfLines;
        unsigned int columnIndex;
        float topOffset;
        float leftOffset;
        float lineSpacing;
        std::string columnText;
        Gametype* gametype_;
        CreateLines* createlines_;
    };
}

#endif /* _Scoreboard_H__ */
