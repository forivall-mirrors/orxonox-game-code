#ifndef _Ambient_H__
#define _Ambient_H__

#include "../OrxonoxPrereqs.h"

#include "core/BaseObject.h"

namespace orxonox
{
    class _OrxonoxExport Ambient : public BaseObject
    {
        public:
            Ambient();
            virtual ~Ambient();

            
            void loadParams(TiXmlElement* xmlElem);

        private:
            

    };
    ExportClass(Ambient, Orxonox);
}

#endif /* _Ambient_H__ */
