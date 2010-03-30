winMgr   = CEGUI.WindowManager:getSingleton()
guiMgr   = orxonox.GUIManager:getInstance()
inputMgr = orxonox.InputManager:getInstance()

local schemeMgr = CEGUI.SchemeManager:getSingleton()
local system    = CEGUI.System:getSingleton()
local cursor    = CEGUI.MouseCursor:getSingleton()

-- Load all required skins
schemeMgr:loadScheme("TaharezGreenLook.scheme")
--schemeMgr:loadScheme("TaharezLook.scheme")
--schemeMgr:loadScheme("WindowsLook.scheme")
--schemeMgr:loadScheme("VanillaLook.scheme")
--schemeMgr:loadScheme("SleekSpaceLook.scheme")

-- Connect skin specific window types with our own window types
-- By loading a different file (if there is) you can change the skin
-- of the menus or the HUD independently
schemeMgr:loadScheme("TaharezGreenMenuWidgets.scheme")
menuImageSet = "TaharezGreenLook"
schemeMgr:loadScheme("TaharezGreenHUDWidgets.scheme")
hudImageSet = "TaharezGreenLook"

-- Just a remaining test hack
schemeMgr:loadScheme("OrxonoxGUIScheme.scheme")

system:setDefaultMouseCursor(menuImageSet, "MouseArrow")
system:setDefaultFont("BlueHighway-12")
system:setDefaultTooltip("MenuWidgets/Tooltip")

local loadedSheets = {}
local activeMenuSheets = {size = 0, topSheet = nil}
--activeHUDSheets  = {size = 0, topSheet = nil}
local root = nil

-- Require all tools
require("GUITools")


-----------------------
--- Local functions ---
-----------------------

-- Loads the GUI with the specified name
-- The name corresponds to the filename of the *.lua and *.layout files
-- but without the extension
local function loadSheet(name)
    -- Check if it has already been loaded
    local sheet = loadedSheets[name]
    if sheet == nil then
        -- Load the sheet
        sheet = require(name)
        if sheet == nil then
            return
        end
        sheet:load()
        loadedSheets[name] = sheet
        -- Hide new GUI as we do not want to show it accidentally
        sheet:hide()
    end
    return sheet
end

local function hideCursor()
    if cursor:isVisible() then
        cursor:hide()
    end
end

local function showCursor()
    if not cursor:isVisible() and orxonox.InputManager:getInstance():isMouseExclusive() then
        cursor:show()
    end
end

local function find(table, value)
    for i, v in ipairs(table) do
        if v == value then
            return i
        end
    end
    return nil
end


------------------------
--- Global functions ---
------------------------

-- ?
function showGUI(name, bHidePrevious, bShowCursor, ptr)
    gui = showGUI(name, bHidePrevious, bShowCursor)
    gui.overlay = ptr
end

-- Shows the specified menu sheet and loads it if neccessary
function showGUI(name, bHidePrevious, bShowCursor)
    -- Handle default value for bShowCursor
    if bShowCursor == nil then
        if activeMenuSheets.size > 0 then
            bShowCursor = activeMenuSheets.topSheet.bShowCursor
        else
            bShowCursor = true
        end
    end

    -- Hide if already displayed (to make sure it is up front in the end)
    if activeMenuSheets[name] ~= nil then
        hideGUI(name)
    end

    if not root then
        setBackground("")
    end

    -- Get sheet (or load it)
    local menuSheet = loadSheet(name)
    if not menuSheet then
        return
    end

    -- Add sheet to the root window
    root:addChildWindow(menuSheet.window)

    -- Pause game control if this is the first menu to be displayed
    -- HUGE HACK?
    if activeMenuSheets.size == 0 then
        orxonox.HumanController:pauseControl()
    end

    -- Handle input distribution
    orxonox.InputManager:getInstance():enterState(menuSheet.inputState)

    if bShowCursor then
        showCursor()
    else
        hideCursor()
    end

    -- Add the sheet in a tuple of additional information
    local sheetTuple =
    {
        ["menuSheet"]      = menuSheet,
        ["name"]           = name,
        ["bShowCursor"]    = bShowCursor,
        ["bHidePrevious"]  = bHidePrevious
    }
    table.insert(activeMenuSheets, sheetTuple) -- indexed array access
    activeMenuSheets[name] = sheetTuple -- name access
    activeMenuSheets.size = activeMenuSheets.size + 1
    activeMenuSheets.topSheet = sheetTuple

    -- Hide all previous sheets if necessary
    if bHidePrevious then
        for i = 1, activeMenuSheets.size - 1 do
            activeMenuSheets[i].menuSheet:hide()
        end
    end

    menuSheet:show()
    return menuSheet
