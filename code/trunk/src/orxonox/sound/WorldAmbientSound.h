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

#ifndef _WorldAmbientSound_H__
#define _WorldAmbientSound_H__

#include "OrxonoxPrereqs.h"

#include "core/BaseObject.h"
#include "network/synchronisable/Synchronisable.h"
#include <string>
#include <vector>


namespace orxonox
{
    /**
        @brief This class is used to play and control ambient sound in a level file. See AmbientSound for the
        actual implementation of the background music player.
    */
    class _OrxonoxExport WorldAmbientSound : public BaseObject, public Synchronisable
    {
        public:
            WorldAmbientSound(Context* context);
            virtual ~WorldAmbientSound();

            void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);

            virtual void changedActivity();

            void play();

            //This function changes the current ambient song.
            //You can call nextSong() active in the level
            //by pressing the key 'M'.
            static void nextSong();

        private:
            void registerVariables();

            //Vector with the diffrent available level sounds.
            //The sound names are pushed in the WorldAmbientSound-constructor.
            static std::vector<std::string> soundList_;

            // This value will be initialized below, don't make this into
            // a const, since we want to change it in nextSong().
            static int soundNumber_;


            AmbientSound* ambientSound_;
    };

    // This is an initialization for the soundnumber variable. Since it is
    // static, we have to initialize it this way.
    int WorldAmbientSound::soundNumber_ = 0;
    std::vector<std::string> WorldAmbientSound::soundList_;

}

#endif /* _WorldAmbientSound_H__ */
