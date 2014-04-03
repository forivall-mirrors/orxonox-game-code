/*
First try of a scriptController. Target: An event occurs in the levelTry.oxw file, which is "heard" by an object of the type of this class. It then SHOULD (because it is not working) execute the party function.
 */

#include "ScriptController.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    RegisterClass(ScriptController);

    ScriptController::ScriptController(Context* context) : ArtificialController(context)
    {
	//Working
        RegisterObject(ScriptController);
        orxout()<<"hello universe constructor"<< endl;

	this->player_=NULL;
	this->entity_=NULL;
	this->pTrigger_=NULL;
    }

    bool ScriptController::party(bool bTriggered, BaseObject* trigger)
       {
	   //XMLPortEventSink seems not to execute the party function
           orxout()<<"hello universe party"<< endl;
           return true;
       }


    void ScriptController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ScriptController, XMLPort, xmlelement, mode);

	
    	XMLPortEventSink(ScriptController, BaseObject, "party", party, xmlelement, mode);
	// Working
        orxout()<<"hello universe xmlport"<< endl;
    }

    void ScriptController::tick(float dt)
    {

	//Get controllable entity which is attached to this controller in the XML file.
        ControllableEntity* entity = this->getControllableEntity();
        if (!entity)
	    orxout()<<"No controllable entity found"<<endl;
            return;

        

        SUPER(ScriptController, tick, dt);
    }

    void ScriptController::takeControl(Controller * controller, BaseObject * trigger) {

	preparationToTakeControl(trigger);
	setNewController(controller);

    }

	
    bool ScriptController::preparationToTakeControl(BaseObject * trigger) {

	this->pTrigger_ = orxonox_cast<PlayerTrigger*>(trigger);
        this->player_ = NULL;

        // Check whether it is a player trigger and extract pawn from it
        if(pTrigger != NULL)
        {
            if(!pTrigger->isForPlayer()) {  // The PlayerTrigger is not exclusively for Pawns which means we cannot extract one.
                orxout(verbose, context::docking) << "Docking:execute PlayerTrigger was not triggered by a player.." << endl;
                return false;
            }
            player_ = pTrigger->getTriggeringPlayer();  //Get the object which triggered the event.
        }
        else
        {
            orxout(verbose, context::docking) << "Docking::execute Not a player trigger, can't extract pawn from it.." << endl;
            return false;
        }

	
	this->entity_ = this->player_->getControllableEntity();
	assert(this->entity_);

    }

    void ScriptController::setNewController(Controller * controller) {


            orxout(verbose) << "New Controller is going to be set!" << endl;

            this->entity_->setDestroyWhenPlayerLeft(false);
            this->player_->pauseControl();
            this->entity_->setController(controller);
            this->setControllableEntity(this->entity_);
       
    

    }

  


/* Detaillierte Planung
Director nimmt event auf und hängt dann einen controller (momentan als erstellt zu betrachten) an objekt, welches event ausgelöst hat.



	Klassenvariablen

		...? brauchts überhaupt?

	Variablen (in Funktionen auftretend):

		Playerinfo * player 		enthält infos über objekt, welches event ausgelöst hat.Relevant für Inputseite des Directors

	Funktion:

		Auf Seite des Inputs des Directors:

			preparationForControlTakeOver(...) 	Vorbereitende Massnahmen um neuen Controller anzuhängen, z.B. player = pTrigger->getTriggeringPlayer();
								Orientierung an execute Funktion von Dock.cc
	
		Auf Outputseite des Directors:

			takeControl(...) 			Orientierung an DockingController.cc

			tick()					Soll von Event ausgelöst werden, wenn z.B. Kamera an Endposition angelangt ist. Danach soll wieder ein
								menschlicher Spieler die Kontrolle übernehmen (wie? new human controller? Klasse noch angucken!).
*/



}
