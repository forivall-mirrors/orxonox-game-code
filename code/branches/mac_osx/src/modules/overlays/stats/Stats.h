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
 *      Fabian 'x3n' Landau, Benjamin Hildebrandt
 *
 */

#ifndef _Stats_H__
#define _Stats_H__

#include "overlays/OverlaysPrereqs.h"

#include "util/OgreForwardRefs.h"
#include "tools/interfaces/Tickable.h"
#include "overlays/OrxonoxOverlay.h"

namespace orxonox
{
    class _OverlaysExport Stats : public OrxonoxOverlay, public Tickable
    {
    public: // functions
        Stats(BaseObject* creator);
        virtual ~Stats();

        void setConfigValues();

        virtual void tick(float dt);

    private: // variables
        Ogre::PanelOverlayElement* statsOverlayNoise_;
        Ogre::BorderPanelOverlayElement* statsOverlayBorder_;
        float noiseSize_;
    };
}

#endif /* _Stats_H__ */
