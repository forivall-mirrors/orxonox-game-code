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
 *       Erwin 'vaiursch' Herrsche
 *       Kevin Young
 *   Co-authors:
 *      ...
 */

#ifndef _SoundManager_H__
#define _SoundManager_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include <string>
#include "util/Singleton.h"

namespace orxonox
{
    /**
     * The SoundManager class manages the OpenAL device, context and listener
     * position. It is a singleton.
     *
     */
    class _OrxonoxExport SoundManager : public Singleton<SoundManager>, public OrxonoxClass
    {
        friend class Singleton<SoundManager>;

    public:
        SoundManager();
        ~SoundManager();

        void update(const Clock& time);
        void setConfigValues();

        void setListenerPosition(const Vector3& position);
        void setListenerOrientation(const Quaternion& orientation);

        void registerAmbientSound(AmbientSound* newAmbient);
        void unregisterAmbientSound(AmbientSound* oldAmbient);
        void pauseAmbientSound(AmbientSound* ambient);
        std::string getAmbientPath(const std::string& source);

    private:
        void processCrossFading(float dt);
        void fadeIn(AmbientSound* sound);
        void fadeOut(AmbientSound* sound);

        void checkFadeStepValidity();

        ALCdevice* device_;
        ALCcontext* context_;
       
        typedef std::list<std::pair<AmbientSound*, bool> > AmbientList;
        AmbientList ambientSounds_;
        
        float crossFadeStep_;       //!< Absolute change per second (0.1 means 10% of the nominal volume) for cross fading
        std::list<AmbientSound*> fadeInList_;
        std::list<AmbientSound*> fadeOutList_;
        
        static SoundManager* singletonPtr_s;
    };
}

#endif /* _SoundManager_H__ */
