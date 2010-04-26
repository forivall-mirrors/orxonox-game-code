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

namespace orxonox 
{
  /* constructor */
  void ChatInputHandler::ChatInputHandler()
  {
    /* create necessary objects */
    this->inpbuf = new InputBuffer();

		configureInputBuffer();
  }

  void ChatInputHandler::configureInputBuffer()
  {
	  /* input has changed */
    this->inputBuffer_->registerListener(this, &ChatInputHandler::inputChanged, true);
		
		/* add a line */
    this->inputBuffer_->registerListener(this, &ChatInputHandler::addline,         '\r',   false);
    this->inputBuffer_->registerListener(this, &ChatInputHandler::addline,         '\n',   false);

		/* backspace */
    this->inputBuffer_->registerListener(this, &ChatInputHandler::backspace,       '\b',   true);
    this->inputBuffer_->registerListener(this, &ChatInputHandler::backspace,       '\177', true);

		/* exit the chatinputhandler thingy (tbd) */
    this->inputBuffer_->registerListener(this, &ChatInputHandler::exit,            '\033', true); // escape

		/* delete character */
    this->inputBuffer_->registerListener(this, &ChatInputHandler::deleteChar,      KeyCode::Delete);

		/* cursor movement */
    this->inputBuffer_->registerListener(this, &ChatInputHandler::cursorRight,     KeyCode::Right);
    this->inputBuffer_->registerListener(this, &ChatInputHandler::cursorLeft,      KeyCode::Left);
    this->inputBuffer_->registerListener(this, &ChatInputHandler::cursorEnd,       KeyCode::End);
    this->inputBuffer_->registerListener(this, &ChatInputHandler::cursorHome,      KeyCode::Home);
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

    /* b) clear the input buffer */

    /* c) send the chat via some call */

    /* d) stop listening to input  */
  }

  void ChatInputHandler::backspace()
  {
    this->inputBuffer_->removeBehindCursor();
    //this->updateListeners<&ShellListener::inputChanged>();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }

  void ChatInputHandler::deleteChar()
  {
    this->inputBuffer_->removeAtCursor();
    //this->updateListeners<&ShellListener::inputChanged>();
  }

  void ChatInputHandler::cursorRight()
  {
    this->inputBuffer_->increaseCursor();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }
  
  void ChatInputHandler::cursorLeft()
  {
    this->inputBuffer_->decreaseCursor();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }
  
  void ChatInputHandler::cursorEnd()
  {
    this->inputBuffer_->setCursorToEnd();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }

  void ChatInputHandler::cursorHome()
  {
    this->inputBuffer_->setCursorToBegin();
    //this->updateListeners<&ShellListener::cursorChanged>();
  }

  void ChatInputHandler::exit()
  {
    //if (this->inputBuffer_->getSize() > 0)
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
