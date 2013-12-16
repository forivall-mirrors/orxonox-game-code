
#include "InputDevice.h"
//#include "InputManager.h"

#include <wiicpp/wiicpp/wiicpp.h>



namespace orxonox
{
	struct Orientation
    {
    	float roll, pitch, yaw;
    };

	class WiiMote : public InputDevice
	{

	 public:
			//! Only resets the members
        	WiiMote(unsigned int id, CWiimote & parent, CWii & parentWii);
        	~WiiMote() { }
	        //! Returns the device class (derived) name as string
	        std::string getClassName() const {return deviceName;}
	        //! Updates the device which should in turn distribute events
	        void update(const Clock& time);
	        //! Clear all button related buffers
	        void clearBuffers();

	 protected:
	        void calibrationStarted() { }
	        void calibrationStopped() { }
	        Orientation lastOrientation;
	        Orientation averageOrientation(Orientation o[], int l);

	 private:
	         static const std::string deviceName;
	         void test(int x, int y);
	         CWiimote * p;
	         CWii * PWii; //parent "wii" the wiimote is registered to
	         IntVector2 lastCursor;
	};
};
