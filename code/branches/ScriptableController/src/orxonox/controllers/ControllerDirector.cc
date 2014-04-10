/*
First try of a ControllerDirector. Target: An event occurs in the levelTry.oxw file, which is "heard" by an object of the type of this class. It then SHOULD (because it is not working) execute the party function.
 */

#include "ControllerDirector.h"
#include "core/CoreIncludes.h"

//#include "network/NetworkFunction.h"

#include "infos/HumanPlayer.h"
#include "interfaces/PlayerTrigger.h"
#include "worldentities/pawns/Pawn.h"
#include "LuaState.h"


namespace orxonox
{
    RegisterClass(ControllerDirector);

    ControllerDirector::ControllerDirector(Context* context) : ArtificialController(context)
    {
	//Working
        RegisterObject(ControllerDirector);
        orxout()<<"hello universe constructor"<< endl;

	   this->player_=NULL;
	   this->entity_=NULL;
	   this->pTrigger_=NULL;
    }


    void ControllerDirector::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ControllerDirector, XMLPort, xmlelement, mode);

	
        orxout()<<"ControllerDriector::XMLPort An instance of ControllerDirector has been created."<< endl;
    }

    void ControllerDirector::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ControllerDirector, XMLEventPort, xmlelement, mode);

        XMLPortEventSink(ControllerDirector, BaseObject, "takeControl", takeControl, xmlelement, mode);

    }




    void ControllerDirector::takeControl(Controller * controller, BaseObject * trigger) {

        preparationToTakeControl(trigger);
        setNewController(controller);
        LuaState test = new Luastate();
        test.doFile("/tmp/myluahelloworld.lua");
        
    } 

	
    bool ControllerDirector::preparationToTakeControl(BaseObject * trigger) {

	    this->pTrigger_ = orxonox_cast<PlayerTrigger*>(trigger);
        this->player_ = NULL;

        orxout() << "Preparation to take Control!" << endl; 
        // Check whether it is a player trigger and extract pawn from it
        if(this->pTrigger_ != NULL)
        {
            
            player_ = this->pTrigger_->getTriggeringPlayer();  //Get the object which triggered the event.
        }
        else
        {
            orxout() << "ControllerDirector::preparationToTakeControl Not a player trigger, can't extract pawn from it.." << endl;
            return false;
        }

	
	this->entity_ = this->player_->getControllableEntity();
	assert(this->entity_);

    return true;

    }

    void ControllerDirector::setNewController(Controller * controller) {


        orxout() << "New Controller is going to be set!" << endl;

        this->entity_->setDestroyWhenPlayerLeft(false);
        this->player_->pauseControl();
        this->entity_->setController(controller);
        this->setControllableEntity(this->entity_);



    }
       
    

}




