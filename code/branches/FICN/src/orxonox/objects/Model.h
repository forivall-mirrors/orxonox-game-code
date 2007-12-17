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
            ~Model();
            virtual void loadParams(TiXmlElement* xmlElem);

        private:
            Mesh mesh_;
    };
}

#endif
