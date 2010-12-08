-- QuestGUI.lua

local P = createMenuSheet("QuestGUI")

P.rootWindow = nil
P.detailsWindows = {}
P.quests = {}
P.hints = {}
P.player = nil

-- design parameters
P.indentWidth = 20
P.scrollbarWidth = 13
P.buttonHeight = 30
P.titleHeight = 26
P.borderWidth = 5

function P.onShow()

    local questsList = winMgr:getWindow("orxonox/QuestGUI/QuestsList")

    P.player = orxonox.GUIManager:getInstance():getPlayer(P.name)
    P.rootWindow = P.createQuestGUI()

    questsList:addChildWindow(P.rootWindow)
end

function P.onHide()
    P.cleanup()
end

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
end

return P

