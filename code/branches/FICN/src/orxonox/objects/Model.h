#ifndef _Model_H__
#define _Model_H__

#include "../OrxonoxPrereqs.h"

#include "WorldEntity.h"
#include "Mesh.h"

class TiXmlElement; // Forward declaration

namespace orxonox
{

    class _OrxonoxExport Model : public WorldEntity
    {
        public:
            Model();
            virtual ~Model();
            virtual void loadParams(TiXmlElement* xmlElem);
            bool create();

        private:
            std::string meshSrc_;
            Mesh mesh_;
            void registerAllVariables();
    };
    ExportClass(Model, Orxonox);
}

#endif /* _Model_H__ */
