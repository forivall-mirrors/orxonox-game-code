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
#include "util/StringUtils.h"

#include "core/BaseObject.h"
#include <iostream>
#include <fstream>
#include "core/OrxonoxClass.h"

namespace orxonox // tolua_export
{ // tolua_export

    /**
    @brief
        The LevelInfoItem class stores information regarding a @ref orxonox::Level "Level" and makes that information accessible through the @ref orxonox::LevelManager "LevelManager".
        A LevelInfoItem object is commonly created from a @ref orxonox::LevelInfo "LevelInfo" object, using its <code>copy()</code> method.

    @author
        Damian 'Mozork' Frick

    @ingroup Orxonox
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
            inline const std::string& getName(void) const { return this->name_; } // tolua_export

            /**
            @brief Set the screenshot of the Level.
            @param screenshot The screenshot to be set.
            */
            inline void setScreenshot(const std::string& screenshot) { this->screenshot_ = std::string(screenshot); }
            /**
            @brief Get the screenshot of the Level.
            @return Returns the screenshot of the Level.
            */
            inline const std::string& getScreenshot() const { return this->screenshot_; } // tolua_export

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

            void setShips(const std::string& ships); //!< Set the starting ship models of the level
            bool addShip(const std::string& ship, bool update = true); //!< Add a model to shipselection
            /**
            @brief Get the set of starting ship models the Level allows
            @return Returns a comma-seperated string of all the allowed ship models for the shipselection.
            */
            inline const std::string& getShips(void) const
                { return this->startingShipsString_; }
            /**
            @brief Get whether the Level allows a specific starting ship model
            @param ship The ship model for which is checked.
            @return Returns true if the Level allows the input ship model
            */
            inline bool hasShip(const std::string& ship) const { return this->ships_.find(ship) != this->ships_.end(); } // tolua_export
            /**
            @brief Get the XML-filename of the Level.
            @return Returns the XML-filename (including *.oxw extension) of the Level.
            */

            inline const std::string& getXMLFilename(void) const { return this->xmlfilename_; } // tolua_export
            inline void selectShip (const std::string& ship) { this->changeShip(ship); } // tolua_export


        protected:
            /**
            @brief Set the XML-filename of the Level.
            @param filename The XML-filename to be set.
            */
            inline void setXMLFilename(const std::string& filename)
                { this->xmlfilename_ = std::string(filename); }

            std::string xmlfilename_; //!< The XML-filename of the Level.

        private:

            inline void changeShip (const std::string& model) {
                static std::string shipSelectionTag = "shipselection";
                //HACK: Read Level XML File, find "shipselection", replace with ship model
                std::string levelPath = "../levels/";
                levelPath.append(this->getXMLFilename());
                std::string tempPath = "../levels/";
                tempPath.append("_temp.oxw");
                orxout(user_status) << levelPath << endl;
                orxout(user_status) << tempPath << endl;
                std::ifstream myLevel (levelPath.c_str());
                std::ofstream tempLevel (tempPath.c_str());
                while(!myLevel.eof())
                {
                    std::string buff;
                    std::getline(myLevel, buff);
                    std::string pawndesignString = "pawndesign=";
                    size_t found = buff.find(pawndesignString.append(shipSelectionTag));
                    if (found!= std::string::npos)
                        buff = buff.substr(0, found + 11) + model + buff.substr(found+11+shipSelectionTag.length(), std::string::npos);
                    tempLevel.write(buff.c_str(), buff.length());
                    tempLevel << std::endl;
                }
                myLevel.close();
                tempLevel.close();
                orxout(user_status) << "done" << endl;
            }
            void tagsUpdated(void); //!< Updates the comma-seperated string of all tags, if the set of tags has changed.
            void shipsUpdated(void); //!< Updates the comma-seperated string of all tags, if the set of tags has changed.
            static void initializeTags(void); //!< Initialize the set of allowed tags.
            /**
            @brief Check whether an input tag is allowed.
            @param tag The tag to check.
            @return Returns true if the input tag is allowed, false if not.
            */
            static bool validateTag(const std::string& tag)
                { LevelInfoItem::initializeTags(); return LevelInfoItem::possibleTags_s.find(tag) != LevelInfoItem::possibleTags_s.end(); }

