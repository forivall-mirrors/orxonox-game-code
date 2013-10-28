#include "WiiMote.h"
#include "core/CoreIncludes.h"
#include "core/command/ConsoleCommand.h"

namespace orxonox
{

	const std::string WiiMote::deviceName = "WiiMote";
	void WiiMote::update(const Clock& time)
	{
		orxout()<<"fegit";

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
