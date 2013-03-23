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
 *       Reto Grieder
 *   Co-authors:
 *      ...
 */

#ifndef _SoundManager_H__
#define _SoundManager_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"
#include "core/object/SmartPtr.h"

// tolua_begin
namespace orxonox
{
    //! Enum for the sound type.
    namespace SoundType
    {
        enum Value
        {
            All     = 0,
            Music   = 1,
            Effects = 2
        };
    }

    //! The SoundManager class manages the OpenAL device, context and listener position.
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

        // tolua_begin
        static SoundManager& getInstance() { return Singleton<SoundManager>::getInstance(); }
        static bool exists() { return Singleton<SoundManager>::exists(); }

        std::string getDeviceName(unsigned int index) const
            { return index < this->deviceNames_.size() ? this->deviceNames_[index] : std::string(); }
        // tolua_end

        void setListenerPosition(const Vector3& position);
        void setListenerOrientation(const Quaternion& orientation);

        void registerAmbientSound(AmbientSound* newAmbient);
        void unregisterAmbientSound(AmbientSound* oldAmbient);
        void pauseAmbientSound(AmbientSound* ambient);

        // tolua_begin
        void setVolume(float vol, SoundType::Value type);
        float getVolume(SoundType::Value type);
        float getRealVolume(SoundType::Value type);

        void toggleMute(SoundType::Value type);
        bool getMute(SoundType::Value type);
        // tolua_end

        shared_ptr<SoundBuffer> getSoundBuffer(const std::string& filename);
        void releaseSoundBuffer(const shared_ptr<SoundBuffer>& buffer, bool bPoolBuffer);

        ALuint getSoundSource(BaseSound* object);
        void releaseSoundSource(ALuint source);

        static std::string getALErrorString(ALenum error);

    private:
        void processCrossFading(float dt);
        void fadeIn(const SmartPtr<AmbientSound>& sound);
        void fadeOut(const SmartPtr<AmbientSound>& sound);

        void checkFadeStepValidity();

        void checkVolumeValidity(SoundType::Value type);
        void checkSoundVolumeValidity()   { this->checkVolumeValidity(SoundType::All); }
        void checkAmbientVolumeValidity() { this->checkVolumeValidity(SoundType::Music); }
        void checkEffectsVolumeValidity() { this->checkVolumeValidity(SoundType::Effects); }
        void updateVolume(SoundType::Value type);

        unsigned int createSoundSources(unsigned int n);

        // OpenAL device/context related
        std::vector<std::string> deviceNames_;
        ALCdevice* device_;
        ALCcontext* context_;

        // Ambient sound related
        typedef std::list<std::pair<AmbientSound*, bool> > AmbientList;
        AmbientList                        ambientSounds_;
        //! Absolute change per second (0.1 means 10% of the nominal volume) for cross fading
        float                              crossFadeStep_;
        std::list<SmartPtr<AmbientSound> > fadeInList_;
        std::list<SmartPtr<AmbientSound> > fadeOutList_;

        // Volume related
        float volume_[3];
        float mute_[3];

        // Sound buffer related
        static const unsigned int maxEffectsPoolSize_s = 40 * 1024 * 1024;
        unsigned int effectsPoolSize_;
        typedef std::list<shared_ptr<SoundBuffer> > EffectsPoolList;
        EffectsPoolList effectsPool_;
        typedef std::map<std::string, shared_ptr<SoundBuffer> > SoundBufferMap;
        SoundBufferMap soundBuffers_;

        // Sound source related
        unsigned int minSources_;
        unsigned int maxSources_;
        std::vector<ALuint> availableSoundSources_;
        std::vector<std::pair<ALuint, BaseSound*> > usedSoundSources_;

        bool bDestructorCalled_; ///< Becomes true if the destructor is called - used to prevent ambient sounds from registering after the lists were cleared

        static SoundManager* singletonPtr_s;
    }; // tolua_export
} // tolua_export

#endif /* _SoundManager_H__ */
