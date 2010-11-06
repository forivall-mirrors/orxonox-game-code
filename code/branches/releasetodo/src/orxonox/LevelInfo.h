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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file LevelInfo.h
    @brief Definition of the LevelInfo and LevelInfoItem class.
    @ingroup Orxonox
*/

#ifndef _LevelInfo_H__
#define _LevelInfo_H__

#include "OrxonoxPrereqs.h"

#include <set>
#include <string>

#include "core/BaseObject.h"
#include "core/OrxonoxClass.h"

namespace orxonox // tolua_export
{ // tolua_export

    /**
    @brief
        The LevelInfoItem class stores information regarding a @ref orxonox::Level "Level" and makes that information it accessible trough the @ref orxonox::LevelManager "LevelManager".
        A LevelInfoItem object is commonly created from a @ref orxonox::LevelInfo "LevelInfo" object, using its <code>copy()</code> method.

    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport LevelInfoItem // tolua_export
        : virtual public OrxonoxClass
    { // tolua_export
        public:
            LevelInfoItem(); //!< Default constructor.
            LevelInfoItem(const std::string& name, const std::string filename); //!< Constructor. Initializes the object.
            virtual ~LevelInfoItem(); //!< Destructor.

            /**
            @brief Set the name of the Level.
            @param name The name to be set.
            */
            inline void setName(const std::string& name)
                { this->name_ = std::string(name); }
            /**
            @brief Get the name of the Level.
            @return Returns the name of the Level.
            */
            inline const std::string& getName(void) { return this->name_; } // tolua_export

            /**
            @brief Set the description of the Level.
            @param description The description to be set.
            */
            inline void setDescription(const std::string& description)
                { this->description_ = std::string(description); }
            /**
            @brief Get the description of the Level.
            @return Returns the description of the Level.
            */
            inline const std::string& getDescription() const { return this->description_; } // tolua_export

            void setTags(const std::string& tags); //!< Set the tags the Level is tagged with.
            bool addTag(const std::string& tag, bool update = true); //!< Add a tag to the set of tags the Level is tagged with.
            /**
            @brief Get the lis of the tags the Level is tagged with.
            @return Returns a comma-seperated string of all the tags the Level is tagged with.
            */
            inline const std::string& getTags(void) const
                { return this->tagsString_; }
            /**
            @brief Get whether the Level has a specific tag.
            @param tag The tag for which is checked.
            @return Returns true if the Level is tagged with the input tag.
            */
            inline bool hasTag(const std::string& tag) const { return this->tags_.find(tag) != this->tags_.end(); } // tolua_export

            /**
            @brief Get the XML-filename of the Level.
            @return Returns the XML-filename (including *.oxw extension) of the Level.
            */
            inline const std::string& getXMLFilename(void) { return this->xmlfilename_; } // tolua_export

        protected:
            /**
            @brief Set the XML-filename of the Level.
            @param filename The XML-filename to be set.
            */
            inline void setXMLFilename(const std::string& filename)
                { this->xmlfilename_ = std::string(filename); }

            std::string xmlfilename_; //!< The XML-filename of the Level.

        private:
            void tagsUpdated(void); //!< Updates the comma-seperated string of all tags, if the set of tags has changed.

            std::string name_; //!< The name of the Level.
            std::string description_; //!< The description of the Level.
            std::set<std::string> tags_; //!< The set of tags the Level is tagged with.
            std::string tagsString_; //!< The comma-seperated string of all the tags the Level is tagged with.
    }; // tolua_export

    /**
    @brief
        The LevelInfo class can be used to store information regarding a @ref orxonox::Level "Level" in its level file.
        The following parameters can be specified:
        - @b name The name of the level.
        - @b description The description of the level.
        - @b tags A comma-seperated string of tags.

        An example would be:
        @code
        <LevelInfo
            name = "Levelname"
            description = "This is just some awesome level."
            tags = "test, awesome"
        />
        @endcode
        The LevelInfo is best located at the top of the level file.

    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport LevelInfo : public BaseObject, public LevelInfoItem
    {
        public:
            LevelInfo(BaseObject* creator);
            virtual ~LevelInfo();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Creates a LevelInfo object through XML.

            /**
            @brief Set the description of the Level.
            @param description The description to be set.
            */
            inline void setDescription(const std::string& description)
                { this->LevelInfoItem::setDescription(description); }
            /**
            @brief Get the description of the Level.
            @return Returns the description of the Level.
            */
            inline const std::string& getDescription() const
                { return this->LevelInfoItem::getDescription(); }

            /**
            @brief Set the tags the Level is tagged with.
            @param tags A comma-seperated string of all the tags to be set.
            */
            inline void setTags(const std::string& tags)
                { this->LevelInfoItem::setTags(tags); }
            /**
            @brief Get the lis of the tags the Level is tagged with.
            @return Returns a comma-seperated string of all the tags the Level is tagged with.
            */
            inline const std::string& getTags(void) const
                { return this->LevelInfoItem::getTags(); }

            LevelInfoItem* copy(void); //!< Copies the contents of this LevelInfo object to a new LevelInfoItem object.

    };
} // tolua_export

#endif /* _LevelInfo_H__ */
