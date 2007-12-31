/*!
    @file BaseObject.h
    @brief Definition of the BaseObject class.

    The BaseObject is the parent of all classes representing an instance in the game.
*/

#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "CorePrereqs.h"
#include "util/tinyxml/TinyXMLPrereqs.h"

#include "CoreIncludes.h"

namespace orxonox
{
    //! The BaseObject is the parent of all classes representing an instance in the game.
    class _CoreExport BaseObject : virtual public OrxonoxClass
    {
        public:
            BaseObject();
            virtual ~BaseObject();
            virtual void loadParams(TiXmlElement* xmlElem);
    };
    ExportClass(BaseObject, Core);
}

#endif /* _BaseObject_H__ */
