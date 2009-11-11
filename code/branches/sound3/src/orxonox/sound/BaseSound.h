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
#include <OgreSharedPtr.h>
#include <OgreDataStream.h>
#include "core/OrxonoxClass.h"

namespace orxonox
{
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

        virtual void play();
        virtual void stop();
        void pause();

        bool isPlaying();
        bool isPaused();
        bool isStopped();

        virtual void setSource(const std::string& source);
        const std::string& getSource() { return this->source_; }

        bool getPlayOnLoad() { return this->bPlayOnLoad_; }
        void setPlayOnLoad(bool val);

        bool getLoop() { return this->bLoop_; }
        void setLoop(bool val) { this->bLoop_ = val; }

        ALuint getALAudioSource(void);

    protected:
        ALuint loadOggFile();
        ALint getSourceState();

        ALuint audioSource_;
        ALuint audioBuffer_;

    private:
        std::string source_;
        bool bPlayOnLoad_;
        bool bLoop_;
        DataStreamPtr dataStream_;
    };
}

#endif /* _BaseSound_H__ */
