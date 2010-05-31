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
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxItem.h>
#include <elements/CEGUIListboxTextItem.h>
#include <CEGUIWindowManager.h>
#include <string>

namespace orxonox 
{
  /* singleton */
  ManageScopedSingleton( ChatInputHandler, ScopeID::Graphics, false );

  /* add commands to console */
  SetConsoleCommandAlias( ChatInputHandler, activate_static, "startchat",
    true );
  SetConsoleCommandAlias( ChatInputHandler, activate_small_static, 
    "startchat_small", true );

  /* constructor */
  ChatInputHandler::ChatInputHandler()
  {
    /* register the object  */
    RegisterObject(ChatInputHandler);

    /* create necessary objects */
    this->inpbuf = new InputBuffer();
    this->disp_offset = 0;
    assert( this->inpbuf != NULL );

    /* generate chatbox ui and chatbox-inputonly ui */
    GUIManager::getInstance().loadGUI( "ChatBox" );
    GUIManager::getInstance().loadGUI( "ChatBox-inputonly" );

    /* setup colors */
    setupColors();

    /* configure the input buffer */
    configureInputBuffer();

    this->inputState = InputManager::getInstance().createInputState( "chatinput", false, false, InputStatePriority::Dynamic );
    this->inputState->setKeyHandler(this->inpbuf);
  }

  /* configure input buffer, sub for the constructor */
  void ChatInputHandler::configureInputBuffer()
  {
    /* INSTALL CALLBACKS */
    /* input has changed */
    this->inpbuf->registerListener(this, &ChatInputHandler::inputChanged, true);

    /* add a line */
    this->inpbuf->registerListener(this, &ChatInputHandler::addline,         '\r',   false);
    this->inpbuf->registerListener(this, &ChatInputHandler::addline,         '\n',   false);

    /* backspace */
    this->inpbuf->registerListener(this, &ChatInputHandler::backspace,       '\b',   true);
    //this->inpbuf->registerListener(this, &ChatInputHandler::backspace,       '\177', true);

    /* exit the chatinputhandler thingy (tbd) */
    this->inpbuf->registerListener(this, &ChatInputHandler::exit,            '\033', true); // escape

    /* delete character */
    this->inpbuf->registerListener(this, &ChatInputHandler::deleteChar,      KeyCode::Delete);

    /* cursor movement */
    this->inpbuf->registerListener(this, &ChatInputHandler::cursorRight,     KeyCode::Right);
    this->inpbuf->registerListener(this, &ChatInputHandler::cursorLeft,      KeyCode::Left);
    this->inpbuf->registerListener(this, &ChatInputHandler::cursorEnd,       KeyCode::End);
    this->inpbuf->registerListener(this, &ChatInputHandler::cursorHome,      KeyCode::Home);

    /* GET WINDOW POINTERS */
    input = CEGUI::WindowManager::getSingleton().getWindow( "orxonox/ChatBox/input" );
    inputonly = CEGUI::WindowManager::getSingleton().getWindow( "orxonox/ChatBox-inputonly/input" );

    /* get pointer to the history window */
    CEGUI::Window *history = CEGUI::WindowManager::getSingleton().getWindow( "orxonox/ChatBox/history" );

    /* cast it to a listbox */
    lb_history = dynamic_cast<CEGUI::Listbox*>(history); 

    /* assert wee */
    assert( lb_history );
  }

  /* setup the colors, sub for the constructor */
  void ChatInputHandler::setupColors()
  {
    /* auto variables */
    float red = 1.0, green = 0.5, blue = 0.5;
    int i = 0;

    // three loops: red tones, blue tones and green tones
    // reds
    for( i = 0; i < NumberOfColors/3; ++i )
    { this->text_colors[ i ] = new CEGUI::colour( red, green, blue );
      assert( this->text_colors[ i ] );
      green += 0.2, blue += 0.2;
    }

    // greens
    red = 0.5, green = 1, blue = 0.5;
    for( ; i < NumberOfColors*2/3; ++i )
    { this->text_colors[ i ] = new CEGUI::colour( red, green, blue );
      assert( this->text_colors[ i ] );
      red += 0.2, blue += 0.2;
    }

    // blues 
    red = 0.5, green = 0.5, blue = 1;
    for( ; i < NumberOfColors; ++i )
    { this->text_colors[ i ] = new CEGUI::colour( red, green, blue );
      assert( this->text_colors[ i ] );
      red += 0.2, green += 0.2;
    }
  }


  /* activate, deactivate */
  void ChatInputHandler::activate_static()
  { ChatInputHandler::getInstance().activate( true ); }

