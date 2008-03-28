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
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void setAmbientLight(const ColourValue& colour);

            static void setAmbientLightTest(const ColourValue& colour)
                { Ambient::instance_s->setAmbientLight(colour); }

        private:
            static Ambient* instance_s;

    };
}

#endif /* _Ambient_H__ */
