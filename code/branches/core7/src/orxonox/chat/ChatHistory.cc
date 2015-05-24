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

#include "ChatHistory.h"
#include "core/singleton/ScopedSingletonIncludes.h"

#ifndef CHATTEST
namespace orxonox
{
  /* singleton */
  ManageScopedSingleton( ChatHistory, ScopeID::ROOT, false );

  RegisterAbstractClass(ChatHistory).inheritsFrom<ChatListener>();

#endif

  /* constructor */
#ifndef CHATTEST
  ChatHistory::ChatHistory()
#else
  ChatHistory::ChatHistory()
#endif
  {
    /* register the object */
#ifndef CHATTEST
    RegisterObject(ChatHistory);
#endif

    this->hist_log_enabled = true;

    /* Read setting for logfiles */
    if( hist_log_enabled ) /* NOTE Make this a check for the logfile setting */
    { this->chat_hist_openlog();

      /* push starting line */
      this->chat_hist_logline( "--- Logfile opened ---" );
    }

    /* Read setting for maximum number of lines and set limit */
    this->hist_maxlines = 200; /* NOTE to be changed, 200 is just for testing */
  }

  /* destructor */
  ChatHistory::~ChatHistory()
  {
    chat_hist_closelog();

    /* clear list */
    this->hist_buffer.clear();
  }

  /* react to incoming chat */
  void ChatHistory::incomingChat(const std::string& message, const std::string& /*name*/)
  {
    /* add the line to the history */
    this->chat_hist_addline( message );

    /* add the line to the log */
    this->chat_hist_logline( message );
  }

  /* Synchronize logfile onto the hard drive */ /* MARK MARK */
  int ChatHistory::syncLog()
  {
    //if( this->hist_logfile )
      //this->hist_logfile.sync();
    return 0;
  }

  /* add a line to this history */
  int ChatHistory::chat_hist_addline( const std::string& toadd )
  {
    /* crop history at the end if it's too large */
    while( this->hist_buffer.size() > this->hist_maxlines+1 )
      this->hist_buffer.pop_front();

    /* push to the front of the history */
    this->hist_buffer.push_back( toadd );
    return 0;
  }

  /* log a line to a logfile */
  int ChatHistory::chat_hist_logline( const std::string& toadd )
  {
    /* output the line to the file if logging is enabled */
    if( this->hist_log_enabled )
      this->hist_logfile << toadd << endl;
    return 0;
  }

  /* open logfile */
  int ChatHistory::chat_hist_openlog()
  {
    /* TODO: find out the name of the file to log to via settings
     *       and set the this->hist_logfile_path variable to it
     */
#ifndef CHATTEST
    this->hist_logfile.open( (PathConfig::getInstance().getLogPathString() +
      "chatlog.log").c_str(),
      std::fstream::out | std::fstream::app );
#else
    this->hist_logfile.open( "/tmp/chatlog.log",
      std::fstream::out | std::fstream::app );
#endif

    /* TODO check whether this works (not sure how you'd like it?) */
    if( !this->hist_logfile )
    { this->hist_log_enabled = false;
#ifndef CHATTEST
      orxout(internal_warning) << "Could not open logfile." << endl;
#endif
    }

    /* if it worked */
    return 0;
  }

  /* close logfile */
  void ChatHistory::chat_hist_closelog()
  {
    /* see if we've actually got a logfile */
    if( this->hist_logfile )
    {
      /* yes, we've got one, add a line that shows we're closing it */
      this->chat_hist_logline( "--- Logfile closed ---" );

      /* actually close down the file */
      this->hist_logfile.close();
    }
  }

  /* output history for debugging */
  void ChatHistory::debug_printhist()
  {
    /* create deque iterator */
    std::deque<std::string>::iterator it;

    /* output all the strings */
    for( it = this->hist_buffer.begin(); it != this->hist_buffer.end();
      ++it )
      orxout(debug_output) << *it << endl;

    /* output size */
    orxout(debug_output) << "Size: " << hist_buffer.size() << endl;
  }

#ifndef CHATTEST
}
#endif
