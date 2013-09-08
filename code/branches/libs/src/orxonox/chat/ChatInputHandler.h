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

#ifndef _ChatInputHandler_H__
#define _ChatInputHandler_H__

#include "OrxonoxPrereqs.h"

#include <string>

#if CEGUI_VERSION >= 0x000800
#   include <CEGUI/ForwardRefs.h>
#   include <CEGUI/Colour.h>
#else
#   include <CEGUIForwardRefs.h>
#   include <CEGUIcolour.h>
#endif

#include "util/Singleton.h"
#include "chat/ChatListener.h"

namespace orxonox // tolua_export
{ // tolua_export

#if CEGUI_VERSION >= 0x000800
  typedef CEGUI::Colour CEGUIColour;
#else
  typedef CEGUI::colour CEGUIColour;
#endif

  /* class to handle chat using an InputBuffer */
  class _OrxonoxExport ChatInputHandler  // tolua_export
    : public Singleton<ChatInputHandler>, public ChatListener
  { // tolua_export
    private:
      /** Input buffer, to be used to catch input from the
       * keyboard
       */
      InputBuffer *inpbuf;
      int disp_offset, width;
      bool fullchat;

      /* colors for nickname coloring */
      static const int NumberOfColors = 10;
      CEGUIColour text_colors[ NumberOfColors ];

      /** input state */
      InputState *inputState;

      /** setup input buffer, the constructor calls this */
      void configureInputBuffer();

      /** adjust display offset depending on cursor position */
      void sub_adjust_dispoffset( int maxlen, int cursorpos, int inplen );

      /** singleton pointer */
      static ChatInputHandler* singletonPtr_s;

      /** cegui window handles */
      CEGUI::Window *input, *inputonly;

      /** cegui handle for the history window */
      CEGUI::Listbox *lb_history;

      /* methods to deal with colors */
      void sub_setcolor( CEGUI::ListboxTextItem *tocolor,
        std::string name );

      void setupColors();

      /* callbacks for input handler */
      void inputChanged();
      void addline();
      void backspace();
      void deleteChar();
      void cursorRight();
      void cursorLeft();
      void cursorEnd();
      void cursorHome();
      void exit();

    public:
      /** constructor */
      ChatInputHandler();
      ~ChatInputHandler();
      friend class Singleton<ChatInputHandler>;

      static ChatInputHandler& getInstance(void) { return Singleton<ChatInputHandler>::getInstance(); }  // tolua_export

      /** start listening */
      static void activate_static();

      /** stop listening */
      static void activate_small_static();

      /** \param message the message text
       * \param senderID ID of the player who sent the message
       *
       * Deal with incoming chat (which means in our case: Add it to the
       * history window of the full chat window)
       */
      void incomingChat(const std::string& message, const std::string& name);

      /** \param full true means show full chat window with history,
            false means show only an input line
       *
       * Show the chat window and redirect the game's keyboard input
       * into it.
       */
      void activate( bool full );

      /** Deactivate the chat window, meaning: hide it. */
      void deactivate();  // tolua_export

  };  // tolua_export


}  // tolua_export


#endif /*_ChatInputHandler_H__*/
