#include "WiiMote.h"
#include "core/CoreIncludes.h"
#include "core/command/ConsoleCommand.h"
#include <wiicpp/wiicpp/wiicpp.h>

namespace orxonox
{

	const std::string WiiMote::deviceName = "WiiMote";
	void WiiMote::update(const Clock& time)
	{
		orxout()<<"fegit\n";

	}
	void WiiMote::clearBuffers()
	{

	}
	void WiiMote::test(int x, int y)
	{

	}
	WiiMote::WiiMote(unsigned int id)  : InputDevice(id)
	{
	}
}
