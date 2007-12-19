#ifndef _Model_H__
#define _Model_H__

#include "WorldEntity.h"
#include "Mesh.h"
#include "../../tinyxml/tinyxml.h"

namespace orxonox
{
    class Model : public WorldEntity
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
}

#endif
