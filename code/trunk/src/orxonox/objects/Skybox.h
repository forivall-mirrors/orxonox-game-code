#ifndef _Skybox_H__
#define _Skybox_H__

#include "../OrxonoxPrereqs.h"

#include "core/BaseObject.h"

class TiXmlElement; // Forward declaration

namespace orxonox
{
    class _OrxonoxExport Skybox : public BaseObject
    {
        public:
            Skybox();
            virtual ~Skybox();

            void loadParams(TiXmlElement* xmlElem);
            virtual void XMLPort(Element& xmlelement, bool loading);
            void setSkybox(const std::string& skyboxname);

        private:


    };
}

#endif /* _Skybox_H__ */
