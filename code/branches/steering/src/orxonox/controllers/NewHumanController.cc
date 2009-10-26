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
 *      Michael Wirth
 *   Co-authors:
 *      ...
 *
 */

#include "NewHumanController.h"

#include "core/input/InputManager.h"
#include "core/input/InputState.h"

#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "gametypes/Gametype.h"
#include "infos/PlayerInfo.h"
#include "overlays/Map.h"
#include "graphics/Camera.h"
#include "sound/SoundManager.h"
#include "Radar.h"
#include "Scene.h"

namespace orxonox
{

    CreateUnloadableFactory(NewHumanController);

    NewHumanController::NewHumanController(BaseObject* creator) : HumanController(creator)
    {
        RegisterObject(NewHumanController);
    }

    NewHumanController::~NewHumanController()
    {
        if( this->isInitialized() )
        {
        }
    }

    /*void NewHumanController::tick(float dt)
    {
        if (GameMode::playsSound() && NewHumanController::localController_s && NewHumanController::localController_s->controllableEntity_)
        {
            // Update sound listener
            Camera* camera = NewHumanController::localController_s->controllableEntity_->getCamera();
            if (camera)
            {
                SoundManager::getInstance().setListenerPosition(camera->getWorldPosition());
                SoundManager::getInstance().setListenerOrientation(camera->getWorldOrientation());
            }
            else
                COUT(3) << "NewHumanController, Warning: Using a ControllableEntity without Camera" << std::endl;
        }
    }*/
    
    void NewHumanController::yaw(const Vector2& value)
    {
//         SUPER(NewHumanController, yaw, value);
        HumanController::yaw(value);
        
        this->currentYaw_ = value.x;
        std::cout << "X: " << static_cast<float>(this->currentPitch_) << " Y: " << static_cast<float>(this->currentYaw_) << endl;
    }
    void NewHumanController::pitch(const Vector2& value)
    {
//         SUPER(NewHumanController, pitch, value);
        HumanController::pitch(value);
        
        this->currentPitch_ = value.x;
        std::cout << "X: " << static_cast<float>(this->currentPitch_) << " Y: " << static_cast<float>(this->currentYaw_) << endl;
    }

}
