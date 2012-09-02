-- QuestGUI.lua

local P = createMenuSheet("QuestGUI")

P.questManager = nil -- The QuestManager.
P.showActive = true -- Whether the active or finished quest list is displayed.
P.currentQuest = nil -- The quest that is currently displayed.
P.player = nil -- The player the quests are displayed for.
P.quests = {}
P.subquests = {}

-- design parameters
P.scrollbarWidth = 13
P.frameHeigth = 18
P.borderSize = 5
P.titleHeight = 26

--TODO:
-- Highlight whether we are currently looking at active or finished quests
-- Distinguish completed from failed quests

function P.onLoad()
    P.questManager = orxonox.QuestManager:getInstance() -- Store the pointer to the QuestManager as an internal variable to allow for faster access,
end

function P.onShow()
    -- Get the player.
    P.player = orxonox.GUIManager:getInstance():getPlayer(P.name)

    -- Load the list of quests to be displayed.
    P.loadQuestsList(P.currentQuest)
end

-- Loads the list of quests, depending on P.showActive, either the active (P.showActive == true) or the finished, i.e. inactive quests are loaded.
-- selectQuest is a pointer to a quest that should be selected, if it is nil the first quest is selected.
function P.loadQuestsList(selectQuest)
    local list = CEGUI.toListbox(winMgr:getWindow("orxonox/QuestGUI/QuestsList"))
    P.clearQuestList()

    local selectQuestId = nil
    if selectQuest ~= nil then
        selectQuestId = P.questManager:getId(selectQuest)
    end

    -- Iterate through all root-quests.
    local numRootQuests = P.questManager:getNumRootQuests(P.player)
    if numRootQuests > 0 then
        local i = 0
        while i <= numRootQuests-1 do
            local quest = P.questManager:getRootQuest(P.player, i)
            -- Insert the current quest into the list.
            local item = P.insertQuest(list, quest)
            -- If the quest was inserted in the list and is has the same id as the selectQuest (thus it is the same quest) it is selected.
            if selectQuestId ~= nil and item ~= nil and selectQuestId == P.questManager:getId(quest) then
                list:setItemSelectState(item, true)
            end
            -- Insert all subquests of this rootquest.
            P.insertSubQuests(list, quest, selectQuestId)
            i = i+1
        end
        -- If there were quests added to the list but there was no selectQuest specified (i.e. selectQuest was nil), the first item is selected.
        if list:getItemCount() > 0 then
            if selectQuestId == nil then
                list:setItemSelectState(list:getListboxItemFromIndex(0), true)  -- Select first quest.
            end
        -- If there werent any quests added the standard "no quests" message is loaded.
        else
            P.loadQuest()
        end
    end
end

-- Helper function, recursively inserts all the (active or inactive, depending on P.showActive) subquests of the input quest.
-- list is the list into which the subquests should be insterted.
-- quest is the quest, whose subquests should be inserted.
-- selectQuestId is the id of the quest that should be selected.
function P.insertSubQuests(list, quest, selectQuestId)
    -- Iterate through all sub-quests.
    local numQuests = P.questManager:getNumSubQuests(quest, P.player)
    if numQuests > 0 then
        local i = 0
        while i <= numQuests-1 do
            local subquest = P.questManager:getSubQuest(quest, P.player, i)
            -- Insert the current quest into the list.
            local item = P.insertQuest(list, subquest)
            -- If the quest was inserted in the list and is has the same id as the selectQuest (thus it is the same quest) it is selected.
            if selectQuestId ~= nil and item ~= nil and selectQuestId == P.questManager:getId(subquest) then
                list:setItemSelectState(item, true)
            end
            i = i+1
        end
    end
end

-- Helper function, inserts a quest into the list (depending whether active or inactive quests are being shown). Returns nil if the quest was not inserted.
-- list is the list into which the quets should be inserted.
-- quest is the quest to be inserted.
function P.insertQuest(list, quest)
    if P.showActive == quest:isActive(P.player) then
        local item = CEGUI.createListboxTextItem(P.questManager:getDescription(quest):getTitle())
        item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
        list:addItem(item)
        table.insert(P.quests, quest)
        return item
    end
    return nil
