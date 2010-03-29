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

#include <ChatHistory.h>

namespace orxonox
{
  /* is this necessary? if yes uncomment please :P */
  //CreateFactory(ChatHistory);

  /* constructor */
  ChatHistory(BaseObject* creator) : BaseObject(creator) 
  {
    /* register the object */
    RegisterObject(ChatHistory);

    /* Read setting for logfiles */
    if( true ) /* NOTE Make this a check for the logfile setting */
      this->chat_hist_openlog();

    /* Read setting for maximum number of lines and set limit */
    this->hist_maxlines = 200; /* NOTE to be changed, 200 is just for testing */

    /* push starting line */
    this->hist_buffer.push_front( "--- Logfile opened ---" )
  }

  /* destructor */
  virtual ChatHistory::~ChatHistory()
  {
    /* check if loggin is enabled */
      /* y -> synchronize the logfile */
  }

  /* react to incoming chat */
  virtual void ChatHistory::incomingChat(const std::string& message, 
    unsigned int senderID)
  {
    /* sanity - check for valid senderID */
    /* sanity - check for valid string format */ 

    /* format the message and senderID into a line */
    std::string buf = "empty"; /* NOTE to be changed */

    /* --> a) look up the actual name of the sender */
    /* --> b) add sender name and string up with a separator
     *    to make up the actual message
     */

    /* add the line to the history */
    this->chat_hist_addline( buf );

    /* add the line to the log */
    this->chat_hist_logline( buf );
  } 

  /* Synchronize logfile onto the hard drive */ /* MARK MARK */
  int ChatHistory::syncLog();

  /* add a line to this history */
  int ChatHistory::chat_hist_addline( const std::string& toadd );
  {
    /* push to the front of the history */
    this->hist_buffer.push_front( toadd );

    /* crop history at the end if it's too large */
    this->hist_buffer.resize( this->hist_maxlines );
  }

  /* log a line to a logfile */
  int ChatHistory::chat_hist_logline( const std::string& toadd )
  { 
    /* TODO use already written class to batch write to file
     * so not every line gets written on its own 
     */
    /* output the line to the file if logging is enabled */
    if( this->hist_log_enabled )
      this->hist_logfile << buf << std::endl;
  }

  /* open logfile */
  int ChatHistory::chat_hist_openlog()
  {
    /* TODO: find out the name of the file to log to via settings 
     *       and set the this->hist_logfile_path variable to it
     */
    this->hist_logfile.open( "/tmp/setsomepath.txt", 
      fstream::out | fstream::app );

    /* TODO check whether this works (not sure how you'd like it?) */

    /* if it worked */
    return 0;
  }

}
