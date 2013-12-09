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
		if(p == NULL)
	      exit(0);
		PWii->RefreshWiimotes();
		Orientation o;
		Orientation toMove;
		for (int i=0; i<4; i++)
		{
			if(PWii->Poll())
				{
					//orxout()<<"test11"<<endl;
					switch (p->GetEvent())
					{
						case CWiimote::EVENT_EVENT:
						{
							if(p->Buttons.isPressed(CButtons::BUTTON_A)||p->Buttons.isJustPressed(CButtons::BUTTON_A)) //ugly hack to just do something on button press easily
								{
									orxout()<<"fak u dolan"<<endl;
									CommandExecutor::execute("fire 0", 0, 0);
								}
							if (i==0)
								p->Accelerometer.GetOrientation(o.pitch, o.roll, o.yaw);
							//orxout()<<time.getDeltaTime()<<std::endl;

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
		//orxout() << time.getDeltaTime()<<endl;
		orxout() << o.pitch << endl;
	//	int x = (int)(-10*(o.yaw-lastOrientation.yaw)); //get difference in orientation, divide by time to make faster movements result in faster orientation change
		int y = (int)(4*(o.pitch-lastOrientation.pitch));//-lastOrientation.pitch)/time.getDeltaTime());
		int x=0;
		//orxout() << x<< endl << y << endl;
		IntVector2 abs(0,0);
		IntVector2 rel(x,y);
		IntVector2 clippingSize(1920, 1080);


		if((x!=0 || y!=0)&&(o.pitch!=0))
			{
				for (unsigned int i = 0; i < inputStates_.size(); ++i)
					inputStates_[i]->mouseMoved(abs, rel, clippingSize); 	//pass random mouse movements to all input states
			}
		lastOrientation = o;
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