end

-- Loads the input quest.
-- quest the quest to be loaded.
function P.loadQuest(quest)

    P.clearQuest() -- Clear the old quest.
    if quest == nil then -- If quets is nil there is nothing to display.
        return
    else
        local offset = 0

        -- Load title and description
        local description = P.questManager:getDescription(quest)
        local titleWindow = winMgr:getWindow("orxonox/QuestGUI/Quest/Title")
        titleWindow:setText(description:getTitle())
        local descriptionWindow = winMgr:getWindow("orxonox/QuestGUI/Quest/Description")
        descriptionWindow:setText(description:getDescription())
        descriptionWindow:setSize(CEGUI.UVector2(CEGUI.UDim(1, -P.borderSize), CEGUI.UDim(1, 0)))
        descriptionWindow:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.borderSize), CEGUI.UDim(0, P.borderSize)))
        local height = getStaticTextWindowHeight(descriptionWindow)
        descriptionWindow:setHeight(CEGUI.UDim(0, height))
        offset = offset + height

        -- Load subquests
        local list = CEGUI.toListbox(winMgr:getWindow("orxonox/QuestGUI/Quest/SubquestsList"))
        local numQuests = P.questManager:getNumSubQuests(quest, P.player)
        local i = 0
        while i <= numQuests-1 do
            local quest = P.questManager:getSubQuest(quest, P.player, i)
            local item = CEGUI.createListboxTextItem(P.questManager:getDescription(quest):getTitle())
            item:setSelectionBrushImage(menuImageSet, "MultiListSelectionBrush")
            list:addItem(item)
            table.insert(P.subquests, quest)
            i = i+1
        end
        height = list:getTotalItemsHeight()
        if height > 0 then
            height = height+P.frameHeigth
        end
        list:setSize(CEGUI.UVector2(CEGUI.UDim(1, -P.borderSize), CEGUI.UDim(0, height)))
        list:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.borderSize), CEGUI.UDim(0, offset)))
        offset = offset + height + P.borderSize

        -- Load hints
        local hintsWindow = winMgr:getWindow("orxonox/QuestGUI/Quest/Hints")
        hintsWindow:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.borderSize), CEGUI.UDim(0, offset)))
        hintsWindow:setSize(CEGUI.UVector2(CEGUI.UDim(1, -P.borderSize), CEGUI.UDim(0, 0)))
        height = P.titleHeight
        local numHints = P.questManager:getNumHints(quest, P.player)
        local i = 0
        while i <= numHints-1 do
            local hint = P.questManager:getHints(quest, P.player, i)
            height = height + P.insertHint(hintsWindow, hint, i, height)
            i = i+1
        end
        if numHints == 0 then
            height = 0
        end
        hintsWindow:setHeight(CEGUI.UDim(0, height))
        offset = offset + height

        -- Set the size of the wrapper
        local window = winMgr:getWindow("orxonox/QuestGUI/Quest/Wrapper")
        window:setSize(CEGUI.UVector2(CEGUI.UDim(1, -P.borderSize-P.scrollbarWidth), CEGUI.UDim(0,offset+P.borderSize)))
    end

    P.currentQuest = quest
end

-- Clear the currently displayed quest.
function P.clearQuest()
    -- clear title
    local titleWindow = winMgr:getWindow("orxonox/QuestGUI/Quest/Title")
    titleWindow:setText("no Quests")

    -- clear description
    local descriptionWindow = winMgr:getWindow("orxonox/QuestGUI/Quest/Description")
    descriptionWindow:setText("There is currently no quest that can be displayed.")

    -- clear list fo subquests
    local list = CEGUI.toListbox(winMgr:getWindow("orxonox/QuestGUI/Quest/SubquestsList"))
    list:resetList()
    list:setHeight(CEGUI.UDim(0, 0))
    P.subquests = {}

    -- clear hints
    local hints = winMgr:getWindow("orxonox/QuestGUI/Quest/Hints")
    local numChildren = hints:getChildCount()-2 -- TODO: HACK
    local i = 0
    while i < numChildren do
        local hint = hints:getChild("orxonox/QuestGUI/Quest/Hints/" .. i)
        if hint ~= nil then
            hints:removeChildWindow(hint)
            winMgr:destroyWindow(hint)
        end
        i = i+1
    end
    hints:setSize(CEGUI.UVector2(CEGUI.UDim(1, -P.scrollbarWidth-P.borderSize), CEGUI.UDim(0, 0)))

    P.currentQuest = nil
