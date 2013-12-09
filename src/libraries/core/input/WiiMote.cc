#include "WiiMote.h"
#include "core/CoreIncludes.h"
//#include "core/command/ConsoleCommand.h"
#include <wiicpp/wiicpp/wiicpp.h>
#include "core/command/CommandExecutor.h"


namespace orxonox
{

	const std::string WiiMote::deviceName = "WiiMote";
	void WiiMote::update(const Clock& time)
	{
		//orxout(user_warning) << this->inputStates_.size() << std::endl;
		if(p == NULL)
	      exit(0);
		//else
		  //orxout(user_warning) << "pointer is not null, yay" << std::endl;
		PWii->RefreshWiimotes();
		if(PWii->Poll())
		{
			orxout()<<"test11"<<endl;
			switch (p->GetEvent())
			{
				case CWiimote::EVENT_EVENT:
				{
					if(p->Buttons.isPressed(CButtons::BUTTON_A)||p->Buttons.isJustPressed(CButtons::BUTTON_A)) //ugly hack to just do something on button press easily
										{
											orxout()<<"fak u dolan"<<endl;
											CommandExecutor::execute("fire 0", 0, 0);
										}
					Orientation o;
					p->Accelerometer.GetOrientation(o.pitch, o.roll, o.yaw);
					int x = (int)(500*(o.yaw-lastOrientation.yaw)/time.getDeltaTime()); //get difference in orientation, divide by time to make faster movements result in faster orientation change
					int y = (int)((o.pitch-lastOrientation.pitch)/time.getDeltaTime());
					orxout()<<time.getDeltaTime()<<std::endl;
					IntVector2 abs(0, 0);
					IntVector2 rel(x, y);
					IntVector2 clippingSize(1920, 1080);
					lastOrientation.pitch = o.pitch;
					lastOrientation.yaw = o.yaw;
					orxout()<<x<<std::endl<<y<<std::endl;
					if(x!=0 || y!=0)
					{
						for (unsigned int i = 0; i < inputStates_.size(); ++i)
						inputStates_[i]->mouseMoved(abs, rel, clippingSize); 	//pass random mouse movements to all input states
					}
					break;
				}
				case CWiimote::EVENT_STATUS:
				{

					break;
				}
				default:
					break;

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

	}
}
