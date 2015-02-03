--CampaignMenu.lua

local P = createMenuSheet("CampaignMenu")

function P:onShow()
    P:updateButtons()
end

function P.updateButtons()
    P.updateButton(0, winMgr:getWindow("orxonox/MissionOneButton"))
    P.updateButton(1, winMgr:getWindow("orxonox/MissionTwoButton"))
    P.updateButton(2, winMgr:getWindow("orxonox/MissionThreeButton"))
    P.updateButton(3, winMgr:getWindow("orxonox/MissionFourButton"))

    if (P.getIndexOfLastFinishedMission() == orxonox.LevelManager:getInstance():getNumberOfCampaignMissions() - 1) then
        local label = winMgr:getWindow("orxonox/CampaignMenuCongratulation")
        label:setProperty("Visible","True")
    end
end

function P.updateButton(index, button)
    if (P.shouldDisplayButton(index)) then
        button:setProperty("Visible", "True")

        if (P.shouldEnableButton(index)) then
            button:setProperty("Disabled", "False")
        end
    end
end

function P.shouldDisplayButton(index)
    local size = orxonox.LevelManager:getInstance():getNumberOfCampaignMissions()
    return index < size
end

function P.shouldEnableButton(index)
    return index <= P.getIndexOfLastFinishedMission() + 1
end

function P.getIndexOfLastFinishedMission()
    local lastMission = orxonox.LevelManager:getInstance():getLastFinishedCampaignMission()
    if (lastMission and lastMission ~= "") then
        local size = orxonox.LevelManager:getInstance():getNumberOfCampaignMissions()
        local index = 0
        while index < size do
            local mission = orxonox.LevelManager:getInstance():getCampaignMission(index)
            if (mission == lastMission) then 
                return index
            end
            index = index + 1
        end
    end
    return -1
end

function P.MissionOneButton_clicked(e)
    P.loadLevel(P.FindLevel(0))
end

function P.MissionTwoButton_clicked(e)
    P.loadLevel(P.FindLevel(1))
end

function P.MissionThreeButton_clicked(e)
    P.loadLevel(P.FindLevel(2))
end

function P.MissionFourButton_clicked(e)
    P.loadLevel(P.FindLevel(3))
end

function P.loadLevel(level)
    orxonox.execute("startGame " .. level:getXMLFilename())
    hideAllMenuSheets()
end

function P.FindLevel(index)
    local filename = orxonox.LevelManager:getInstance():getCampaignMission(index)
    local level = nil
    local templevel = nil
    local size = orxonox.LevelManager:getInstance():getNumberOfLevels()
    local index = 0
    while index < size do
        templevel = orxonox.LevelManager:getInstance():getAvailableLevelListItem(index)
        if(templevel:getXMLFilename()==filename) then 
            level = templevel
        end
    index=index+1
    end
    return level
end

function P.CampaignMenuBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P