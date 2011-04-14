#ifndef _PortalLink_H__
#define _PortalLink_H__

#include "portals/PortalsPrereqs.h"
#include "tools/interfaces/Tickable.h"
#include "core/BaseObject.h"
#include "PortalEndPoint.h"
#include "objects/eventsystem/EventListener.h"

#include <map>

namespace orxonox
{
    class _PortalsExport PortalLink : public BaseObject
    {
        public:
            PortalLink(BaseObject* creator);
            virtual ~PortalLink();
    
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);
            inline void setFromID(unsigned int from)
            {
                this->fromID_ = from;
            }
            inline unsigned int getFromID(unsigned int) const
            {
                return this->fromID_;
            }
            inline void setToID(unsigned int to)
            {
                this->toID_ = to;
            }
            inline unsigned int getToID(unsigned int) const
            {
                return this->toID_;
            }
            static void use(MobileEntity * entity, PortalEndPoint * entrance);
        protected:
        private:
            static std::map<PortalEndPoint *, PortalEndPoint *> links_s;
            unsigned int fromID_;
            unsigned int toID_;
            PortalEndPoint* from_;
            PortalEndPoint* to_;
            float activationRadius_;
            bool isNowPortable(WorldEntity * ent);
    };

}

#endif /* _Portals_H__ */
