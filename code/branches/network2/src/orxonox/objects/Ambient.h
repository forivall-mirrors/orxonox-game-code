#ifndef _Ambient_H__
#define _Ambient_H__

#include "../OrxonoxPrereqs.h"

#include "core/BaseObject.h"
#include "network/Synchronisable.h"

namespace orxonox
{
    class _OrxonoxExport Ambient : public BaseObject, network::Synchronisable
    {
        public:
            Ambient();
            virtual ~Ambient();

            void loadParams(TiXmlElement* xmlElem);
            virtual void XMLPort(Element& xmlelement, bool loading);
            void setAmbientLight(const ColourValue& colour);
            bool create();
            void registerAllVariables();

        private:
            ColourValue ambientLight_;

    };
}

#endif /* _Ambient_H__ */
