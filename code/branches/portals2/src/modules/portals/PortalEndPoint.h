/**
 *  @file PortalEndPoint.h
 *  @brief Declaration of the PortalEndPoint class.
 *  @ingroup Portals
 */

#ifndef _PortalEndPoint_H__
#define _PortalEndPoint_H__

#include "portals/PortalsPrereqs.h"

#include <set>
#include <string>
#include <map>

#include "worldentities/StaticEntity.h"
#include "graphics/Billboard.h"
#include "objects/triggers/DistanceMultiTrigger.h"
#include "core/EventIncludes.h"
#include <ctime>

namespace orxonox
{
    /**
     @brief
     A PortalEndPoint serves as portal entrance and/or exit.
     
     @ingroup Portals
     */
    
    class _PortalsExport PortalEndPoint : public StaticEntity
    {
        public:
            PortalEndPoint(BaseObject* creator);
            virtual ~PortalEndPoint();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            inline void setTarget(const std::string & target)                 //!< add types which are allowed to activate the PortalEndPoint
            {
                this->trigger_->addTarget(target);
            }
            
            void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);
            static std::map<unsigned int, PortalEndPoint *> idMap_s; //!< Maps the id of each PortalEndPoint to a pointer to that PortalEndPoint
            inline void setReenterDelay(unsigned int seconds)
            {
                this->reenterDelay_ = seconds;
            }
            inline unsigned int getReenterDelay()
            {
                return this->reenterDelay_;
            }
            inline void setID(unsigned int id)
            {
                this->id_ = id;
            }
            
            inline unsigned int getID() const
            {
                return this->id_;
            }
            
            /// \brief Set templateName_ (the name of the design Template) and add that Template to this Object
            inline void setTemplate(const std::string & name)
            {
                this->templateName_ = name;
                this->addTemplate(name);
            }

            /// \brief Get the name of the attached design template
            inline const std::string & getTemplate()
            {
                return this->templateName_;
            }

            /*! \brief This function is called each time the DistanceMultiTrigger of this PortalEndPoint changed
                \param bTriggered true if the trigger was triggered on, false if the trigger has switched to off
                \param trigger the MultiTriggerContainer containing the triggering BaseObject (and trigger_ the portal's MultiDistanceTrigger which we already know)
            */
            bool execute(bool bTriggered, BaseObject* trigger);

            /*! \brief Let an Entity jump out of this portal no matter where it was before
             * \param entity The Entity which should jump out of this portal */
            void jumpOut(MobileEntity * entity);
            
            /** \brief Tells wether a certain Entity is allowed to enter the PortalEndPoint?
                @return @c true if the entity not just came out of this portal and the reenterDelay has expired for it, @c false otherwise
            */
            bool letsEnter(MobileEntity* entity);
        protected:
            
        private:
            static const std::string EVENTFUNCTIONNAME; //!< = "execute"
            
            unsigned int id_;            //!< the hopefully (depends on the writer of the levelfile) unique id, which is used to establish links between PortalEndPoints
            DistanceMultiTrigger * trigger_;      //!< the DistanceMultiTrigger which notices near entities of the defined type
            std::string templateName_;            //!< The name of the design template used for this endpoint

            int reenterDelay_;
            std::map<MobileEntity *, time_t> jumpOutTimes_;   //!< Stores the time at which a certain MobileEntity @ref jumpOut "jumped out" of this PortalEndPoint
            std::set<MobileEntity *> recentlyJumpedOut_;
    };

}

#endif /* _Portals_H__ */
