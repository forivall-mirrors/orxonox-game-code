/*
First try of a scriptController. Target: An event occurs in the levelTry.oxw file, which is "heard" by an object of the type of this class. It then SHOULD (because it is not working) execute the party function.
 */

#include "ScriptController_00.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    RegisterClass(ScriptController_00);

    ScriptController_00::ScriptController_00(Context* context) : ArtificialController(context)
    {
	//Working
        RegisterObject(ScriptController_00);
        orxout()<<"hello universe constructor"<< endl;
    }

    bool ScriptController_00::party(bool bTriggered, BaseObject* trigger)
       {
	   //XMLPortEventSink seems not to execute the party function
           orxout()<<"hello universe party"<< endl;
           return true;
       }


    void ScriptController_00::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ScriptController_00, XMLPort, xmlelement, mode);

	//WORKING
    	XMLPortEventSink(ScriptController_00, BaseObject, "party", party, xmlelement, mode);
        orxout()<<"hello universe xmlport"<< endl;
    }




}
