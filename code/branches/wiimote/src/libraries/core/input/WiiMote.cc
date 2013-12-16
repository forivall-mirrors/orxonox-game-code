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
		IntVector2 o(0,0);
		float r = 0; //roll variable
		if (p->ExpansionDevice.GetType()!=CExpansionDevice::TYPE_NUNCHUK)
			{
				p->UpdateStatus();
				PWii->RefreshWiimotes();
			}
		for (int i=0; i<4; i++)
		{
			if(PWii->Poll())
				{
					switch (p->GetEvent())
					{
						case CWiimote::EVENT_EVENT:
						{
							if(p->Buttons.isPressed(CButtons::BUTTON_A)||p->Buttons.isJustPressed(CButtons::BUTTON_A)) //ugly hack to just do something on button press easily
							{
								//orxout()<<"fak u dolan"<<endl;
								CommandExecutor::execute("fire 0", 0, 0);
							}
							if(p->Buttons.isPressed(CButtons::BUTTON_B)||p->Buttons.isJustPressed(CButtons::BUTTON_B))
								CommandExecutor::execute("boost");
							if(p->ExpansionDevice.GetType()==CExpansionDevice::TYPE_NUNCHUK)
							{
								if(p->ExpansionDevice.Nunchuk.Buttons.isPressed(CNunchukButtons::BUTTON_C))
									CommandExecutor::execute("NewHumanController accelerate");
								if(p->ExpansionDevice.Nunchuk.Buttons.isPressed(CNunchukButtons::BUTTON_Z))
									CommandExecutor::execute("NewHumanController decelerate");
							}
							float dummyPitch, dummyYaw, dummyRoll;
							p->Accelerometer.GetOrientation(dummyPitch, dummyRoll, dummyYaw);
							r += dummyRoll;
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
		r/=4;
		std::stringstream temp;
		if (r>4.5)
			temp << "HumanController rotateRoll " << r/6 << ",1";
		if (r<-4.5)
			temp << "HumanController rotateRoll "<< r/6 << ",-1";
		if (r==0)
			temp << "";
		string com = temp.str();
		CommandExecutor::execute(com, 0, 0);

		IntVector2 abs(0,0);
		IntVector2 rel(0,0);
		IntVector2 clippingSize(1920, 1080);
		p->IR.GetCursorPosition(o.x, o.y);
		rel.x = (o.x-lastCursor.x);
		rel.y = (o.y-lastCursor.y);
		abs.x = o.x;
		abs.y = o.y;
		if((rel.x!=0 || rel.y!=0))
					{
						for (unsigned int i = 0; i < inputStates_.size(); ++i)
							inputStates_[i]->mouseMoved(abs, rel, clippingSize); 	//pass random mouse movements to all input states
					}
		lastCursor.x = o.x;
		lastCursor.y = o.y;

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
		lastCursor.x = 0;
		lastCursor.y = 0;
	}
}
