#include "ControllerDirector.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/ControllableEntity.h"




namespace orxonox
{


RegisterClass(ControllerDirector);

    ControllerDirector::ControllerDirector(Context* context) : BaseObject(context)
    {
        RegisterObject(ControllerDirector);

        
    }
    ControllerDirector::~ControllerDirector(){}
    
    
   

}