end

-- Clear the quests list
function P.clearQuestList()
    local list = CEGUI.toListbox(winMgr:getWindow("orxonox/QuestGUI/QuestsList"))
    list:resetList()
    P.quests = {}
end

-- Select an input quest in the input list.
-- list is the list in which the input quest is to be selected.
-- quest is the quest to be selected.
function P.selectQuest(list, quest)
    if quest == nil then -- If the input quest is nil, there is nothing to be selected, an error is output and the first quest is selected instead.
        cout(1, "Error in QuestGUI: selectQuest(), input quest is nil. Selecting first.")
        list:setItemSelectState(list:getListboxItemFromIndex(0), true) -- Select first
        return
    end

    local questId = P.questManager:getId(quest)
    local found = false
    local index = 0
    -- Iterate over all quests currently in the list.
    for k,v in pairs(P.quests) do
        -- If the id's are the same we have found the quest.
        if P.questManager:getId(v) == questId then
            found = true
            index = k-1
        end
    end

    if found then -- If the quest was found it is selected.
        list:setItemSelectState(list:getListboxItemFromIndex(index), true)
    else -- If the quest isn't found an error is output and the first quest is selected instead.
        cout(1, "Error in QuestGUI: selectQuest(), input quest is not in list. Selecting first.")
        list:setItemSelectState(list:getListboxItemFromIndex(0), true) -- Select first
    end
end

-- Helper function, insert the input hint into the input hintsWindow. Returns the height of the newly inserted hint.
-- hintsWindow is the window in which the hint is to be inserted.
-- hint is the hint to be inserted.
-- index is the index of the hint.
-- offset is the current offset in the hintsWindow.
function P.insertHint(hintsWindow, hint, index, offset)
    -- Create the window for the hint.
    local window = winMgr:createWindow("MenuWidgets/StaticText", "orxonox/QuestGUI/Quest/Hints/" .. index)
    window:setProperty("HorzFormatting", "WordWrapLeftAligned")
    window:setProperty("VertFormatting", "TopAligned")
    window:setProperty("FrameEnabled", "false")
    window:setID(index)
    hintsWindow:addChildWindow(window)
    local description = P.questManager:getDescription(hint)
    window:setText(description:getDescription())
    window:setSize(CEGUI.UVector2(CEGUI.UDim(1, -P.borderSize), CEGUI.UDim(1, 0)))
    local height = getStaticTextWindowHeight(window)
    window:setHeight(CEGUI.UDim(0, height))
    window:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.borderSize), CEGUI.UDim(0, offset)))
    return height
end

-- Show the currently active quests in the quests list.
function P.showActiveQuestsButton_clicked(e)
    if P.showActive == false then
        P.showActive = true
        P.loadQuestsList()
    end
end

-- Show the finished (i.e. inactive) quests in the quests list.
function P.showFinishedQuestsButton_clicked(e)
    if P.showActive == true then
        P.showActive = false
        P.loadQuestsList()
    end
end

-- Change to a new quest.
function P.changeQuest_clicked(e)
    local list = CEGUI.toListbox(winMgr:getWindow("orxonox/QuestGUI/QuestsList"))
    local choice = list:getFirstSelectedItem()
    if choice ~= nil then
        local index = list:getItemIndex(choice)
        local quest = P.quests[index+1]
        if quest ~= nil then
            P.loadQuest(quest)
        end
    end
end

