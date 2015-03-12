/*
 * First try of a ControllerDirector. Target: An event occurs in the levelTry.oxw
 * file, which is "heard" by an object of the type of this class. It then SHOULD
 * (because it is not working) execute the party function.
 */

#include "ControllerDirector.h"
#include "ScriptController.h"
#include "core/CoreIncludes.h"

//#include "network/NetworkFunction.h"

#include "infos/HumanPlayer.h"
#include "interfaces/PlayerTrigger.h"
#include "worldentities/pawns/Pawn.h"
#include "core/LuaState.h"


namespace orxonox
{
    RegisterClass(ControllerDirector);

    ControllerDirector::ControllerDirector(Context* context) : ArtificialController(context)
    {
        // Register the object with the framework
        RegisterObject(ControllerDirector);

        // output a message to ensure we know the constructor was run
        orxout(verbose)<<"hello universe constructor blablub"<< endl;



        // Initialize member variables
        this->player_ = NULL;
        this->entity_ = NULL;
        this->pTrigger_ = NULL;
        this->context_ = context;
    }

    void ControllerDirector::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ControllerDirector, XMLPort, xmlelement, mode);

        orxout(verbose)<< "ControllerDirector::XMLPort " 
          << "An instance of ControllerDirector has been created." << endl;
    }

    void ControllerDirector::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        // Call the xmleventport functions of the classes we derive from
        SUPER(ControllerDirector, XMLEventPort, xmlelement, mode);

        // Add an event sink for a "takeControl" event, which leads to the
        // function takeControl() being called.
        XMLPortEventSink(ControllerDirector, BaseObject, "takeControl", 
          takeControl, xmlelement, mode);
    }




    void ControllerDirector::takeControl(Controller * controller, BaseObject * trigger) 
    {
       /* Output a message confirming that the function was called */
       orxout(verbose)<<"test takecontrol."<< endl;

       /* First, we set up a new controller to attach to the unit that
        * triggered our event. 
        */
       static int ctrlid = 1;
       bool prepok = preparationToTakeControl(trigger);
       if( prepok == true) 
       {
         /* Create a scriptcontroller object */
         ScriptController *newctrl = new ScriptController(this->context_);

         /* Make the player we were given its slave */
         newctrl->setPlayer(this->player_);

         /* Start controlling that object */
         newctrl->takeControl(ctrlid);
       }
       else
         return;
       
       /* Set up a luastate to use for running the scripts */
       LuaState * ls = new LuaState();
       
       /* Assemble a string to define a controller id variable in lua space */
       std::stringstream tmp;
       tmp << "newctrlid = " << ctrlid << endl;
       std::string todo = tmp.str();
       
       /* Run the string using the luastate created earlier */
       ls->doString(todo);

       /* Now run the script on this controller. This will still have the above
        * variable "newctrlid" defined, which means it can make use of it.
        */

       ls->doFile("testscript.lua");

       /* Increase the controller ID so we have a different one for
        * the next time it is triggered */
       ctrlid += 1;
    } 

    bool ControllerDirector::preparationToTakeControl(BaseObject * trigger) 
    {
        this->pTrigger_ = orxonox_cast<PlayerTrigger*>(trigger);
        this->player_ = NULL;

        orxout(verbose) << "Preparation to take Control!" << endl; 

        // Check whether it is a player trigger and extract pawn from it
        if(this->pTrigger_ != NULL)
        {
            // Get the object which triggered the event.
            this->player_ = this->pTrigger_->getTriggeringPlayer();  

            // Check if there actually was a player returned.
            if( this->player_ == NULL) return false;
        }
        else
        {
            orxout(verbose) << "ControllerDirector::preparationToTakeControl " 
              << "Not a player trigger, can't extract pawn from it.." << endl;
            return false;
        }

        this->entity_ = this->player_->getControllableEntity();
        assert(this->entity_);

        return true;
    }

    /* // Currently unused
    void ControllerDirector::setNewController(Controller * controller) {


        orxout() << "New Controller is going to be set!" << endl;

        this->entity_->setDestroyWhenPlayerLeft(false);
        this->player_->pauseControl();
        this->entity_->setController(controller);
        this->player_->startControl(this->entity_);
        //this->setControllableEntity(this->entity_);
    }
    */
       
    

}




