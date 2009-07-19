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
    @file
    @brief Implementation of the QuestManager class.
*/

#include "QuestManager.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "gui/GUIManager.h"

#include "objects/infos/PlayerInfo.h"
#include "objects/infos/PlayerInfo.h"
#include "overlays/GUIOverlay.h"
#include "Quest.h"
#include "QuestHint.h"
#include "QuestItem.h"

namespace orxonox
{
    //! Pointer to the current (and single) instance of this class.
    /*static*/ QuestManager* QuestManager::singletonRef_s = NULL;

    /**
    @brief
        Constructor. Registers the object.
    @todo
        Is inheriting from BaseObject proper?
    */
    QuestManager::QuestManager()
    {
        RegisterRootObject(QuestManager);

        assert(singletonRef_s == 0);
        singletonRef_s = this;
    }

    /**
    @brief
        Destructor.
    */
    QuestManager::~QuestManager()
    {

    }

    /**
    @brief
        Returns a reference to the current (and single) instance of the QuestManager, and creates one if there isn't one to begin with.
    @return
        Returns a reference to the single instance of the Quest Manager.
    */
    /*static*/ QuestManager & QuestManager::getInstance()
    {
        assert(singletonRef_s);
        return *singletonRef_s;
    }

    /**
    @brief
        Registers a Quest with the QuestManager to make it globally accessable.
        Uses it's id to make sure to be able to be identify and retrieve it later.
    @param quest
        The Quest that is to be registered.
    @return
        Returns true if successful, false if not.
    */
    bool QuestManager::registerQuest(Quest* quest)
    {
        if(quest == NULL) //!< Doh! Just as if there were actual quests behind NULL-pointers.
        {
            COUT(2) << "Registration of Quest in QuestManager failed, because inserted Quest-pointer was NULL." << std::endl;
            return false;
        }

        std::pair<std::map<std::string, Quest*>::iterator,bool> result;
        result = this->questMap_.insert( std::pair<std::string,Quest*>(quest->getId(),quest) ); //!< Inserting the Quest.

        if(result.second) //!< If inserting was a success.
        {
            COUT(3) << "Quest with questId {" << quest->getId() << "} successfully inserted." << std::endl;
            return true;
        }
        else
        {
           COUT(2) << "Quest with the same id was already present." << std::endl;
           return false;
        }
    }

    /**
    @brief
        Registers a QuestHint with the QuestManager to make it globally accessable.
        Uses it's id to make sure to be able to be identify and retrieve it later.
    @param hint
        The QuestHint to be registered.
    @return
        Returns true if successful, false if not.
    */
    bool QuestManager::registerHint(QuestHint* hint)
    {
        if(hint == NULL) //!< Still not liking NULL-pointers.
        {
            COUT(2) << "Registration of QuestHint in QuestManager failed, because inserted QuestHint-pointer was NULL." << std::endl;
            return false;
        }

        std::pair<std::map<std::string, QuestHint*>::iterator,bool> result;
        result = this->hintMap_.insert ( std::pair<std::string,QuestHint*>(hint->getId(),hint) ); //!< Inserting the QuestHSint.

        if(result.second) //!< If inserting was a success.
        {
            COUT(3) << "QuestHint with hintId {" << hint->getId() << "} successfully inserted." << std::endl;
            return true;
        }
        else
        {
           COUT(2) << "QuestHint with the same id was already present." << std::endl;
           return false;
        }
    }

    /**
    @brief
        Finds a Quest with the given id.
    @param questId
        The id of the Quest sought for.
    @return
        Returns a pointer to the Quest with the input id.
        Returns NULL if there is no Quest with the given questId.
    @throws
        Throws an exception if the given questId is invalid.
    */
    Quest* QuestManager::findQuest(const std::string & questId)
    {
        if(!QuestItem::isId(questId)) //!< Check vor validity of the given id.
        {
            ThrowException(Argument, "Invalid questId.");
        }

        Quest* quest;
        std::map<std::string, Quest*>::iterator it = this->questMap_.find(questId);
        if (it != this->questMap_.end()) //!< If the Quest is registered.
        {
            quest = it->second;
        }
        else
        {
           quest = NULL;
           COUT(2) << "The quest with id {" << questId << "} is nowhere to be found." << std::endl;
        }

        return quest;

    }