end

function hideGUI(name)
    local sheetTuple = activeMenuSheets[name]
    if sheetTuple == nil then
        return
    end

    -- Hide the sheet
    sheetTuple.menuSheet:hide()

    -- Show sheets that were hidden by the sheet to be removed
    local i = activeMenuSheets.size
    -- Only do something if all sheets on top of sheetTuple
    -- have bHidePrevious == false and sheetTuple.bHidePrevious == true
    while i > 0 do
        if activeMenuSheets[i].bHidePrevious == true then
            if activeMenuSheets[i] == sheetTuple then
                i = i - 1
                while i > 0 do
                    activeMenuSheets[i].menuSheet:show()
                    if activeMenuSheets[i].bHidePrevious == true then
                        break
                    end
                    i = i - 1
                end
            end
            break
        end
        i = i - 1
    end

    -- Remove sheet with its tuple from the table
    root:removeChildWindow(sheetTuple.menuSheet.window)
    table.remove(activeMenuSheets, find(activeMenuSheets, sheetTuple))
    activeMenuSheets[name] = nil
    activeMenuSheets.size = activeMenuSheets.size - 1
    activeMenuSheets.topSheet = activeMenuSheets[activeMenuSheets.size]

    -- Leave the input state
    orxonox.InputManager:getInstance():leaveState(sheetTuple.menuSheet.inputState)
    
    -- See whether to show or hide cursor
    if activeMenuSheets.size > 0 and activeMenuSheets.topSheet.bShowCursor then
        showCursor()
    else
        hideCursor()
    end

    -- Resume control if the last menu is hidden
    if activeMenuSheets.size == 0 then
        orxonox.HumanController:resumeControl()
        hideCursor()
    end
end

-- Hides all menu GUI sheets
function hideAllGUIs()
    while activeMenuSheets.size ~= 0 do
        hideGUI(activeMenuSheets.topSheet.name)
    end
end

function keyESC()
    -- HUGE, very HUGE hacks!
    if activeMenuSheets.size == 1 and activeMenuSheets[1].name == "MainMenu" then
        orxonox.execute("exit")
    elseif activeMenuSheets.size > 0 then
        orxonox.execute("hideGUI "..activeMenuSheets.topSheet.name)
    else
        showGUI("InGameMenu")
    end
end

function setBackground(name)
    local newroot
    if root ~= nil then
        root:rename("oldRootWindow")
    end
    if name ~= "" then
        newroot = winMgr:loadWindowLayout(name .. ".layout")
        newroot:rename("AbsoluteRootWindow")
        system:setGUISheet(newroot)
    else
        newroot = winMgr:createWindow("DefaultWindow", "AbsoluteRootWindow")
        newroot:setProperty("Alpha", "0.0")
        newroot:setSize(CEGUI.UVector2(CEGUI.UDim(1.0,0),CEGUI.UDim(1.0,0)))
        system:setGUISheet(newroot)
    end
    if root ~= nil then
        local child
        while root:getChildCount()~=0 do
            child = root:getChildAtIdx(0)
            root:removeChildWindow(child)
            newroot:addChildWindow(child)
        end
        winMgr:destroyWindow(root)
    end
    newroot:show()
    root = newroot
end
