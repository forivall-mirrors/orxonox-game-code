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

#include <al.h>
#include <alc.h>
#include <vorbis/vorbisfile.h>
#include "SoundManager.h"
#include "util/Sleep.h"

namespace orxonox
{
    // vorbis callbacks
    size_t readVorbis(void* ptr, size_t size, size_t nmemb, void* datasource);
    int seekVorbis(void* datasource, ogg_int64_t offset, int whence);
    long tellVorbis(void* datasource);

    void orxonox::SoundStreamer::operator()(ALuint audioSource, DataStreamPtr dataStream)
    {
        // Open file with custom streaming
        ov_callbacks vorbisCallbacks;
        vorbisCallbacks.read_func  = &readVorbis;
        vorbisCallbacks.seek_func  = &seekVorbis;
        vorbisCallbacks.tell_func  = &tellVorbis;
        vorbisCallbacks.close_func = NULL;

        OggVorbis_File vf;
        int ret = ov_open_callbacks(dataStream.get(), &vf, NULL, 0, vorbisCallbacks);
        if (ret < 0)
        {
            COUT(2) << "Sound: libvorbisfile: File does not seem to be an Ogg Vorbis bitstream" << std::endl;
            ov_clear(&vf);
            return;
        }
        vorbis_info* vorbisInfo;
        vorbisInfo = ov_info(&vf, -1);
        ALenum format;
        if (vorbisInfo->channels == 1)
            format = AL_FORMAT_MONO16;
        else
            format = AL_FORMAT_STEREO16;

        char inbuffer[256*1024];
        ALuint initbuffers[4];
        alGenBuffers(4, initbuffers);
        if (ALint error = alGetError()) {
            COUT(2) << "Sound: Streamer: Could not generate buffer:" << getALErrorString(error) << std::endl;
            return;
        }
        int current_section;

        for(int i = 0; i < 4; i++)
        {
            long ret = ov_read(&vf, inbuffer, sizeof(inbuffer), 0, 2, 1, &current_section);
            if (ret == 0)
            {
                return;
            }
            else if (ret < 0)
            {
                COUT(2) << "Sound: libvorbisfile: error reading the file" << std::endl;
                ov_clear(&vf);
                return;
            }

            alBufferData(initbuffers[i], format, &inbuffer, ret, vorbisInfo->rate);
            alGetError();
        }
        alSourceQueueBuffers(audioSource, 4, initbuffers);
        if (ALint error = alGetError()) {
            COUT(2) << "Sound: Warning: Couldn't queue buffers: " << getALErrorString(error) << std::endl;
        }

        while(true) // Stream forever, control through thread control
        {
            int processed;

        if(alcGetCurrentContext() == NULL)
            COUT(2) << "This should not be!" << std::endl;

            alGetSourcei(audioSource, AL_BUFFERS_PROCESSED, &processed);
            if (ALint error = alGetError())
            COUT(2) << "Sound: Warning: Couldn't get number of processed buffers: " << getALErrorString(error) << std::endl;

            if(processed > 0)
            {
                ALuint* buffers = new ALuint[processed];
                alSourceUnqueueBuffers(audioSource, processed, buffers);
                if (ALint error = alGetError())
                    COUT(2) << "Sound: Warning: Couldn't unqueue buffers: " << getALErrorString(error) << std::endl;

                for(int i = 0; i < processed; i++)
                {
                    long ret = ov_read(&vf, inbuffer, sizeof(inbuffer), 0, 2, 1, &current_section);
                    if (ret == 0)
                    {
                        return;
                    }
                    else if (ret < 0)
                    {
                        COUT(2) << "Sound: libvorbisfile: error reading the file" << std::endl;
                        ov_clear(&vf);
                        return;
                    }

                    alBufferData(buffers[i], format, &inbuffer, ret, vorbisInfo->rate);
                    alGetError();
                }

                alSourceQueueBuffers(audioSource, processed, buffers);
                if (ALint error = alGetError())
                    COUT(2) << "Sound: Warning: Couldn't queue buffers: " << getALErrorString(error) << std::endl;
            }
            msleep(100); // perhaps another value here is better
        }
    }
}
