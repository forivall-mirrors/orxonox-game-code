/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Sandro 'smerkli' Merkli
 *   Co-authors:
 *      ...
 *
 */

#include "ChatInputHandler.h"
#include <core/ScopedSingletonManager.h>
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/GUIManager.h"
#include "core/CorePrereqs.h"
#include <CEGUIWindow.h>
#include <CEGUI/elements/CEGUIListbox.h>
#include <CEGUI/elements/CEGUIListboxItem.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUIWindowManager.h>
#include <string>

namespace orxonox 
{
  /* singleton */
  ManageScopedSingleton( ChatInputHandler, ScopeID::Graphics, false );
  SetConsoleCommandAlias( ChatInputHandler, activate_static, "startchat",
    true );


  /* constructor */
  ChatInputHandler::ChatInputHandler()
  {
    /* register the object  */
    RegisterObject(ChatInputHandler);

    /* create necessary objects */
    this->inpbuf = new InputBuffer();
    assert( this->inpbuf != NULL );

    /* MARK add generation of ChatBox thingy here */
    GUIManager::getInstance().loadGUI( "ChatBox" );

    /* configure the input buffer */
    configureInputBuffer();

    this->inputState = InputManager::getInstance().createInputState( "chatinput", false, false, InputStatePriority::Dynamic );
    this->inputState->setKeyHandler(this->inpbuf);

    //[> set console shortcut <]
    //this->getIdentifier()->addConsoleCommand(createConsoleCommand(createFunctor(
      //&ChatInputHandler::activate, this), "startchat"), false);
  }

  void ChatInputHandler::configureInputBuffer()
  {
    /* input has changed */
    this->inpbuf->registerListener(this, &ChatInputHandler::inputChanged, true);

    /* add a line */
    this->inpbuf->registerListener(this, &ChatInputHandler::addline,         '\r',   false);
    this->inpbuf->registerListener(this, &ChatInputHandler::addline,         '\n',   false);

    /* backspace */
    this->inpbuf->registerListener(this, &ChatInputHandler::backspace,       '\b',   true);
    this->inpbuf->registerListener(this, &ChatInputHandler::backspace,       '\177', true);

    /* exit the chatinputhandler thingy (tbd) */
    this->inpbuf->registerListener(this, &ChatInputHandler::exit,            '\033', true); // escape

    /* delete character */
    this->inpbuf->registerListener(this, &ChatInputHandler::deleteChar,      KeyCode::Delete);

    /* cursor movement */
    this->inpbuf->registerListener(this, &ChatInputHandler::cursorRight,     KeyCode::Right);
    this->inpbuf->registerListener(this, &ChatInputHandler::cursorLeft,      KeyCode::Left);
    this->inpbuf->registerListener(this, &ChatInputHandler::cursorEnd,       KeyCode::End);
    this->inpbuf->registerListener(this, &ChatInputHandler::cursorHome,      KeyCode::Home);

    /* get window pointers */
    input = CEGUI::WindowManager::getSingleton().getWindow( "orxonox/ChatBox/input" );
    CEGUI::Window *history = CEGUI::WindowManager::getSingleton().getWindow( "orxonox/ChatBox/history" );
    lb_history = dynamic_cast<CEGUI::Listbox*>(history); 

    /* assert wee */
    assert( lb_history );
  }


  /* activate, deactivate */
  void ChatInputHandler::activate_static()
  { ChatInputHandler::getInstance().activate(); }
  
  void ChatInputHandler::activate()
  {
    /* start listening */
    //COUT(0) << "chatinput activated." << std::endl;
    InputManager::getInstance().enterState("chatinput");

    /* MARK add spawning of chat widget stuff here.*/
    GUIManager::getInstance().showGUI( "ChatBox" );
  }

  void ChatInputHandler::deactivate() 
  {
    /* stop listening */
    InputManager::getInstance().leaveState("chatinput");

    /* MARK add un-spawning of chat widget stuff here. */
    GUIManager::getInstance().hideGUI( "ChatBox" );
  }

  /* callbacks for InputBuffer */
  void ChatInputHandler::inputChanged()
  {
    /* update the cursor and the window */
    std::string raw = this->inpbuf->get();
    
    /* get string before cursor */
    std::string left = raw.substr( 0, this->inpbuf->getCursorPosition() );

    /* see if there's a string after the cursor */
    std::string right = "";
    if( raw.length() >= left.length()+1 )
      right = raw.substr( this->inpbuf->getCursorPosition() );
      
    /* set the text */
    this->input->setProperty( "Text", left + "|" + right );
  }

  void ChatInputHandler::addline()
  {
    /* actually do send what was input */
    /* a) get the string out of the inputbuffer */
    std::string msgtosend = this->inpbuf->get();

    if( msgtosend.length() == 0 )
    { this->deactivate();
      return;
    }

    /* b) clear the input buffer */
    if (this->inpbuf->getSize() > 0)
      this->inpbuf->clear();

    /* c) send the chat via some call */
    Host::Chat( msgtosend );

    /* d) stop listening to input  */
    this->deactivate();

    /* e) create item and add to history */
    CEGUI::ListboxTextItem *toadd = new CEGUI::ListboxTextItem( msgtosend );
    this->lb_history->addItem( dynamic_cast<CEGUI::ListboxItem*>(toadd) );
    this->lb_history->ensureItemIsVisible( dynamic_cast<CEGUI::ListboxItem*>(toadd) );

    /* f) make sure the history handles it */
    this->lb_history->handleUpdatedItemData();
  }

  void ChatInputHandler::backspace()
  { this->inpbuf->removeBehindCursor(); }

  void ChatInputHandler::deleteChar()
  { this->inpbuf->removeAtCursor(); }

  void ChatInputHandler::cursorRight()
  { this->inpbuf->increaseCursor(); }
  
  void ChatInputHandler::cursorLeft()
  { this->inpbuf->decreaseCursor(); }
  
  void ChatInputHandler::cursorEnd()
  { this->inpbuf->setCursorToEnd(); }

  void ChatInputHandler::cursorHome()
  { this->inpbuf->setCursorToBegin(); }

  void ChatInputHandler::exit()
  { }

}