  void ChatInputHandler::activate_small_static()
  { ChatInputHandler::getInstance().activate( false ); }

  void ChatInputHandler::activate( bool full )
  {
    /* start listening */
    InputManager::getInstance().enterState("chatinput");

    /* MARK add spawning of chat widget stuff here.*/
    if( full )
      GUIManager::getInstance().showGUI( "ChatBox" );
    else
      GUIManager::getInstance().showGUI( "ChatBox-inputonly" );

    this->fullchat = full;
  }

  void ChatInputHandler::deactivate() 
  {
    /* stop listening */
    InputManager::getInstance().leaveState("chatinput");

    /* un-spawning of chat widget stuff */
    GUIManager::getInstance().hideGUI( "ChatBox" );
    GUIManager::getInstance().hideGUI( "ChatBox-inputonly" );
  }


  /* subs for incomingChat */
  void ChatInputHandler::sub_setcolor( CEGUI::ListboxTextItem *tocolor,
    std::string name )
  {
    /* sanity checks */
    if( !tocolor )
      COUT(2) << "Empty ListBoxTextItem given to "
        "ChatInputhandler::sub_setcolor().\n";

    /* "hash" the name */
    int hash = 0;
    for( int i = name.length(); i > 0; --i )
      hash += name[i-1];
    hash = hash % this->NumberOfColors;

    /* set the color according to the hash */
    tocolor->setTextColours( *(this->text_colors[ hash ]) );
  }

  /* handle incoming chat */
  void ChatInputHandler::incomingChat(const std::string& message, 
    unsigned int senderID)
  {
    /* look up the actual name of the sender */
    std::string text, name = "unknown";

    /* setup player name info */
    if (senderID != CLIENTID_UNKNOWN)
    { 
       PlayerInfo* player = PlayerManager::getInstance().getClient(senderID);
       if (player)
         name = player->getName();
    }

    /* assemble the text */
    text = name + ": " + message;

    /* create item */
    CEGUI::ListboxTextItem *toadd = new CEGUI::ListboxTextItem( text );

    /* setup colors */
    sub_setcolor( toadd, name );

    /* now add */
    this->lb_history->addItem( dynamic_cast<CEGUI::ListboxItem*>(toadd) );
    this->lb_history->ensureItemIsVisible( 
      dynamic_cast<CEGUI::ListboxItem*>(toadd) );

    /* make sure the history handles it */
    this->lb_history->handleUpdatedItemData();
  } 


  /* sub for inputchanged */
  void ChatInputHandler::sub_adjust_dispoffset( int maxlen, 
    int cursorpos, 
    int inplen )
  {
    /* already start offsetting 5 characters before end */
    if( cursorpos+5 > maxlen )
    { 
      /* always stay 5 characters ahead of end, looks better */
      ((disp_offset = cursorpos-maxlen+5) >= 0) ? 1 : disp_offset = 0;

      /* enforce visibility of cursor */
      (disp_offset > cursorpos ) ? disp_offset = 0 : 1;
    }
     
    /* make sure we don't die at substr */
    if( inplen <= disp_offset ) disp_offset = 0;
  }

  /* callbacks for InputBuffer */
  void ChatInputHandler::inputChanged()
  {
    /* update the cursor and the window */
    std::string raw = this->inpbuf->get();
    int cursorpos = this->inpbuf->getCursorPosition();
    
    /* get string before cursor */
    std::string left = raw.substr( 0, cursorpos );

    /* see if there's a string after the cursor */
    std::string right = "";
    if( raw.length() >= left.length()+1 )
      right = raw.substr( cursorpos );
      
    /* set the text */
    std::string assembled = "$ " + left + "|" + right;

    if( this->fullchat )
    { 
      /* adjust curser position - magic number 5 for font width */
      sub_adjust_dispoffset( (this->input->getUnclippedInnerRect().getWidth()/6), 
        cursorpos, assembled.length() );
      this->input->setProperty( "Text", assembled.substr( disp_offset ) );
    }
    else
    {
      /* adjust curser position - magic number 5 for font width */
      sub_adjust_dispoffset( (this->inputonly->getUnclippedInnerRect().getWidth()/6), 
        cursorpos, assembled.length() );
      this->inputonly->setProperty( "Text", assembled.substr( disp_offset) );
    }

    /* reset display offset */
    disp_offset = 0;
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

    /* d) stop listening to input - only if this is not fullchat */
    if( !this->fullchat )
      this->deactivate();

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
  {
    /* b) clear the input buffer */
    if (this->inpbuf->getSize() > 0)
      this->inpbuf->clear();

    /* d) stop listening to input  */
    this->deactivate();
  }

}