-- Change to a new subquest.
function P.changeToSubquest_clicked(e)
    local list = CEGUI.toListbox(winMgr:getWindow("orxonox/QuestGUI/Quest/SubquestsList"))
    local questsList = CEGUI.toListbox(winMgr:getWindow("orxonox/QuestGUI/QuestsList"))
    local choice = list:getFirstSelectedItem()
    if choice ~= nil then
        local index = list:getItemIndex(choice)
        local quest = P.subquests[index+1]
        if quest ~= nil then
            -- If the P.showActive must be changed to display the quest the quests list also has to be regenerated.
            if quest:isActive(P.player) == P.showActive then
                P.selectQuest(questsList, quest)
            else
                P.showActive = quest:isActive(P.player)
                P.loadQuestsList(quest)
            end
        else
            cout(1, "Error in QuestGUI: changeToSubquest(), quest was nil. Ignoring...")
        end
    end
end

-- old:
--[[
function P.createQuestGUI()
    local questManager = orxonox.QuestManager:getInstance()

    local depth = 0
    local index = 0

    local questWindow = winMgr:createWindow("MenuWidgets/ScrollablePane", "orxonox/QuestGUI/Quests")
    questWindow:setSize(CEGUI.UVector2(CEGUI.UDim(1, 0),CEGUI.UDim(1, 0)))

    -- Iterate through all root-quests.
    local numRootQuests = orxonox.QuestManager:getInstance():getNumRootQuests(P.player)
    local i = 0
    while i <= numRootQuests-1 do
        local quest = orxonox.QuestManager:getInstance():getRootQuest(P.player, i)
        index = P.createQuestNodes(questWindow, quest, depth, index)
        i = i+1
    end

    return questWindow
end

function P.createQuestNodes(root, parent, depth, index)
    local number = table.getn(P.quests)+1
    local name = "orxonox/QuestGUI/Quests/" .. number
    local node = winMgr:createWindow("MenuWidgets/TabButton", name)
    node:setText(orxonox.QuestManager:getInstance():getDescription(parent):getTitle())
    node:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.indentWidth*depth), CEGUI.UDim(0, P.buttonHeight*index)))
    node:setSize(CEGUI.UVector2(CEGUI.UDim(1, -P.indentWidth*depth-P.scrollbarWidth), CEGUI.UDim(0, P.buttonHeight)))
    orxonox.GUIManager:subscribeEventHelper(node, "Clicked", P.name .. ".openDetails_clicked")
    root:addChildWindow(node)
    
    table.insert(P.quests, parent)

    index = index+1

    -- Iterate through all sub-quests.
    local numQuests = orxonox.QuestManager:getInstance():getNumSubQuests(parent, P.player)
    local i = 0
    while i <= numQuests-1 do
        local quest = orxonox.QuestManager:getInstance():getSubQuest(parent, P.player, i)
        index = P.createQuestNodes(root, quest, depth+1, index)
        i = i+1
    end

    return index
end

function P.cleanup()
    winMgr:destroyWindow(P.rootWindow)
    for k,v in pairs(P.detailsWindows) do
        if v ~= nil then
            winMgr:destroyWindow(v)
            P.detailsWindows[k] = nil
        end
    end
    P.detailsWindows = {}

    P.quests = {}
    P.hints = {}
    P.player = nil

    winMgr:destroyWindow(P.rootWindow)
    P.rootWindow = nil
end

