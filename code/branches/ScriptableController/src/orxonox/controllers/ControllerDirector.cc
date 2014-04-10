/*
First try of a ControllerDirector. Target: An event occurs in the levelTry.oxw file, which is "heard" by an object of the type of this class. It then SHOULD (because it is not working) execute the party function.
 */

#include "ControllerDirector.h"
#include "core/CoreIncludes.h"

//#include "network/NetworkFunction.h"

#include "infos/HumanPlayer.h"
#include "interfaces/PlayerTrigger.h"
#include "worldentities/pawns/Pawn.h"

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

    bool ControllerDirector::party(bool bTriggered, BaseObject* trigger)
       {
	   //XMLPortEventSink seems not to execute the party function
           orxout()<<"hello universe party"<< endl;
           return true;
       }


    void ControllerDirector::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ControllerDirector, XMLPort, xmlelement, mode);

	
        orxout()<<"hello universe xmlport"<< endl;
    }

    void ControllerDirector::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ControllerDirector, XMLEventPort, xmlelement, mode);

        XMLPortEventSink(ControllerDirector, BaseObject, "party", party, xmlelement, mode);

    }



   /* void ControllerDirector::tick(float dt)
    {

	//Get controllable entity which is attached to this controller in the XML file.
        ControllableEntity* entity = this->getControllableEntity();
        if (!entity)
	    orxout()<<"No controllable entity found"<<endl;
            return;

        

        SUPER(ControllerDirector, tick, dt);
    }

    */

    void ControllerDirector::takeControl(Controller * controller, BaseObject * trigger) {

        preparationToTakeControl(trigger);
        setNewController(controller);

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



