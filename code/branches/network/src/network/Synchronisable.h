//
// C++ Interface: synchronisable
//
// Description: 
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NETWORKSYNCHRONISABLE_H
#define NETWORKSYNCHRONISABLE_H

namespace network {

/**
 * This class is the base class of all the Objects in the universe that need to be synchronised over the network
 * Every class, that inherits from this class has to link the DATA THAT NEEDS TO BE SYNCHRONISED into the linked list. Additionally it also has to provide a Constructor, that takes exactly the variables in this linked list.
 * @author Oliver Scheuss
*/
class Synchronisable{
public:
    Synchronisable();

    ~Synchronisable();

};

}

#endif
