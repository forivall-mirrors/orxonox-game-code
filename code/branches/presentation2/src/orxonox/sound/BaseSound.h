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
 *      Erwin 'vaiursch' Herrsche
 *   Co-authors:
 *      Reto Grieder
 *
 */

#ifndef _BaseSound_H__
#define _BaseSound_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <boost/shared_ptr.hpp>
#include <OgreDataStream.h>
#include "core/OrxonoxClass.h"

namespace orxonox
{
    // forward declaration
    class SoundBuffer;

    /**
     * The BaseSound class is the base class for all sound file loader classes.
     * It server as main interface to the OpenAL library.
     *
     */
    class _OrxonoxExport BaseSound : virtual public OrxonoxClass
    {
    public:
        BaseSound();
        virtual ~BaseSound();

        void XMLPortExtern(Element& xmlelement, XMLPort::Mode mode);

        virtual void play();
        virtual void stop();
        virtual void pause();

        bool isPlaying() { return this->state_ == Playing; }
        bool isPaused()  { return this->state_ == Paused; }
        bool isStopped() { return this->state_ == Stopped; }

        void setPlaying(bool val)
            { val ? this->play() : this->stop(); }

        virtual void setSource(const std::string& source);
        virtual const std::string& getSource() const { return this->source_; }

        void setVolume(float vol);
        float getVolume() const { return this->volume_; }
        
        virtual float getVolumeGain();
        void updateVolume(void);

        bool getLooping() const   { return this->bLoop_; }
        void setLooping(bool val);

        float getPitch() const   { return this->pitch_; }
        void setPitch(float pitch);

        //ALuint getALAudioSource(void);

    protected:
        ALint getSourceState() const;

        ALuint          audioSource_;
        bool            bPooling_;
        shared_ptr<SoundBuffer> soundBuffer_;

    private:
        enum State
        {
            Stopped,
            Playing,
            Paused
        };

        std::string     source_;
        float           volume_;
        bool            bLoop_;
        State           state_;
        float           pitch_;
        DataStreamPtr   dataStream_;
    };
}

#endif /* _BaseSound_H__ */