    /**
    @brief
        Finds a QuestHint with the given id.
    @param hintId
        The id of the QuestHint sought for.
    @return
        Returns a pointer to the QuestHint with the input id.
        Returns NULL if there is no QuestHint with the given hintId.
    @throws
        Throws an exception if the given hintId is invalid.
    */
    QuestHint* QuestManager::findHint(const std::string & hintId)
    {
        if(!QuestItem::isId(hintId)) //!< Check vor validity of the given id.
        {
            ThrowException(Argument, "Invalid hintId.");
        }

        QuestHint* hint;
        std::map<std::string, QuestHint*>::iterator it = this->hintMap_.find(hintId);
        if (it != this->hintMap_.end()) //!< If the QuestHint is registered.
        {
            hint = it->second;
        }
        else
        {
           hint = NULL;
           COUT(2) << "The hint with id {" << hintId << "} is nowhere to be found." << std::endl;
        }

        return hint;

    }

    /**
    @brief
        
    @param name
    @return
    */
    QuestContainer* QuestManager::getQuestTree(std::string & name)
    {
        GUIOverlay* gui = GUIManager::getInstance().getOverlay(name);

        PlayerInfo* player;
        if(gui == NULL)
        {
            COUT(1) << "Error: No GUIOverlay with the given name '" << name << "' present." << std::endl;
            return NULL;
        }
        BaseObject* obj = gui->getOwner();
        if(obj == NULL)
        {
            COUT(1) << "Error: GUIOverlay has no owner. " << std::endl;
            return NULL;
        }
        player = orxonox_cast<PlayerInfo*>(obj);
    
        QuestContainer* root = NULL;
        QuestContainer* current = NULL;
        
        std::list<Quest*>* rootQuests = new std::list<Quest*>();
        getRootQuests(player, *rootQuests);
        
        for(std::list<Quest*>::iterator it = rootQuests->begin(); it != rootQuests->end(); it++)
        {
            QuestContainer* container = addSubQuest(*it, player);

            if(root == NULL)
            {
                root = container;
            }
            else
            {
                current->next = container;
            }
            
            current = container;

        }
        if(current != NULL)
            current->next = NULL;

        delete rootQuests;

        return root;
    }

    /**
    @brief
        
    @param player
    @param list
    @return
    */
    void QuestManager::getRootQuests(const PlayerInfo* player, std::list<Quest*> & list)
    {
        for(std::map<std::string, Quest*>::iterator it=this->questMap_.begin(); it!=this->questMap_.end(); it++)
        {
            Quest* quest = (*it).second;
            if(quest->getParentQuest() == NULL && !quest->isInactive(player))
            {
                list.push_back(quest);
            }
        }
    }

    /**
    @brief
        
    @param quest
    @param player
    @return
    */
    QuestContainer* QuestManager::addSubQuest(Quest* quest, const PlayerInfo* player)
    {
        if(quest == NULL)
            return NULL;

        QuestContainer* container = new QuestContainer;
        container->description = quest->getDescription();
        container->hint = addHints(quest, player);

        if(quest->isActive(player))
        {
            container->status = "active";
        }
        else if(quest->isCompleted(player))
        {
            container->status = "completed";
        }
        else if(quest->isFailed(player))
        {
            container->status = "failed";
        }
        else
        {
            container->status = "";
            COUT(1) << "An error occured. A Quest of un-specified status wanted to be displayed." << std::endl;
        }
        
        std::list<Quest*> quests = quest->getSubQuestList();
        QuestContainer* current = NULL;
        QuestContainer* first = NULL;
        for(std::list<Quest*>::iterator it = quests.begin(); it != quests.end(); it++)
        {
            Quest* subQuest = *it;
            if(!subQuest->isInactive(player))
            {
                QuestContainer* subContainer = addSubQuest(subQuest, player);

                if(first == NULL)
                {
                    first = subContainer;
                }
                else
                {
                    current->next = subContainer;
                }
                
                current = subContainer;
            }
        }
        if(current != NULL)
            current->next = NULL;
        container->subQuests = first;
        
        return container;
    }

    /**
    @brief
        
    @param quest
    @param player
    @return
    */
    HintContainer* QuestManager::addHints(Quest* quest, const PlayerInfo* player)
    {
        HintContainer* current = NULL;
        HintContainer* first = NULL;

        std::list<QuestHint*> hints = quest->getHintsList();
        for(std::list<QuestHint*>::iterator it = hints.begin(); it != hints.end(); it++)
        {
            if((*it)->isActive(player))
            {
                HintContainer* hint = new HintContainer;
                hint->description = (*it)->getDescription();

                if(first == NULL)
                {
                    first = hint;
                }
                else
                {
                    current->next = hint;
                }
                
                current = hint;
            }
        }

        if(current != NULL)
            current->next = NULL;
        return first;
    }


}
