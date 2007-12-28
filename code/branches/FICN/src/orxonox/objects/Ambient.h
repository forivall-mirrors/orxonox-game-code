#ifndef _Ambient_H__
#define _Ambient_H__

#include "../OrxonoxPrereqs.h"

#include "BaseObject.h"

namespace orxonox
{
    class Ambient : public BaseObject
    {
        public:
            Ambient();
            virtual ~Ambient();

            
            void loadParams(TiXmlElement* xmlElem);

        private:
            

    };
}

#endif /* _Ambient_H__ */
