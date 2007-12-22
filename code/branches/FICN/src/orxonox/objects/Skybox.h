#ifndef _Skybox_H__
#define _Skybox_H__

#include "BaseObject.h"
#include "../../tinyxml/tinyxml.h"

namespace orxonox
{
    class Skybox : public BaseObject
    {
        public:
            Skybox();
            virtual ~Skybox();

            
            void loadParams(TiXmlElement* xmlElem);

        private:
            

    };
}

#endif /* _Skybox_H__ */
