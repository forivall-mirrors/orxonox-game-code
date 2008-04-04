#ifndef _Model_H__
#define _Model_H__

#include "../OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "../tools/Mesh.h"

namespace orxonox
{

    class _OrxonoxExport Model : public WorldEntity
    {
        public:
            Model();
            virtual ~Model();
            virtual void loadParams(TiXmlElement* xmlElem);
            virtual void XMLPort(Element& xmlelement, bool loading);
            void setMesh(const std::string& meshname);
            bool create();

        protected:
            void registerAllVariables();
            
        private:
            std::string meshSrc_;
            Mesh mesh_;
    };
}

#endif /* _Model_H__ */
