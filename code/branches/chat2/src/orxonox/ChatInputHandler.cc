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
  }


  /* activate, deactivate */
  void ChatInputHandler::activate_static()
  { ChatInputHandler::getInstance().activate(); }
  
  void ChatInputHandler::activate()
  {
    /* start listening */
    COUT(0) << "chatinput activated." << std::endl;
    InputManager::getInstance().enterState("chatinput");
  }

  void ChatInputHandler::deactivate() 
  {
    /* stop listening */
    InputManager::getInstance().leaveState("chatinput");
  }



  /* callbacks for InputBuffer */
  void ChatInputHandler::inputChanged()
  {
    //this->updateListeners<&ShellListener::inputChanged>();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }

  void ChatInputHandler::addline()
  {
    /* MARK MARK */

    /* actually do send what was input */
    /* a) get the string out of the inputbuffer */
    std::string msgtosend = this->inpbuf->get();

    /* b) clear the input buffer */
    if (this->inpbuf->getSize() > 0)
      this->inpbuf->clear();

    /* c) send the chat via some call */
    Host::Chat( msgtosend );

    /* d) stop listening to input  */
    this->deactivate();
  }

  void ChatInputHandler::backspace()
  {
    this->inpbuf->removeBehindCursor();
    //this->updateListeners<&ShellListener::inputChanged>();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }

  void ChatInputHandler::deleteChar()
  {
    this->inpbuf->removeAtCursor();
    //this->updateListeners<&ShellListener::inputChanged>();
  }

  void ChatInputHandler::cursorRight()
  {
    this->inpbuf->increaseCursor();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }
  
  void ChatInputHandler::cursorLeft()
  {
    this->inpbuf->decreaseCursor();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }
  
  void ChatInputHandler::cursorEnd()
  {
    this->inpbuf->setCursorToEnd();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }

  void ChatInputHandler::cursorHome()
  {
    this->inpbuf->setCursorToBegin();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }

  void ChatInputHandler::exit()
  {
    //if (this->inpbuf->getSize() > 0)
    //{
      //this->clearInput();
      //return;
    //}

    //this->clearInput();
    //this->scrollPosition_ = 0;
    //this->scrollIterator_ = this->outputLines_.begin();

    //this->updateListeners<&ShellListener::exit>();
  }

}
