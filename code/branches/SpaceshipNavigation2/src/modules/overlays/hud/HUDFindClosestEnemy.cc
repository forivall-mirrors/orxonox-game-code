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
 *      Andrea Soro
 *
 */

#include "HUDFindClosestEnemy.h"

/*#include <OgreCamera.h>
#include <OgreFontManager.h>
#include <OgreOverlayManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>*/

#include "util/Math.h"
#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "CameraManager.h"
#include "Scene.h"
#include "Radar.h"
#include "graphics/Camera.h"
#include "controllers/HumanController.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/WorldEntity.h"
#include "core/ConfigValueIncludes.h"
#include "tools/TextureGenerator.h"
// #include <boost/bind/bind_template.hpp>


namespace orxonox
{
    CreateFactory ( HUDFindClosestEnemy );

    HUDFindClosestEnemy::HUDFindClosestEnemy(BaseObject* creator) :HUDNavigation(creator)
    {
        orxout()<<"ClosestEnemyFinder activated"<<endl;
    }

    HUDFindClosestEnemy::~HUDFindClosestEnemy()
    {

    }


    void HUDFindClosestEnemy::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(HUDFindClosestEnemy, XMLPort, xmlelement, mode);

    }




    void HUDFindClosestEnemy::tick(float dt)
    {
        SUPER(HUDFindClosestEnemy, tick, dt);


    }




}