function P.openDetails_clicked(e)
    --Get some numbers from the window
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    local match = string.gmatch(name, "%d+")
    local questNr = tonumber(match())

    name = name .. "/Details" .. P.getNewDetailNumber()
    quest = P.quests[questNr]

    local details = winMgr:createWindow("MenuWidgets/FrameWindow", name)
    details:setSize(CEGUI.UVector2(CEGUI.UDim(0.7, 0), CEGUI.UDim(0.7, 0)))
    details:setPosition(CEGUI.UVector2(CEGUI.UDim(0.1, 0), CEGUI.UDim(0.1, 0)))
    details:setText(orxonox.QuestManager:getInstance():getDescription(quest):getTitle())
    details:setProperty("Alpha", 1.0)
    details:setProperty("InheritsAlpha", "setFalse")
    orxonox.GUIManager:subscribeEventHelper(details, "CloseClicked", P.name .. ".closeDetails_clicked")

    table.insert(P.detailsWindows, details)

    name = name .. "/Scrollable"
    local window = winMgr:createWindow("MenuWidgets/ScrollablePane", name)
    window:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -2*P.borderWidth),CEGUI.UDim(1.0, -P.titleHeight)))
    window:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.borderWidth), CEGUI.UDim(0, P.titleHeight)))
    details:addChildWindow(window)

    local offset = 0

    local status = winMgr:createWindow("MenuWidgets/StaticText", name .. "/Status")
    window:addChildWindow(status)
    status:setProperty("HorzFormatting", "WordWrapLeftAligned")
    status:setProperty("VertFormatting", "TopAligned")
    if quest:isActive(P.player) then
        status:setText("This quest is active.")
    elseif quest:isCompleted(P.player) then
        status:setText("This quest was completed.")
    elseif quest:isFailed(P.player) then
        status:setText("This quest was failed.")
    end
    status:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, offset)))
    status:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -P.scrollbarWidth), CEGUI.UDim(1.0, 0)))
    local height = getStaticTextWindowHeight(status)
    status:setHeight(CEGUI.UDim(0, height))
    offset = offset + height

    local descriptionTitle = winMgr:createWindow("MenuWidgets/StaticText", name .. "/Description/Title")
    window:addChildWindow(descriptionTitle)
    descriptionTitle:setProperty("HorzFormatting", "HorzCentred")
    descriptionTitle:setProperty("VertFormatting", "TopAligned")
    descriptionTitle:setText("Description:")
    descriptionTitle:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, offset)))
    descriptionTitle:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -P.scrollbarWidth), CEGUI.UDim(1.0, 0)))
    height = getStaticTextWindowHeight(descriptionTitle)
    descriptionTitle:setHeight(CEGUI.UDim(0, height))
    offset = offset + height

    local description = winMgr:createWindow("MenuWidgets/StaticText", name .. "/Description")
    window:addChildWindow(description)
    description:setProperty("HorzFormatting", "WordWrapLeftAligned")
    description:setProperty("VertFormatting", "TopAligned")
    description:setText(orxonox.QuestManager:getInstance():getDescription(quest):getDescription())
    description:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, offset)))
    description:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -P.scrollbarWidth), CEGUI.UDim(1.0, 0)))
    height = getStaticTextWindowHeight(description)
    description:setHeight(CEGUI.UDim(0, height))
    offset = offset + height

    -- Display the hints of this quest
    local numHints = orxonox.QuestManager:getInstance():getNumHints(quest, P.player)
    if numHints > 0 then
        local hintsTitle = winMgr:createWindow("MenuWidgets/StaticText", name .. "/Hints/Title")
        window:addChildWindow(hintsTitle)
        hintsTitle:setProperty("HorzFormatting", "HorzCentred")
        hintsTitle:setProperty("VertFormatting", "TopAligned")
        hintsTitle:setText("Hints:")
        hintsTitle:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, offset)))
        hintsTitle:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -P.scrollbarWidth), CEGUI.UDim(1.0, 0)))
        height = getStaticTextWindowHeight(hintsTitle)
        hintsTitle:setHeight(CEGUI.UDim(0, height))
        offset = offset + height
    end
    local i = 0
    while i <= numHints-1 do
        local hint = orxonox.QuestManager:getInstance():getHints(quest, P.player, i)
        table.insert(P.hints, hint)
        local number = table.getn(P.hints)
        local node = winMgr:createWindow("MenuWidgets/TabButton", name .. "/Hints" .. number)
        node:setText(orxonox.QuestManager:getInstance():getDescription(hint):getTitle())
        node:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, offset)))
        node:setSize(CEGUI.UVector2(CEGUI.UDim(1, -P.scrollbarWidth), CEGUI.UDim(0, P.buttonHeight)))
        window:addChildWindow(node)
        offset = offset + P.buttonHeight

        orxonox.GUIManager:subscribeEventHelper(node, "Clicked", P.name .. ".openHintDetails_clicked")
        i = i+1
    end

    local window = winMgr:getWindow("orxonox/QuestGUI/Background")
    window:addChildWindow(details)
