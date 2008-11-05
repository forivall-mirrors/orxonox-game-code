/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Definition of the BaseObject class.

    The BaseObject is the parent of all classes representing an instance in the game.
*/

#ifndef _BaseObject_H__
#define _BaseObject_H__

#include <map>

#include "CorePrereqs.h"

#include "Super.h"
#include "OrxonoxClass.h"
#include "XMLIncludes.h"
#include "Event.h"

namespace orxonox
{
    class Scene;
    class Gametype;

    //! The BaseObject is the parent of all classes representing an instance in the game.
    class _CoreExport BaseObject : virtual public OrxonoxClass
    {
        public:
            BaseObject(BaseObject* creator);
            virtual ~BaseObject();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            /** @brief Returns if the object was initialized (passed the object registration). @return True was the object is initialized */
            inline bool isInitialized() const { return this->bInitialized_; }

            /** @brief Sets the name of the object. @param name The name */
            inline void setName(const std::string& name) { this->oldName_ = this->name_; this->name_ = name; this->changedName(); }
            /** @brief Returns the name of the object. */
            inline const std::string& getName() const { return this->name_; }
            /** @brief Returns the old name of the object. */
            inline const std::string& getOldName() const { return this->oldName_; }
            /** @brief This function gets called if the name of the object changes. */
            virtual void changedName() {}

            /** @brief Sets the state of the objects activity. @param bActive True = active */
            inline void setActive(bool bActive)
            { 
                bool bTemp = this->bActive_;
                this->bActive_ = bActive;
                if ( bTemp != bActive )
                    this->changedActivity();
            }
            /** @brief Returns the state of the objects activity. @return The state of the activity */
            inline bool isActive() const { return this->bActive_; }
            /** @brief This function gets called if the activity of the object changes. */
            virtual void changedActivity() {}

            /** @brief Sets the state of the objects visibility. @param bVisible True = visible */
            inline void setVisible(bool bVisible) { this->bVisible_ = bVisible; this->changedVisibility(); }
            /** @brief Returns the state of the objects visibility. @return The state of the visibility */
            inline bool isVisible() const { return this->bVisible_; }
            /** @brief This function gets called if the visibility of the object changes. */
            virtual void changedVisibility() {}

            /** @brief Sets a pointer to the xml file that loaded this object. @param file The pointer to the XMLFile */
            inline void setFile(const XMLFile* file) { this->file_ = file; }
            /** @brief Returns a pointer to the XMLFile that loaded this object. @return The XMLFile */
            inline const XMLFile* getFile() const { return this->file_; }
            const std::string& getFilename() const;

            void addTemplate(const std::string& name);
            void addTemplate(Template* temp);
            /** @brief Returns the set of all aplied templates. */
            inline const std::set<Template*>& getTemplates() const
                { return this->templates_; }

            virtual inline void setNamespace(Namespace* ns) { this->namespace_ = ns; }
            inline Namespace* getNamespace() const { return this->namespace_; }

            inline void setCreator(BaseObject* creator) { this->creator_ = creator; }
            inline BaseObject* getCreator() const { return this->creator_; }

            inline void setScene(Scene* scene) { this->scene_ = scene; }
            inline Scene* getScene() const { return this->scene_; }

            inline void setGametype(Gametype* gametype) { this->oldGametype_ = this->gametype_; this->gametype_ = gametype; this->changedGametype(); }
            inline Gametype* getGametype() const { return this->gametype_; }
            inline Gametype* getOldGametype() const { return this->oldGametype_; }
            virtual inline void changedGametype() {}

            void fireEvent();
            void fireEvent(bool activate);
            void fireEvent(bool activate, BaseObject* originator);
            void fireEvent(Event& event);

            virtual void processEvent(Event& event);

            inline void registerEventListener(BaseObject* object, const std::string& sectionname)
                { this->eventListeners_[object] = sectionname; }
            inline void unregisterEventListener(BaseObject* object)
                { this->eventListeners_.erase(object); }

            void addEvent(BaseObject* event, const std::string& sectionname);
            void removeEvent(BaseObject* event);
            BaseObject* getEvent(unsigned int index) const;

            void addEventContainer(const std::string& sectionname, EventContainer* container);
            EventContainer* getEventContainer(const std::string& sectionname) const;

            /** @brief Sets the indentation of the debug output in the Loader. @param indentation The indentation */
            inline void setLoaderIndentation(const std::string& indentation) { this->loaderIndentation_ = indentation; }
            /** @brief Returns the indentation of the debug output in the Loader. @return The indentation */
            inline const std::string& getLoaderIndentation() const { return this->loaderIndentation_; }

        protected:
            std::string name_;                          //!< The name of the object
            std::string oldName_;                       //!< The old name of the object
            bool bActive_;                              //!< True = the object is active
            bool bVisible_;                             //!< True = the object is visible

        private:
            void setXMLName(const std::string& name);
            Template* getTemplate(unsigned int index) const;

            bool                  bInitialized_;         //!< True if the object was initialized (passed the object registration)
            const XMLFile*        file_;                 //!< The XMLFile that loaded this object
            std::string           loaderIndentation_;    //!< Indentation of the debug output in the Loader
            Namespace*            namespace_;
            BaseObject*           creator_;
            Scene*                scene_;
            Gametype*             gametype_;
            Gametype*             oldGametype_;
            std::set<Template*>   templates_;
            std::map<BaseObject*, std::string> eventListeners_;
            std::list<BaseObject*> events_;
            std::map<std::string, EventContainer*> eventContainers_;
    };

    SUPER_FUNCTION(0, BaseObject, XMLPort, false);
    SUPER_FUNCTION(2, BaseObject, changedActivity, false);
    SUPER_FUNCTION(3, BaseObject, changedVisibility, false);
    SUPER_FUNCTION(4, BaseObject, processEvent, false);
}

#endif /* _BaseObject_H__ */
