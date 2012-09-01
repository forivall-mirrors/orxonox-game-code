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
 *
 */
#include "SoundStreamer.h"

#include <boost/thread.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#include "SoundManager.h"
#include "util/Sleep.h"

namespace orxonox
{
    // vorbis callbacks
    size_t readVorbis(void* ptr, size_t size, size_t nmemb, void* datasource);
    int seekVorbis(void* datasource, ogg_int64_t offset, int whence);
    long tellVorbis(void* datasource);

    void orxonox::SoundStreamer::operator()(ALuint audioSource, DataStreamPtr dataStream, OggVorbis_File* vf, int current_section)
    {
        char inbuffer[4096];
        vorbis_info* vorbisInfo;
        vorbisInfo = ov_info(vf, -1);
        ALenum format;
        if (vorbisInfo->channels == 1)
            format = AL_FORMAT_MONO16;
        else
            format = AL_FORMAT_STEREO16;

        while(true) // Stream forever, control through thread control
        {

            int info;
            alGetSourcei(audioSource, AL_SOURCE_STATE, &info);
            if(info == AL_PLAYING)
                COUT(4) << "Sound: " << dataStream->getName() << " is playing." << std::endl;
            else
            {
                COUT(4) << "Sound: " << dataStream->getName() << " is not playing." << std::endl;
            }

            if(alcGetCurrentContext() == NULL)
            {
                COUT(2) << "Sound: There is no context, terminating thread for " << dataStream->getName() << std::endl;
                return;
            }

            int processed;
            alGetSourcei(audioSource, AL_BUFFERS_PROCESSED, &processed);
            if (ALint error = alGetError())
                COUT(2) << "Sound: Warning: Couldn't get number of processed buffers: " << getALErrorString(error) << std::endl;

            COUT(4) << "Sound: processed buffers: " << processed << std::endl;

            if(processed > 0)
            {
                ALuint* buffers = new ALuint[processed];
                alSourceUnqueueBuffers(audioSource, processed, buffers);
                if (ALint error = alGetError())
                    COUT(2) << "Sound: Warning: Couldn't unqueue buffers: " << getALErrorString(error) << std::endl;
            
                int queued;
                alGetSourcei(audioSource, AL_BUFFERS_QUEUED, &queued);
                if (ALint error = alGetError())
                    COUT(2) << "Sound: Warning: Couldn't get number of queued buffers: " << getALErrorString(error) << std::endl;
                COUT(4) << "Sound: queued buffers: " << queued << std::endl;

                for(int i = 0; i < processed; i++)
                {
                    long ret = ov_read(vf, inbuffer, sizeof(inbuffer), 0, 2, 1, &current_section);
                    if (ret == 0)
                    {
                        COUT(4) << "Sound: End of file " << dataStream->getName() << ", terminating thread" << std::endl;
                        return;
                    }
                    else if (ret < 0)
                    {
                        COUT(2) << "Sound: libvorbisfile: error reading the file " << dataStream->getName() << std::endl;
                        ov_clear(vf);
                        return;
                    }

                    alBufferData(buffers[i], format, &inbuffer, ret, vorbisInfo->rate);
                    if(ALint error = alGetError()) {
                        COUT(2) << "Sound: Could not fill buffer: " << getALErrorString(error) << std::endl;
                        break;
                    }
                    alSourceQueueBuffers(audioSource, 1, &buffers[i]);
                    if (ALint error = alGetError()) {
                        COUT(2) << "Sound: Warning: Couldn't queue buffers: " << getALErrorString(error) << std::endl;
                    }
                }
            }
            else
            {
                msleep(10); // perhaps another value here is better
            }

            try {
                boost::this_thread::interruption_point();
            }
            catch(boost::thread_interrupted) {
                COUT(4) << "Sound: Catched interruption. Terminating thread for " << dataStream->getName() << std::endl;
                ALuint* buffers = new ALuint[5];
                alSourceUnqueueBuffers(audioSource, 5, buffers);
                if (ALint error = alGetError())
                    COUT(2) << "Sound: Warning: Couldn't unqueue buffers: " << getALErrorString(error) << std::endl;

                alDeleteBuffers(5, buffers);
                if (ALint error = alGetError())
                    COUT(2) << "Sound: Warning: Couldn't delete buffers: " << getALErrorString(error) << std::endl;

                return;
            }
        }
    }
}