end

function P.getNewDetailNumber()
    local number = table.getn(P.detailsWindows)
    for k,v in pairs(P.detailsWindows) do
        if v == nil then
            number = k-1
        end
    end
    return number+1
end

function P.closeDetails_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    local match = string.gmatch(name, "%d+")
    match()
    local detailsNr = tonumber(match())

    winMgr:destroyWindow(P.detailsWindows[detailsNr])
    P.detailsWindows[detailsNr] = nil
end

function P.openHintDetails_clicked(e)
    --Get some numbers from the window
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    local match = string.gmatch(name, "%d+")
    match()
    match()
    local hintNr = tonumber(match())

    name = name .. "/Details" .. P.getNewDetailNumber()
    hint = P.hints[hintNr]

    local details = winMgr:createWindow("MenuWidgets/FrameWindow", name)
    details:setSize(CEGUI.UVector2(CEGUI.UDim(0.7, 0), CEGUI.UDim(0.7, 0)))
    details:setPosition(CEGUI.UVector2(CEGUI.UDim(0.1, 0), CEGUI.UDim(0.1, 0)))
    details:setText(orxonox.QuestManager:getInstance():getDescription(hint):getTitle())
    details:setProperty("Alpha", 1.0)
    details:setProperty("InheritsAlpha", "setFalse")
    orxonox.GUIManager:subscribeEventHelper(details, "CloseClicked", P.name .. ".closeHintDetails_clicked")

    table.insert(P.detailsWindows, details)

    name = name .. "/Scrollable"
    local window = winMgr:createWindow("MenuWidgets/ScrollablePane", name)
    window:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -2*P.borderWidth),CEGUI.UDim(1.0, -P.titleHeight)))
    window:setPosition(CEGUI.UVector2(CEGUI.UDim(0, P.borderWidth), CEGUI.UDim(0, P.titleHeight)))
    details:addChildWindow(window)

    local offset = 0
    
    local descriptionTitle = winMgr:createWindow("MenuWidgets/StaticText", name .. "/Description/Title")
    window:addChildWindow(descriptionTitle)
    descriptionTitle:setProperty("HorzFormatting", "HorzCentred")
    descriptionTitle:setProperty("VertFormatting", "TopAligned")
    descriptionTitle:setText("Description:")
    descriptionTitle:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, offset)))
    descriptionTitle:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -P.scrollbarWidth), CEGUI.UDim(1.0, 0)))
    height = getStaticTextWindowHeight(descriptionTitle)
    descriptionTitle:setHeight(CEGUI.UDim(0, height))
    offset = offset + height

    local description = winMgr:createWindow("MenuWidgets/StaticText", name .. "/Description")
    window:addChildWindow(description)
    description:setProperty("HorzFormatting", "WordWrapLeftAligned")
    description:setProperty("VertFormatting", "TopAligned")
    description:setText(orxonox.QuestManager:getInstance():getDescription(hint):getDescription())
    description:setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, offset)))
    description:setSize(CEGUI.UVector2(CEGUI.UDim(1.0, -P.scrollbarWidth), CEGUI.UDim(1.0, 0)))
    height = getStaticTextWindowHeight(description)
    description:setHeight(CEGUI.UDim(0, height))

    local window = winMgr:getWindow("orxonox/QuestGUI/Background")
    window:addChildWindow(details)
end

function P.closeHintDetails_clicked(e)
    local we = CEGUI.toWindowEventArgs(e)
    local name = we.window:getName()
    local match = string.gmatch(name, "%d+")
    match()
    match()
    match()
    local detailsNr = tonumber(match())

    winMgr:destroyWindow(P.detailsWindows[detailsNr])
    P.detailsWindows[detailsNr] = nil
end --]]

return P

