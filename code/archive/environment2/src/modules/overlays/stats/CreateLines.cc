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
        playerNameText_->setTextSize(0.04f);
        playerNameText_->setColour(ColourValue(0.0f, 0.75f, 0.2f, 1.0f));
        playerNameText_->setPosition(Vector2(0.1f, topOffset + 0.01f));

        scoreText_ = new OverlayText(0);
        scoreText_->setTextSize(0.04f);
        scoreText_->setColour(ColourValue(0.0f, 0.75f, 0.2f, 1.0f));
        scoreText_->setPosition(Vector2(0.6f, topOffset + 0.01f));

        deathsText_ = new OverlayText(0);
        deathsText_->setTextSize(0.04f);
        deathsText_->setColour(ColourValue(0, 0.75f, 0.2f, 1.0f));
        deathsText_->setPosition(Vector2(0.8f, topOffset + 0.01f));

        background_ = new Stats(0);
        background_->setPosition(Vector2(leftOffset, topOffset));
        background_->setSize(Vector2(width, height));
    }

    CreateLines::~CreateLines()
    {
        this->playerNameText_->destroy();
        this->scoreText_->destroy();
        this->deathsText_->destroy();
        this->background_->destroy();
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
    void CreateLines::changedVisibility()
    {
        this->scoreText_->changedVisibility();
        this->deathsText_->changedVisibility();
        this->playerNameText_->changedVisibility();
        this->background_->changedVisibility();
    }

    void CreateLines::setOverlayGroup( OverlayGroup* group )
    {
        this->scoreText_->setOverlayGroup( group );
        this->deathsText_->setOverlayGroup( group );
        this->playerNameText_->setOverlayGroup( group );
        this->background_->setOverlayGroup( group );
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