            static std::set<std::string> possibleTags_s; //!< The set of allowed tags.
            static const bool initialized_s = false; //!< Whether the set of allowed tags has been inizialized.

            std::string name_; //!< The name of the Level.
            std::string description_; //!< The description of the Level.
            std::string screenshot_; //!< The screenshot of the Level.
            std::set<std::string> tags_; //!< The set of tags the Level is tagged with.
            std::string tagsString_; //!< The comma-seperated string of all the tags the Level is tagged with.
            std::set<std::string> ships_; //!< The set of starting ship models the Level allows.
            std::string startingShipsString_; //!< The comma-seperated string of all the allowed ship models for the shipselection.
    }; // tolua_export

    /**
    @brief
        The LevelInfo class can be used to store information regarding a @ref orxonox::Level "Level" in its level file.
        The following parameters can be specified:
        - @b name The name of the level.
        - @b description The description of the level.
        - @b screenshot The screenshot of the level.
        - @b tags A comma-seperated string of tags. Allowed tags are: <em>test</em>, <em>singleplayer</em>, <em>multiplayer</em>, <em>showcase</em>, <em>tutorial</em>, <em>presentation</em>, <em>shipselection</em>.
        - @b (optional) startingships The comma-seperated string of starting ship models
        An example would be:
        @code
        <LevelInfo
            name = "Levelname"lhs->compare(rhs) < 0
            description = "This is just some awesome level."
            screenshot = "Screenshot.png"
            tags = "test, awesome"
        />
        @endcode
        The LevelInfo is best located at the top of the level file.

    @author
        Damian 'Mozork' Frick
    @edit
        Matthias Hutter
    @ingroup Orxonox
    */
    class _OrxonoxExport LevelInfo : public BaseObject, public LevelInfoItem
    {
        public:
            LevelInfo(BaseObject* creator);
            virtual ~LevelInfo();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Creates a LevelInfo object through XML.

            /**
            @brief Set the screenshot of the Level.
            @param screenshot The screenshot to be set.
            */
            inline void setScreenshot(const std::string& screenshot) { this->LevelInfoItem::setScreenshot(screenshot); }
            /**
            @brief Get the screenshot of the Level.
            @return Returns the screenshot of the Level.
            */
            inline const std::string& getScreenshot() const { return this->LevelInfoItem::getScreenshot(); }

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
            /**
            @brief Set the starting ship models of the level
            @param A comma-seperated string of all the allowed ship models for the shipselection.
            */
            inline void setShips(const std::string& ships)
                { this->LevelInfoItem::setShips(ships); }
            /**
            @brief Get the starting ship models of the level
            @return Returns a comma-seperated string of all the allowed ship models for the shipselection.
            */
            inline const std::string& getShips(void) const
                { return this->LevelInfoItem::getShips(); }
            LevelInfoItem* copy(void); //!< Copies the contents of this LevelInfo object to a new LevelInfoItem object.
    };

    /**
    @brief
        Struct that overloads the compare operation between two @ref orxonox::LevelInfoItem "LevelInfoItem" pointers.

    @ingroup Orxonox
    */
    struct LevelInfoCompare
    {
        bool operator() (const LevelInfoItem* lhs, const LevelInfoItem* rhs) const
            {
                if(getLowercase(lhs->getName()).compare(getLowercase(rhs->getName())) == 0)
                    return getLowercase(lhs->getXMLFilename()).compare(getLowercase(rhs->getXMLFilename())) < 0;
                return getLowercase(lhs->getName()).compare(getLowercase(rhs->getName())) < 0;
            }
    };

} // tolua_export

#endif /* _LevelInfo_H__ */
