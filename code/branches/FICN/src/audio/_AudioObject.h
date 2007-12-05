#ifndef AUDIOOBJECT_H_
#define AUDIOOBJECT_H_

#include <iostream>
#include "../../include/AL/al.h"
#include "../../include/AL/alc.h"
#include "../../include/AL/alut.h"

namespace audio
{
	class AudioObject
	{
	public:
		AudioObject(std::string audioFile);
		~AudioObject();

		void play();
		void pause();
		void stop();

	private:
		ALboolean LoadALData();	
		void SetListenerValues();
		void KillALData();
		
		std::string audioFile_;
		
		ALuint Buffer;
		ALuint Source;
		ALfloat SourcePos[3];
		ALfloat SourceVel[3];
		ALfloat ListenerPos[3];
		ALfloat ListenerVel[3];
		ALfloat ListenerOri[6];
	};	
}

#endif /*AUDIOOBJECT_H_*/
