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
 *      Oliver Scheuss <scheusso [at] ee.ethz.ch>, (C) 2008
 *   Co-authors:
 *      ...
 *
 */
#ifndef NETWORKBYTESTREAM_H
#define NETWORKBYTESTREAM_H

#define DATA_TYPE unsigned char

#include <string>
  
/**
	@author Oliver Scheuss <scheusso [at] ee.ethz.ch>
*/
class Bytestream{
  public:
    Bytestream();
    Bytestream( unsigned int size );
    Bytestream( DATA_TYPE *data, unsigned int size, bool empty=false );
    ~Bytestream();
    
    template <class T> Bytestream& operator<< ( T& data )
        { write(&data, sizeof(T)); return *this; }
    template <class T> Bytestream& operator>> ( T& data )
        { read(&data, sizeof(T)); return *this; }
    void write(void *data, unsigned int length);
    void read(void *data, unsigned int length);
    void readString(std::string& s);
    
    // specialisations
    template <std::string&> Bytestream& operator<< ( std::string& s ){ write( s.c_str(), s.length()+1); return *this; }
    template <std::string&> Bytestream& operator>> ( std::string& s ){ readString(s); return *this; }
    
    DATA_TYPE *getData(){ return data_; }
    unsigned int getSize(){ return writeIterator_; }
    
  private:
    

// Membervariables
  
  private:
    DATA_TYPE *data_;
    unsigned int writeIterator_;
    unsigned int readIterator_;
    unsigned int maxSize_;
};


#endif
