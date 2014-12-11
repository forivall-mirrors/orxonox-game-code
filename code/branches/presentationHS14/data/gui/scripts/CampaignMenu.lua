--CampaignMenu.lua

local P = createMenuSheet("CampaignMenu")

function P.onLoad()
	
	local MissionTwoButton = winMgr:getWindow("orxonox/MissionTwoButton")
	if (P.CheckLevel("missionOne.oxw")) then
	MissionTwoButton:setProperty("Disabled", "False")
	end
	
	local MissionThreeButton = winMgr:getWindow("orxonox/MissionThreeButton")
	if (P.CheckLevel("fightInOurBack.oxw")) then
	MissionThreeButton:setProperty("Disabled", "False")
	end

	local MissionFourButton = winMgr:getWindow("orxonox/MissionFourButton")
	if (P.CheckLevel("pirateAttack.oxw")) then
	MissionFourButton:setProperty("Disabled", "False")
	end
	
	local Completed = winMgr:getWindow("orxonox/CampaignMenuCongratulation")
	if (P.CheckLevel("iJohnVane_TriptoArea51.oxw")) then
	Completed:setProperty("Visible","True")
	end
end

function P.GenerateHelperString(number)
	local string = ""
	while number > 1 do
		string = string.." "
		number = number-1
	end
	string = string.."."
	return string
end

function P.FindLevel(filename)
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
    
function P.CheckLevel(filename)
	local file = io.open("/home/maxima/maxima-extra-0/orxonox/presentationHS14/data/gui/scripts/campaign.txt", "r+")
	local index = 0
	local returnvalue = false
	local numberoflines = 58-string.len(filename)
	local helpstringfalse = filename.." 0"..P.GenerateHelperString(numberoflines) 
	local helpstringtrue = filename.." 1"..P.GenerateHelperString(numberoflines)
	while index < 100 do
		local line = file:read()
		if(line == helpstringfalse) then
			returnvalue = false 
			break
		end
		if(line == helpstringtrue) then
			returnvalue = true
			break
		end
		index=index+1
	end	
	io.close(file)
	return returnvalue
end

function P.MissionOneButton_clicked(e)
	local level = P.FindLevel("missionOne.oxw")
		orxonox.execute("startGame " .. level:getXMLFilename())
		hideAllMenuSheets()
end

function P.MissionTwoButton_clicked(e)
	local level = P.FindLevel("fightInOurBack.oxw")
	if (P.CheckLevel("missionOne.oxw")) then
		orxonox.execute("startGame " .. level:getXMLFilename())
		hideAllMenuSheets()
	end
end

function P.MissionThreeButton_clicked(e)
	local level = P.FindLevel("pirateAttack.oxw")
	if (P.CheckLevel("fightInOurBack.oxw")) then
		orxonox.execute("startGame " .. level:getXMLFilename())
		hideAllMenuSheets()
	else
		 hideMenuSheet(P.name)
	end
end

function P.MissionFourButton_clicked(e)
	local level = P.FindLevel("iJohnVane_TriptoArea51.oxw")
	if (P.CheckLevel("pirateAttack.oxw")) then
		orxonox.execute("startGame " .. level:getXMLFilename())
		hideAllMenuSheets()
	else
		 hideMenuSheet(P.name)
	end
end

function P.CampaignMenuRefreshButton_clicked(e)
	local MissionTwoButton = winMgr:getWindow("orxonox/MissionTwoButton")
	if (P.CheckLevel("missionOne.oxw")) then
	MissionTwoButton:setProperty("Disabled", "False")
	end
	
	local MissionThreeButton = winMgr:getWindow("orxonox/MissionThreeButton")
	if (P.CheckLevel("fightInOurBack.oxw")) then
	MissionThreeButton:setProperty("Disabled", "False")
	end

	local MissionFourButton = winMgr:getWindow("orxonox/MissionFourButton")
	if (P.CheckLevel("pirateAttack.oxw")) then
	MissionFourButton:setProperty("Disabled", "False")
	end
	
	local Completed = winMgr:getWindow("orxonox/CampaignMenuCongratulation")
	if (P.CheckLevel("iJohnVane_TriptoArea51.oxw")) then
	Completed:setProperty("Visible","True")
	end
end

function P.CampaignMenuBackButton_clicked(e)
    hideMenuSheet(P.name)
end

return P
