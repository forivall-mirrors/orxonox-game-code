#ifndef _PortalEndPoint_H__
#define _PortalEndPoint_H__

#include <set>
#include <string>
#include <map>

#include "StaticEntity.h"
#include "graphics/Billboard.h"
#include "objects/triggers/DistanceMultiTrigger.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox
{
    class _OrxonoxExport PortalEndPoint : public StaticEntity
    {
        public:
            PortalEndPoint(BaseObject* creator);
            virtual ~PortalEndPoint();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            //virtual void tick(float dt);
            static std::map<unsigned int, PortalEndPoint *> idMap_s;
            inline void setID(unsigned int id)
            {
                this->id_ = id;
            }
            
            inline unsigned int getID() const
            {
                return this->id_;
            }
        protected:
        private:
            unsigned int id_;
            std::set<WorldEntity *> recentlyJumpedOut_;
            std::string material_;
            Billboard billboard_;
            DistanceMultiTrigger trigger_;
    };

}

#endif /* _Portals_H__ */
