
#include "InputDevice.h"
//#include "InputState.h"
//#include "InputManager.h"



namespace orxonox
{
	class WiiMote : public InputDevice
	{
	 public:
			//! Only resets the members
        	WiiMote(unsigned int id);
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
	         //! List of all input states that receive events from this device
	         std::vector<InputState*> inputStates_;
	 private:
	         static const std::string deviceName;
	         void test(int x, int y);
	};
};
