#include <iostream>
#include "Ambient.h"
#include "../../include/AL/al.h"
#include "../../include/AL/alc.h"
#include "../../include/AL/alut.h"

namespace audio
{
	Ambient::Ambient()
	{
		SourcePos[0]=0;
		SourcePos[1]=0;
		SourcePos[2]=0;

		 SourceVel[0]=0;
		 SourceVel[1]=0;
		 SourceVel[2]=0;

		 ListenerPos[0]=0;
		 ListenerPos[1]=0;
		 ListenerPos[2]=0;

		 ListenerVel[0]=0;
		 ListenerVel[1]=0;
		 ListenerVel[2]=0;

		 ListenerOri[0]=0;
		 ListenerOri[1]=0;
		 ListenerOri[2]=-1;
		 ListenerOri[3]=0;
		 ListenerOri[4]=1;
		 ListenerOri[5]=0;

		std::cout << "Play sone ambient background sound";
	}
	
ALboolean Ambient::LoadALData()
{
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	alGenBuffers(1, &Buffer);

	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	alutLoadWAVFile((ALbyte*)"wavdata/pirate.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alGenSources(1, &Source);

	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	alSourcei (Source, AL_BUFFER,   Buffer   );
	alSourcef (Source, AL_PITCH,    1.0      );
	alSourcef (Source, AL_GAIN,     1.0      );
	alSourcefv(Source, AL_POSITION, SourcePos);
	alSourcefv(Source, AL_VELOCITY, SourceVel);
	alSourcei (Source, AL_LOOPING,  loop     );

	if(alGetError() == AL_NO_ERROR)
		return AL_TRUE;

	return AL_FALSE;
}	
	
	
}

