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
#include <InputBuffer.h>


namespace orxonox
{
  /* class to handle chat using an InputBuffer */
  class _OrxonoxExport ChatInputHandler
  {
    private:
      /** Input buffer, to be used to catch input from the
       * keyboard 
       */
      InputBuffer *inpbuf;

      /* setup input buffer, the constructor calls this */
      void configureInputBuffer();

    public:
      /** constructor */
      ChatInputHandler();

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
