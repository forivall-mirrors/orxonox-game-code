#ifndef __CLASS_AMBIENT_H__
#define __CLASS_AMBIENT_H__

#include <iostream>
#include "../../include/AL/al.h"
#include "../../include/AL/alc.h"
#include "../../include/AL/alut.h"


namespace audio
{
	class Ambient
	{
	public:
		Ambient();
		void play();
	private:
		ALboolean LoadALData();	
		void SetListenerValues();
		void KillALData();

		ALuint Buffer;
		ALuint Source;
		ALfloat SourcePos[3];
		ALfloat SourceVel[3];
		ALfloat ListenerPos[3];
		ALfloat ListenerVel[3];
		ALfloat ListenerOri[6];
	};
	
	
	
	
	
}

#endif
