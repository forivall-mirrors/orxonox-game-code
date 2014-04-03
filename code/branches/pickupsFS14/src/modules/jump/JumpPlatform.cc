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
 *      Benjamin de Capitani
 *   Co-authors:
 *      ...
 *
 */

#include "JumpPlatform.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/command/Executor.h"
#include "tools/ParticleInterface.h"
#include "Scene.h"
#include "graphics/ParticleSpawner.h"
#include "graphics/Model.h"
#include "worldentities/MovableEntity.h"
#include "chat/ChatManager.h"
#include "OrxonoxPrereqs.h"

namespace orxonox
{
    RegisterClass(JumpPlatform);

    JumpPlatform::JumpPlatform(Context* context) : MobileEntity(context)
    {
        RegisterObject(JumpPlatform);


        model = new Model(getContext());
        model->setSyncMode(0);
        model->setMeshSource("drone.mesh");

        movableEntity = new MovableEntity(getContext());
        movableEntity->attach(model);

        movableEntity->setScale(100);
        //movableEntity->setPosition(Vector3(0,0,0));
        //movableEntity->setVelocity(Vector3(0,0,1));


    }




}
