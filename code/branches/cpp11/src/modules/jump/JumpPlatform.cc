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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpPlatform.cc
    @brief All platforms in this minigame inherit from this class. The basic functions of a platform (interact with figure) is implemented here. Special functions are implemented in the specialized classes.
*/

#include "JumpPlatform.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"
#include "sound/WorldSound.h"
#include "JumpFigure.h"

namespace orxonox
{
    RegisterClass(JumpPlatform);

    JumpPlatform::JumpPlatform(Context* context) : MovableEntity(context)
    {
        RegisterObject(JumpPlatform);

        figure_ = 0;

        //initialize sound
        if (GameMode::isMaster())
        {
            defScoreSound_ = new WorldSound(this->getContext());
            defScoreSound_->setVolume(1.0f);
            defBatSound_ = new WorldSound(this->getContext());
            defBatSound_->setVolume(0.4f);
            defBoundarySound_ = new WorldSound(this->getContext());
            defBoundarySound_->setVolume(0.5f);
        }
        else
        {
            defScoreSound_ = 0;
            defBatSound_ = 0;
            defBoundarySound_ = 0;
        }

        setPosition(Vector3(0,0,0));
        setVelocity(Vector3(0,0,0));
        setAcceleration(Vector3(0,0,0));
    }

    /**
    @brief
        Destructor.
    */
    JumpPlatform::~JumpPlatform()
    {

    }

    //xml port for loading sounds
    void JumpPlatform::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPlatform, XMLPort, xmlelement, mode);

        XMLPortParam(JumpPlatform, "height", setHeight, getHeight, xmlelement, mode);
        XMLPortParam(JumpPlatform, "width", setWidth, getWidth, xmlelement, mode);

        XMLPortParam(JumpPlatform, "defScoreSound",  setDefScoreSound,  getDefScoreSound,  xmlelement, mode);
        XMLPortParam(JumpPlatform, "defBatSound",  setDefBatSound,  getDefBatSound,  xmlelement, mode);
        XMLPortParam(JumpPlatform, "defBoundarySound",  setDefBoundarySound,  getDefBoundarySound,  xmlelement, mode);
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
    void JumpPlatform::tick(float dt)
    {
        SUPER(JumpPlatform, tick, dt);

        Vector3 platformPosition = this->getPosition();

        if (figure_ != NULL)
        {
            Vector3 figurePosition = figure_->getPosition();
            Vector3 figureVelocity = figure_->getVelocity();

            float tolerance = 3.0;

            if(figureVelocity.z < 0 && figurePosition.x > platformPosition.x-width_/2 && figurePosition.x < platformPosition.x+width_/2 && figurePosition.z > platformPosition.z-height_/2*tolerance && figurePosition.z < platformPosition.z+height_/2)
            {
                touchFigure();
            }
        }
    }

    void JumpPlatform::setFigure(WeakPtr<JumpFigure> newFigure)
    {
        figure_ = newFigure;
    }

    void JumpPlatform::touchFigure()
    {

    }

    void JumpPlatform::setDefScoreSound(const std::string &jumpSound)
    {
        if( defScoreSound_ )
            defScoreSound_->setSource(jumpSound);
        else
            assert(0); // This should never happen, because soundpointer is only available on master
    }

    const std::string& JumpPlatform::getDefScoreSound()
    {
        if( defScoreSound_ )
            return defScoreSound_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }

    void JumpPlatform::setDefBatSound(const std::string &jumpSound)
    {
        if( defBatSound_ )
            defBatSound_->setSource(jumpSound);
        else
            assert(0); // This should never happen, because soundpointer is only available on master
    }

    const std::string& JumpPlatform::getDefBatSound()
    {
        if( defBatSound_ )
            return defBatSound_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }

    void JumpPlatform::setDefBoundarySound(const std::string &jumpSound)
    {
        if( defBoundarySound_ )
            defBoundarySound_->setSource(jumpSound);
        else
            assert(0); // This should never happen, because soundpointer is only available on master
    }

    const std::string& JumpPlatform::getDefBoundarySound()
    {
        if( defBoundarySound_ )
            return defBoundarySound_->getSource();
        else
            assert(0);
        return BLANKSTRING;
    }
}
