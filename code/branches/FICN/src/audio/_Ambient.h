#ifndef __CLASS_AMBIENT_H__
#define __CLASS_AMBIENT_H__

#include <iostream>
#include "../../include/AL/al.h"
#include "../../include/AL/alc.h"
#include "../../include/AL/alut.h"

#include "AudioObject.h"

namespace audio
{
	class Ambient
	{
	public:
		Ambient(std::string audioFile);
		~Ambient();
		void play();
	private:
		AudioObject* a;
	};
}

#endif
