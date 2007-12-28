#ifndef _Camera_H__
#define _Camera_H__

#include "../OrxonoxPrereqs.h"

#include "BaseObject.h"

namespace orxonox
{
    class Camera : public BaseObject
    {
        public:
            Camera();
            virtual ~Camera();

            
            void loadParams(TiXmlElement* xmlElem);

        private:
            

    };
}

#endif /* _Camera_H__ */
