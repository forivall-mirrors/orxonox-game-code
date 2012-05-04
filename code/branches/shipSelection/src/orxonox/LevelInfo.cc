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

#include "LevelInfo.h"

#include <sstream>
#include <vector>

#include "util/SubString.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "LevelManager.h"

namespace orxonox
{

    // LevelInfoItem
    
    //! The list of allowed tags.
    /*static*/ std::set<std::string> LevelInfoItem::possibleTags_s = std::set<std::string>();

    /**
    @brief
        Default constructor.
    */
    LevelInfoItem::LevelInfoItem()
    {

    }

    /**
    @brief
        Constructor. Initializes the object.
    @param name
        The name of the Level.
    @param filename
        The XML-filename of the Level.
    */
    LevelInfoItem::LevelInfoItem(const std::string& name, const std::string filename)
    {
        this->setName(name);
        this->setXMLFilename(filename);
    }

    /**
    @brief
        Destructor.
    */
    LevelInfoItem::~LevelInfoItem()
    {

    }

    /**
    @brief
        Initialize the set of allowed tags.
    */
    /*static*/ void LevelInfoItem::initializeTags(void)
    {
        if(!LevelInfoItem::initialized_s)
        {
            LevelInfoItem::possibleTags_s.insert("test");
            LevelInfoItem::possibleTags_s.insert("showcase");
            LevelInfoItem::possibleTags_s.insert("tutorial");
            LevelInfoItem::possibleTags_s.insert("presentation");
            LevelInfoItem::possibleTags_s.insert("mission");
            LevelInfoItem::possibleTags_s.insert("gametype");
            LevelInfoItem::possibleTags_s.insert("minigame");
            LevelInfoItem::possibleTags_s.insert("shipselection");
        }
    }

    /**
    @brief
        Set the tags the Level is tagged with.
    @param tags
        A comma-seperated string of all the tags to be set.
    */
    void LevelInfoItem::setTags(const std::string& tags)
    {
        SubString substr = SubString(tags, ",", " "); // Split the string into tags.
        const std::vector<std::string>& strings = substr.getAllStrings();
        for(std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end(); it++)
            this->addTag(*it, false);

        this->tagsUpdated();
    }
    /**
    @brief
        Set the starting ship models of the level
    @param tags
        A comma-seperated string of all the allowed ship models for the shipselection.
    */
    void LevelInfoItem::setShips(const std::string& ships)
    {
        SubString substr = SubString(ships, ",", " "); // Split the string into tags.
        const std::vector<std::string>& strings = substr.getAllStrings();
        for(std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end(); it++)
            this->addShip(*it, false);

        this->shipsUpdated();
    }

    /**
    @brief
        Add a tag to the set of tags the Level is tagged with.
    @param tag
        The tag to be added.
    @param update
        Whether the comma-seperated string of all tags should be updated. Default is true.
    @return
        Returns true if the tag was successfully added, if the tag was already present it returns false.
    */
    bool LevelInfoItem::addTag(const std::string& tag, bool update)
    {
        if(!this->validateTag(tag))
        {
            orxout(internal_warning) << "Bad LevelInfo tag '" << tag << "' in " << this->getXMLFilename() << ". Ignoring..." << endl;
            return false;
        }
        bool success = this->tags_.insert(*LevelInfoItem::possibleTags_s.find(tag)).second;
        if(update && success)
            this->tagsUpdated();
        return success;
    }

    /**
    @brief
        Add a ship model to allowed models for the shipselection
    @param ship
        The ship model to be added.
    @param update
        Whether the comma-seperated string of all ship models should be updated. Default is true.
    @return
        Returns true if the ship was successfully added, if the ship was already present it returns false.
    */
    bool LevelInfoItem::addShip(const std::string& ship, bool update)
    {
        bool success = this->ships_.insert(ship).second;
        if(update && success)
            this->shipsUpdated();
            
        return success;
    }


    /**
    @brief
        Updates the comma-seperated string of all tags, if the set of tags has changed.
    */
    void LevelInfoItem::tagsUpdated(void)
    {
        std::stringstream stream;
        std::set<std::string>::iterator temp;
        for(std::set<std::string>::iterator it = this->tags_.begin(); it != this->tags_.end(); )
        {
            temp = it;
            if(++it == this->tags_.end()) // If this is the last tag we don't add a comma.
                stream << *temp;
            else
                stream << *temp << ", ";
        }

        this->tagsString_ = std::string(stream.str());
    }

    /**
    @brief
        Updates the comma-seperated string of all ships, if the set of tags has changed.
    */
    void LevelInfoItem::shipsUpdated(void)
    {
        std::stringstream stream;
        std::set<std::string>::iterator temp;
        for(std::set<std::string>::iterator it = this->ships_.begin(); it != this->ships_.end(); )
        {
            temp = it;
            if(++it == this->ships_.end()) // If this is the last ship we don't add a comma.
                stream << *temp;
            else
                stream << *temp << ", ";
        }

        this->startingShipsString_ = std::string(stream.str());
    }
    // LevelInfo

    CreateFactory(LevelInfo);

    /**
    @brief

    @param creator
        The creator of this object.
    */
    LevelInfo::LevelInfo(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(LevelInfo);

        this->xmlfilename_ = this->getFilename();
    }

    /**
    @brief
        Destructor.
    */
    LevelInfo::~LevelInfo()
    {

    }

    /**
    @brief
        Creates a LevelInfo object through XML.
    */
    void LevelInfo::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LevelInfo, XMLPort, xmlelement, mode);

        XMLPortParam(LevelInfo, "description", setDescription, getDescription, xmlelement, mode);
        XMLPortParam(LevelInfo, "screenshot", setScreenshot, getScreenshot, xmlelement, mode);
        XMLPortParam(LevelInfo, "tags", setTags, getTags, xmlelement, mode);
        XMLPortParam(LevelInfo, "startingships", setShips, getShips, xmlelement, mode);
    }

    /**
    @brief
        Copies the contents of this LevelInfo object to a new LevelInfoItem object.
        This is needed, because a LeveInfo object is only created within the scope of the XML-file it is loaded with and is destroyed once that is unloaded.
    @return
        Returns a new LevelInfoItem with the same contents as the LevelInfo object.
    */
    LevelInfoItem* LevelInfo::copy(void)
    {
        LevelInfoItem* info = new LevelInfoItem(this->BaseObject::getName(), this->getXMLFilename());
        info->setDescription(this->getDescription());
        info->setScreenshot(this->getScreenshot());
        info->setTags(this->getTags());
        info->setShips(this->getShips());
        return info;
    }

}

