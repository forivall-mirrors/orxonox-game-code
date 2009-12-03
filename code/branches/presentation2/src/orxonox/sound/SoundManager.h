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
#include <map>
#include <string>
#include <boost/weak_ptr.hpp>

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"

// tolua_begin
namespace orxonox
{
    // forward declaration
    class SoundBuffer;
    
    //! Enum for the sound type.
    namespace SoundType
    {
        enum Value
        {
            none,
            ambient,
            effects
        };
    }
    
    /**
     * The SoundManager class manages the OpenAL device, context and listener
     * position. It is a singleton.
     *
     */
    class _OrxonoxExport SoundManager
    // tolua_end
        : public Singleton<SoundManager>, public OrxonoxClass
    { // tolua_export
        friend class Singleton<SoundManager>;

    public:
        SoundManager();
        ~SoundManager();

        void preUpdate(const Clock& time);
        void setConfigValues();
        
        static SoundManager& getInstance() { return Singleton<SoundManager>::getInstance(); } // tolua_export

        void setListenerPosition(const Vector3& position);
        void setListenerOrientation(const Quaternion& orientation);

        void registerAmbientSound(AmbientSound* newAmbient);
        void unregisterAmbientSound(AmbientSound* oldAmbient);
        void pauseAmbientSound(AmbientSound* ambient);
        
        void setVolume(float vol, SoundType::Value type);
        float getVolume(SoundType::Value type); // tolua_export
        
        void toggleMute(SoundType::Value type); // tolua_export
        bool getMute(SoundType::Value type); // tolua_export

        shared_ptr<SoundBuffer> getSoundBuffer(shared_ptr<ResourceInfo> fileInfo);
        void removeBuffer(shared_ptr<ResourceInfo> fileInfo);

    private:
        void processCrossFading(float dt);
        void fadeIn(AmbientSound* sound);
        void fadeOut(AmbientSound* sound);

        void checkFadeStepValidity();
        bool checkVolumeValidity(SoundType::Value type);
        void checkSoundVolumeValidity(void);
        void checkAmbientVolumeValidity(void);
        void checkEffectsVolumeValidity(void);
        
        float checkVolumeRange(float vol);
        
        void updateVolume(SoundType::Value type);
        
        void setVolumeInternal(float vol, SoundType::Value type);
        float getVolumeInternal(SoundType::Value type);

/*
        ALCdevice* device_;
        ALCcontext* context_;
*/
       
        typedef std::list<std::pair<AmbientSound*, bool> > AmbientList;
        AmbientList ambientSounds_;
        
        float crossFadeStep_;       //!< Absolute change per second (0.1 means 10% of the nominal volume) for cross fading
        std::list<AmbientSound*> fadeInList_;
        std::list<AmbientSound*> fadeOutList_;
        
        float soundVolume_;
        float ambientVolume_;
        float effectsVolume_;
        std::map<SoundType::Value, bool> mute_;

        std::map<std::string, weak_ptr<SoundBuffer> > soundBuffers_;
        
        static SoundManager* singletonPtr_s;
    }; // tolua_export
} // tolua_export

#endif /* _SoundManager_H__ */
