#ifndef _Model_H__
#define _Model_H__

#include "../OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "../tools/Mesh.h"

class TiXmlElement; // Forward declaration

namespace orxonox
{

    class _OrxonoxExport Model : public WorldEntity
    {
        public:
            Model();
            virtual ~Model();
            virtual void loadParams(TiXmlElement* xmlElem);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void setMesh(const std::string& meshname);
            bool create();

        private:
            std::string meshSrc_;
            Mesh mesh_;
            void registerAllVariables();
    };
}

#endif /* _Model_H__ */
