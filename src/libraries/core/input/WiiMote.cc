#include "WiiMote.h"
#include "core/CoreIncludes.h"
//#include "core/command/ConsoleCommand.h"
#include <wiicpp/wiicpp/wiicpp.h>


namespace orxonox
{

	const std::string WiiMote::deviceName = "WiiMote";
	void WiiMote::update(const Clock& time)
	{
		orxout(user_warning) << this->inputStates_.size() << std::endl;
		if(p == NULL)
	      exit(0);
		//else
		  //orxout(user_warning) << "pointer is not null, yay" << std::endl;
		if (PWii->Poll())
		{
			CWiimote::EventTypes e = p->GetEvent();
			if(p->Buttons.isPressed(CButtons::BUTTON_A))
				{
					CE->execute("fire 0", 0, 0);
				}
			Orientation o;
			p->SetMotionSensingMode(CWiimote::ON);
			p->Accelerometer.GetOrientation(o.pitch, o.roll, o.yaw);
			int x = (int)(500*(o.yaw-lastOrientation.yaw)/time.getDeltaTime());
			int y = (int)((o.pitch-lastOrientation.pitch)/time.getDeltaTime());
			IntVector2 abs(0, 0);
			IntVector2 rel(x, y);
			IntVector2 clippingSize(1920, 1080);
			lastOrientation.pitch = o.pitch;
			lastOrientation.yaw = o.yaw;
			orxout()<<x<<std::endl<<y<<std::endl;
			if(x!=0 || y!=0)
			{
				for (unsigned int i = 0; i < inputStates_.size(); ++i)
								inputStates_[i]->mouseMoved(abs, rel, clippingSize);
			}
		}


	}
	void WiiMote::clearBuffers()
	{

	}
	void WiiMote::test(int x, int y)
	{

	}
	WiiMote::WiiMote(unsigned int id, CWiimote & parent, CWii & parentWii)  : InputDevice(id)
	{
		p = &parent;
		PWii = &parentWii;
		lastOrientation.yaw = 0;
		lastOrientation.roll = 0;
		lastOrientation.pitch = 0;
		CE = &(CommandExecutor::getInstance());

	}
}
