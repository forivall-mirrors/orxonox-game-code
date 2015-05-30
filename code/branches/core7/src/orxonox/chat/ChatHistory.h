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

#include <deque>
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

/* define this if you're unit testing */
//#define CHATTEST

#ifndef CHATTEST
#include "OrxonoxPrereqs.h"

#include "util/Singleton.h"
#include "core/BaseObject.h"
#include "chat/ChatListener.h"
#include "infos/PlayerInfo.h"
#include "PlayerManager.h"
#endif

#ifndef _ChatHistory_H__
#define _ChatHistory_H__


/* Class to implement chat history */
#ifndef CHATTEST
namespace orxonox
{
#endif

  /* constructor */
#ifndef CHATTEST
  class _OrxonoxExport ChatHistory : public ChatListener,
    public Singleton<ChatHistory>

#else
  class ChatHistory
#endif
  {
    public:
      /* constructors, destructors */
#ifndef CHATTEST
      ChatHistory();
      friend class Singleton<ChatHistory>;
#else
      ChatHistory();
#endif
      virtual ~ChatHistory();


    //protected:
      /** what to do with incoming chat
       *
       * \param message The incoming message
       * \param senderID Identification number of the sender
       */
      virtual void incomingChat(const std::string& message, const std::string& name);

      /** Synchronize logfile onto the hard drive
       *
       * \return 0 for success, other for error
       */
      int syncLog();

      /** debug-print: output the whole history to stdout */
      void debug_printhist();

    private:
      /* FIELDS */
      /** Vector to store the history in */
      std::deque<std::string> hist_buffer;

      /** Maximum number of lines stored in this history */
      unsigned int hist_maxlines;

      /** is logging enabled? */
      bool hist_log_enabled;

      /** path of logfile on the file system */
      std::string hist_logfile_path;

      /** Output file stream for logfile */
      std::ofstream hist_logfile;

#ifndef CHATTEST
      static ChatHistory* singletonPtr_s;
#endif



      /* METHODS */
      /** Append line to chat history
       *
       * \param toadd The line to add to the history
       * \return 0 for success, other for error TODO: Throw exception
       */
      int chat_hist_addline( const std::string& toadd );

      /** Append line to logfile
       *
       * \param toadd The line to add to the logfile
       * \return 0 for success, other for error TODO: Throw exception
       */
      int chat_hist_logline( const std::string& toadd );

      /** open logfile to log to
       *
       * \return 0 for success,s other for error
       */
      int chat_hist_openlog();


      /** close logfile */
      void chat_hist_closelog();
  };

#ifndef CHATTEST
}
#endif

#endif /* _ChatHistory_H__ */
