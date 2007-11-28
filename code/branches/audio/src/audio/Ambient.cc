#include <iostream>
#include <string>
#include "Ambient.h"

namespace audio
{
	Ambient::Ambient(std::string audioFile)
	{
		a = new AudioObject(audioFile);
	}
	
	Ambient::~Ambient()
	{	
		
	}
	
	void Ambient::play()
	{
		a->play();	
	}
}

