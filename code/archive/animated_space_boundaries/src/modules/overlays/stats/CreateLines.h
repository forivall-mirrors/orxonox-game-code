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

#ifndef _CreateLines_H__
#define _CreateLines_H__

#include "overlays/OverlaysPrereqs.h"

namespace orxonox
{
    class _OverlaysExport CreateLines
    {

    public: // functions
        CreateLines(float, float, float, float);
        virtual ~CreateLines();

        void setPlayerName(const std::string&);
        void setScore(const std::string&);
        void setDeaths(const std::string&);

        void setVisibility(bool);
        void changedVisibility();
        void setOverlayGroup( OverlayGroup* group );

        //void setNumberOfColumns(unsigned int numberOfColumns, unsigned int lineIndex);
        //void alignColumn(int columnIndex, float leftOffset, float topOffset);
        //void setColumnText(int columnIndex, std::string columnText);

    private: // functions

    private: // variables
        //std::vector<OverlayText*> textColumns_;
        OverlayText* playerNameText_;
        OverlayText* scoreText_;
        OverlayText* deathsText_;

        Stats* background_;

    };
}

#endif /* _CreateLines_H__ */
