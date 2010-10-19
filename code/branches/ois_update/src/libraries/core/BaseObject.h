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
    @defgroup BaseObject BaseObject
    @ingroup Core
*/

/**
    @file
    @ingroup BaseObject
    @brief Declaration of BaseObject, the base class of all objects in Orxonox.

    The BaseObject is the parent of all classes representing an instance in the game.
*/

#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "CorePrereqs.h"

#include <map>
#include <list>

#include "util/mbool.h"
#include "OrxonoxClass.h"
#include "Super.h"
#include "SmartPtr.h"

namespace orxonox
{
    class Scene;
    class Gametype;
    class Level;

    /// The BaseObject is the parent of all classes representing an instance in the game.
    class _CoreExport BaseObject : virtual public OrxonoxClass
    {
        template <class T> friend class XMLPortClassParamContainer;

        public:
            BaseObject(BaseObject* creator);
            virtual ~BaseObject();
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);

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
                if (this->bActive_ != bActive)
                {
                    this->bActive_ = bActive;
                    this->changedActivity();
                }
            }
            /** @brief Returns the state of the objects activity. @return The state of the activity */
            inline const mbool& isActive() const { return this->bActive_; }
            /** @brief This function gets called if the activity of the object changes. */
            virtual void changedActivity() {}

            /** @brief Sets the state of the objects visibility. @param bVisible True = visible */
            inline void setVisible(bool bVisible)
            {
                if (this->bVisible_ != bVisible)
                {
                    this->bVisible_ = bVisible;
                    this->changedVisibility();
                }
            }
            /** @brief Returns the state of the objects visibility. @return The state of the visibility */
            inline const mbool& isVisible() const { return this->bVisible_; }
            /** @brief This function gets called if the visibility of the object changes. */
            virtual void changedVisibility() {}

            void setMainState(bool state);

            /** @brief Sets the name of the main state (used for event reactions). */
            void setMainStateName(const std::string& name)
            {
                if (this->mainStateName_ != name)
                {
                    this->mainStateName_ = name;
                    this->changedMainStateName();
                }
            }
            /** @brief Returns the name of the main state. */
            inline const std::string& getMainStateName() const { return this->mainStateName_; }
            /** @brief This function gets called if the main state name of the object changes. */
            virtual void changedMainStateName();

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

            inline void setScene(const SmartPtr<Scene>& scene, uint32_t sceneID) { this->scene_ = scene; this->sceneID_=sceneID; }
            inline const SmartPtr<Scene>& getScene() const { return this->scene_; }
            inline virtual uint32_t getSceneID() const { return this->sceneID_; }

            inline void setGametype(const SmartPtr<Gametype>& gametype)
            {
                if (gametype != this->gametype_)
                {
                    this->oldGametype_ = this->gametype_;
                    this->gametype_ = gametype;
                    this->changedGametype();
                }
            }
            inline const SmartPtr<Gametype>& getGametype() const { return this->gametype_; }
            inline Gametype* getOldGametype() const { return this->oldGametype_; }
            virtual void changedGametype() {}

            inline void setLevel(const SmartPtr<Level>& level)
            {
                if (level != this->level_)
                {
                    this->level_ = level;
                    this->changedLevel();
                }
            }
            inline const SmartPtr<Level>& getLevel() const { return this->level_; }
            virtual void changedLevel() {}

            void addEventSource(BaseObject* source, const std::string& state);
            void removeEventSource(BaseObject* source);
            BaseObject* getEventSource(unsigned int index, const std::string& state) const;

            void addEventListener(BaseObject* listener);
            BaseObject* getEventListener(unsigned int index) const;

            void fireEvent(const std::string& name = "");
            void fireEvent(bool activate, const std::string& name = "");
            void fireEvent(bool activate, BaseObject* originator, const std::string& name = "");
            void fireEvent(Event& event);

            virtual void processEvent(Event& event);

            /** @brief Sets the indentation of the debug output in the Loader. @param indentation The indentation */
            inline void setLoaderIndentation(const std::string& indentation) { this->loaderIndentation_ = indentation; }
            /** @brief Returns the indentation of the debug output in the Loader. @return The indentation */
            inline const std::string& getLoaderIndentation() const { return this->loaderIndentation_; }

            static void loadAllEventStates(Element& xmlelement, XMLPort::Mode mode, BaseObject* object, Identifier* identifier);

        protected:
            void addEventState(const std::string& name, EventState* container);
            EventState* getEventState(const std::string& name) const;

            std::string             name_;                     //!< The name of the object
            std::string             oldName_;                  //!< The old name of the object
            mbool                   bActive_;                  //!< True = the object is active
            mbool                   bVisible_;                 //!< True = the object is visible
            std::string             mainStateName_;
            FunctorPtr              mainStateFunctor_;
            std::set<std::string>   networkTemplateNames_;

        private:
            /** @brief Adds an object which listens to the events of this object. */
            void registerEventListener(BaseObject* object);
            /** @brief Removes an event listener from this object. */
            inline void unregisterEventListener(BaseObject* object)
                { this->eventListeners_.erase(object); }

            void setXMLName(const std::string& name);
            const std::string& getSingleTemplate(void) const;
            Template* getTemplate(unsigned int index) const;
            void registerEventStates();

            bool                   bInitialized_;              //!< True if the object was initialized (passed the object registration)
            const XMLFile*         file_;                      //!< The XMLFile that loaded this object
            Element*               lastLoadedXMLElement_;      //!< Non 0 if the TinyXML attributes have already been copied to our own lowercase map
            std::map<std::string, std::string> xmlAttributes_; //!< Lowercase XML attributes
            std::string            loaderIndentation_;         //!< Indentation of the debug output in the Loader
            Namespace*             namespace_;
            BaseObject*            creator_;
            SmartPtr<Scene>        scene_;
            uint32_t               sceneID_;
            SmartPtr<Gametype>     gametype_;
            Gametype*              oldGametype_;
            SmartPtr<Level>        level_;
            std::set<Template*>    templates_;

            std::map<BaseObject*, std::string>  eventSources_;           //!< List of objects which send events to this object, mapped to the state which they affect
            std::set<BaseObject*>               eventListeners_;         //!< List of objects which listen to the events of this object
            std::set<BaseObject*>               eventListenersXML_;      //!< List of objects which listen to the events of this object through the "eventlisteners" subsection in XML
            std::map<std::string, EventState*>  eventStates_;            //!< Maps the name of the event states to their helper objects
            bool                                bRegisteredEventStates_; //!< Becomes true after the object registered its event states (with XMLEventPort)
    };

    SUPER_FUNCTION(0, BaseObject, XMLPort, false);
    SUPER_FUNCTION(2, BaseObject, changedActivity, false);
    SUPER_FUNCTION(3, BaseObject, changedVisibility, false);
    SUPER_FUNCTION(4, BaseObject, XMLEventPort, false);
    SUPER_FUNCTION(8, BaseObject, changedName, false);
    SUPER_FUNCTION(9, BaseObject, changedGametype, false);
}

#endif /* _BaseObject_H__ */
