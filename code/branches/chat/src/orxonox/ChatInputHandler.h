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

/* std includes */
#include <deque>
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

/* project includes */
#include <OrxonoxPrereqs.h>
#include "core/input/InputBuffer.h"
#include "core/input/InputManager.h"
#include "core/input/InputState.h"
#include "../libraries/network/Host.h"
#include <core/BaseObject.h>
#include <PlayerManager.h>
#include <infos/PlayerInfo.h>
#include <network/ChatListener.h>
#include <core/PathConfig.h>
#include <util/Singleton.h>


namespace orxonox
{
  /* class to handle chat using an InputBuffer */
  class _OrxonoxExport ChatInputHandler : public Singleton<ChatInputHandler>,
    public OrxonoxClass
  {
    private:
      /** Input buffer, to be used to catch input from the
       * keyboard 
       */
      InputBuffer *inpbuf;

      /** input state */
      InputState *inputState;

      /** setup input buffer, the constructor calls this */
      void configureInputBuffer();

      /* singleton pointer */
      static ChatInputHandler* singletonPtr_s;

    public:
      /** constructor */
      ChatInputHandler();
      friend class Singleton<ChatInputHandler>;

      /* start listening, stop listening */
      void activate();
      void deactivate();

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
  };


}


#endif /*_ChatInputHandler_H__*/
