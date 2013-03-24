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
#include "core/class/OrxonoxClass.h"

namespace orxonox
{
    /**
     * The BaseSound class is the base class for all sound file loader classes.
     * It serves as main interface to the OpenAL library.
     */
    class _OrxonoxExport BaseSound : virtual public OrxonoxClass
    {
    public:
        BaseSound();

        void XMLPortExtern(Element& xmlelement, XMLPort::Mode mode);

        virtual void play()  { this->doPlay(); }
        virtual bool stop()  { return this->doStop(); } // returns true if the sound source was destroyed
        virtual void pause() { this->doPause(); }

        bool isPlaying() const { return this->state_ == Playing; }
        bool isPaused()  const { return this->state_ == Paused; }
        bool isStopped() const { return this->state_ == Stopped; }

        virtual void setSource(const std::string& source);
        virtual const std::string& getSource() const
            { return this->source_; }

        void setVolume(float vol);
        float getVolume() const
            { return this->volume_; }
        void updateVolume();

        bool getLooping() const
            { return this->bLooping_; }
        void setLooping(bool val);

        float getPitch() const
            { return this->pitch_; }
        void setPitch(float pitch);

    protected:
        enum State
        {
            Stopped,
            Playing,
            Paused
        };

        virtual ~BaseSound();

        void doPlay();
        bool doStop(); // returns true if the sound source was destroyed
        void doPause();

        // network callbacks
        inline void pitchChanged()
            { this->setPitch(this->pitch_); }
        inline void loopingChanged()
            { this->setLooping(this->bLooping_); }
        inline void volumeChanged()
            { this->setVolume(this->volume_); }
        inline void sourceChanged()
            { this->setSource(this->source_); }
        void stateChanged();

        virtual void initialiseSource();
        ALint getSourceState() const;

        virtual float getRealVolume() = 0;

        ALuint          audioSource_;
        bool            bPooling_;
        shared_ptr<SoundBuffer> soundBuffer_;
        std::string     source_;
        float           volume_;
        bool            bLooping_;
        uint8_t         state_;       // This Variable is actually of type State
        float           pitch_;

    private:
        DataStreamPtr   dataStream_;
    };
}

#endif /* _BaseSound_H__ */
