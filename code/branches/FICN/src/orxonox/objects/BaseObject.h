/*!
    @file BaseObject.h
    @brief Definition of the BaseObject class.

    The BaseObject is the parent of all classes representing an instance in the game.
*/

#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "../core/CoreIncludes.h"
#include "tinyxml/tinyxml.h"

namespace orxonox
{
    //! The BaseObject is the parent of all classes representing an instance in the game.
    class BaseObject : virtual public OrxonoxClass
    {
        public:
            BaseObject();
            virtual ~BaseObject();
            virtual void loadParams(TiXmlElement* xmlElem) {}
    };
}

#endif
