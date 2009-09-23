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
 *      Benjamin Hildebrandt
 *
 */

#include "CreateLines.h"

#include "overlays/OverlayText.h"
#include "overlays/stats/Stats.h"

namespace orxonox
{
    /**
        @brief Constructor: Creates a line.
    */
    CreateLines::CreateLines(float leftOffset, float topOffset, float width, float height)
    {
        playerNameText_ = new OverlayText(0);
        playerNameText_->setTextSize(0.04);
        playerNameText_->setColour(ColourValue(0, 0.75, 0.2, 1));
        playerNameText_->setPosition(Vector2(0.1, topOffset + 0.01));

        scoreText_ = new OverlayText(0);
        scoreText_->setTextSize(0.04);
        scoreText_->setColour(ColourValue(0, 0.75, 0.2, 1));
        scoreText_->setPosition(Vector2(0.6, topOffset + 0.01));

        deathsText_ = new OverlayText(0);
        deathsText_->setTextSize(0.04);
        deathsText_->setColour(ColourValue(0, 0.75, 0.2, 1));
        deathsText_->setPosition(Vector2(0.8, topOffset + 0.01));

        background_ = new Stats(0);
        background_->setPosition(Vector2(leftOffset, topOffset));
        background_->setSize(Vector2(width, height));
    }

    CreateLines::~CreateLines()
    {
        delete this->playerNameText_;
        delete this->scoreText_;
        delete this->deathsText_;
        delete this->background_;
    }

    void CreateLines::setPlayerName(const std::string& str)
    {
        this->playerNameText_->setCaption(str);
    }

    void CreateLines::setScore(const std::string& str)
    {
        this->scoreText_->setCaption(str);
    }

    void CreateLines::setDeaths(const std::string& str)
    {
        this->deathsText_->setCaption(str);
    }

    void CreateLines::setVisibility(bool visible)
    {
        this->scoreText_->setVisible(visible);
        this->deathsText_->setVisible(visible);
        this->playerNameText_->setVisible(visible);
        this->background_->setVisible(visible);
    }

    /**
        @brief Ensures that the number of OverlayElements is equal to numberOfColumns.
    */
    //void CreateLines::setNumberOfColumns(unsigned int numberOfColumns, unsigned int lineIndex) {

    //    Ogre::OverlayManager* ovMan = Ogre::OverlayManager::getSingletonPtr();

    //    unsigned int colIndex = 0;

    //    while (textColumns_.size() < numberOfColumns)
    //    {
    //        Ogre::TextAreaOverlayElement* tempTextArea = static_cast<Ogre::TextAreaOverlayElement*>(ovMan->createOverlayElement("TextArea", "StatsLineTextArea" + getName() + multi_cast<std::string>(lineIndex) + multi_cast<std::string>(colIndex)));
    //        textColumns_.push_back(tempTextArea);
    //        this->background_->addChild(tempTextArea);

    //        colIndex++;
    //    }

    //    while (textColumns_.size() > numberOfColumns)
    //    {
    //        this->background_->_removeChild(textColumns_.back());
    //        ovMan->destroyOverlayElement(textColumns_.back());
    //        textColumns_.pop_back();
    //    }
    //}

    //void CreateLines::alignColumn(int columnIndex, float leftOffset, float topOffset)
    //{
    //    this->textColumns_[columnIndex]->setPosition(leftOffset, topOffset);
    //}

    //void CreateLines::setColumnText(int columnIndex, std::string columnText)
    //{
    //    this->textColumns_[columnIndex]->setCaption(columnText);
    //}
}
