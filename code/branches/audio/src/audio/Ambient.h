#ifndef __CLASS_AMBIENT_H__
#define __CLASS_AMBIENT_H__

#include <iostream>
#include "../openal/al.h"
#include "../openal/alc.h"
#include "../openal/alut.h"


namespace audio
{
	class Ambient
	{
	public:
		Ambient();
	private:
		ALboolean LoadALData();	


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
