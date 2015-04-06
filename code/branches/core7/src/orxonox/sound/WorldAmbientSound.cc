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

#include "WorldAmbientSound.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/XMLPort.h"
#include "AmbientSound.h"
#include "core/command/ConsoleCommandIncludes.h"
#include <exception>


namespace orxonox
{
    SetConsoleCommand("WorldAmbientSound", "nextsong",      &WorldAmbientSound::nextSong);

    RegisterClass(WorldAmbientSound);

    WorldAmbientSound::WorldAmbientSound(Context* context) : BaseObject(context), Synchronisable(context)
    {
        RegisterObject(WorldAmbientSound);

        this->ambientSound_ = new AmbientSound();
        this->registerVariables();
        soundList_.push_back("Earth.ogg");
        soundList_.push_back("Jupiter.ogg");
        soundList_.push_back("Mars.ogg");
        soundList_.push_back("allgorythm-lift_up.ogg");
        soundList_.push_back("allgorythm-resonance_blaster.ogg");
        soundList_.push_back("AlphaCentauri.ogg");
        soundList_.push_back("Asteroid_rocks.ogg");
        soundList_.push_back("Ganymede.ogg");
        soundList_.push_back("luke_grey_-_hypermode.ogg");

    }
    WorldAmbientSound::~WorldAmbientSound()
    {
        if (this->isInitialized())
        {
            this->ambientSound_->destroy();
            WorldAmbientSound::soundList_.clear();
        }
    }

    void WorldAmbientSound::registerVariables()
    {
        registerVariable(this->ambientSound_->ambientSource_, ObjectDirection::ToClient, new NetworkCallback<AmbientSound>(this->ambientSound_, &AmbientSound::ambientSourceChanged));
        registerVariable(this->ambientSound_->bLooping_,      ObjectDirection::ToClient, new NetworkCallback<AmbientSound>(this->ambientSound_, &AmbientSound::loopingChanged));
        registerVariable(this->ambientSound_->pitch_,         ObjectDirection::ToClient, new NetworkCallback<AmbientSound>(this->ambientSound_, &AmbientSound::pitchChanged));
        registerVariable(this->ambientSound_->bPlayOnLoad_,   ObjectDirection::ToClient, new NetworkCallback<AmbientSound>(this->ambientSound_, &AmbientSound::playOnLoadChanged));
    }

    void WorldAmbientSound::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WorldAmbientSound, XMLPort, xmlelement, mode);

        XMLPortParamExtern(WorldAmbientSound, BaseSound, static_cast<BaseSound*>(this->ambientSound_), "looping", setLooping, getLooping, xmlelement, mode);
        XMLPortParamExtern(WorldAmbientSound, BaseSound, static_cast<BaseSound*>(this->ambientSound_), "pitch",   setPitch,   getPitch,   xmlelement, mode);

        XMLPortParamExtern(WorldAmbientSound, AmbientSound, this->ambientSound_, "source",     setAmbientSource, getAmbientSource, xmlelement, mode);
        XMLPortParamExtern(WorldAmbientSound, AmbientSound, this->ambientSound_, "playOnLoad", setPlayOnLoad,    getPlayOnLoad,    xmlelement, mode);
    }

    void WorldAmbientSound::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WorldAmbientSound, XMLEventPort, xmlelement, mode);

        XMLPortEventState(WorldAmbientSound, BaseObject, "play", play, xmlelement, mode);
    }

    void WorldAmbientSound::play()
    {
        this->ambientSound_->play();
    }

    void WorldAmbientSound::changedActivity()
    {
        SUPER(WorldAmbientSound, changedActivity);

        if (this->isActive())
            this->ambientSound_->play();
        else
            this->ambientSound_->stop();
    }

    void WorldAmbientSound::nextSong()
    {

        //HACK: Assuption - there is only one WorldAmbientSound in a level and only one level is used.
        for (ObjectList<WorldAmbientSound>::iterator it = ObjectList<WorldAmbientSound>::begin();
             it != ObjectList<WorldAmbientSound>::end(); ++it)
        {
            while(it->ambientSound_->setAmbientSource(WorldAmbientSound::soundList_[WorldAmbientSound::soundNumber_]) == false){
                WorldAmbientSound::soundNumber_ = (WorldAmbientSound::soundNumber_ + 1) % WorldAmbientSound::soundList_.size();
            }
            WorldAmbientSound::soundNumber_ = (WorldAmbientSound::soundNumber_ + 1) % WorldAmbientSound::soundList_.size();
        }
    }
}
