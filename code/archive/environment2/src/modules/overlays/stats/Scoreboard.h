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


#include "overlays/OverlaysPrereqs.h"

#include <string>
#include <vector>
#include "tools/interfaces/Tickable.h"
#include "overlays/OrxonoxOverlay.h"

namespace orxonox
{
    class _OverlaysExport Scoreboard : public OrxonoxOverlay, public Tickable
    {
    public: // functions
        Scoreboard(BaseObject* creator);
        virtual ~Scoreboard();

        virtual void tick(float dt);

        inline void setCreateLines(CreateLines* cl)
            { this->createlines_ = cl; }
        inline CreateLines* getCreateLines() const
            { return this->createlines_; }

        virtual void changedVisibility();

    private: // functions
        void printLines();

    private: // variables
        std::vector<CreateLines*> lines_;

        unsigned int numberOfColumns;
        unsigned int numberOfLines;
        unsigned int columnIndex;
        float topOffset;
        float leftOffset;
        float lineSpacing;
        std::string columnText;
        CreateLines* createlines_;
    };
}

#endif /* _Scoreboard_H__ */
