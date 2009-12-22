local schemeMgr = CEGUI.SchemeManager:getSingleton()
winMgr = CEGUI.WindowManager:getSingleton()
local logger = CEGUI.Logger:getSingleton()
local system = CEGUI.System:getSingleton()
local cursor = CEGUI.MouseCursor:getSingleton()

schemeMgr:loadScheme("TaharezLookSkin.scheme")
-- load scheme with our own images
schemeMgr:loadScheme("OrxonoxGUIScheme.scheme")

system:setDefaultMouseCursor("TaharezLook", "MouseArrow")
system:setDefaultFont("BlueHighway-12")
system:setDefaultTooltip("TaharezLook/Tooltip")

loadedGUIs = {}
cursorVisibility = {}
activeSheets = {}
nrOfActiveSheets = 0
root = nil
bShowsCursor = false
bHidePrevious = {}

-- Require all tools
require("GUITools")

-- loads the GUI with the specified filename
-- be sure to set the global variable "filename" before calling this function
function loadGUI(filename)
    -- check if it already exists
    loadedGui = loadedGUIs[filename]
    if loadedGui == nil then
        loadedGuiNS = require(filename)
        if loadedGuiNS == nil then
            return
        end
        loadedGui = loadedGuiNS:load()
        loadedGUIs[filename] = loadedGui
        -- if there has no GUI been loaded yet, set new GUI as current
        if table.getn(loadedGUIs) == 1 then
            current = loadedGUIs[1]
        end
        -- hide new GUI as we do not want to show it accidentially
        loadedGui:hide()
    end
    return loadedGui
end

function showGUI(filename, hidePrevious, bCursorVisible, ptr)
    gui = showGUI(filename, hidePrevious, bCursorVisible)
    gui.overlay = ptr
end

-- shows the specified and loads it if not loaded already
-- be sure to set the global variable "filename" before calling this function
function showGUI(filename, hidePrevious, bCursorVisible)
    if bCursorVisible == nil then
        if nrOfActiveSheets > 0 then
            bCursorVisible = cursorVisibility[activeSheets[nrOfActiveSheets]]
        else
            bCursorVisible = true
        end
    end

    if root == nil then
        setBackground("")
    end

    local currentGUI = loadedGUIs[filename]
    if(currentGUI == nil) then
        currentGUI = loadGUI(filename)
    end

    if(root:isChild(currentGUI.window)) then
        root:removeChildWindow(currentGUI.window)
    end
    root:addChildWindow(currentGUI.window)

    if bCursorVisible then
        showCursor()
    else
        hideCursor()
    end

    if find( activeSheets, filename ) ~= nil then
        table.remove( activeSheets, find( activeSheets, filename ) )
        nrOfActiveSheets = nrOfActiveSheets - 1
    else
        if nrOfActiveSheets == 0 then
            orxonox.InputManager:getInstance():enterState("guiMouseOnly")
            orxonox.HumanController:pauseControl()
        end
    end
    nrOfActiveSheets = nrOfActiveSheets + 1
    table.insert(activeSheets, filename)
    activeSheets[nrOfActiveSheets] = filename
    bHidePrevious[filename]=hidePrevious
    cursorVisibility[filename] = bCursorVisible

    if hidePrevious == true then
        for i=1,nrOfActiveSheets-1 do
            loadedGUIs[ activeSheets[i] ]:hide()
        end
    end
    currentGUI:show()
    return currentGUI
end

function hideCursor()
    if bShowsCursor==true then
        bShowsCursor=false
        cursor:hide()
    end
end

function showCursor()
    if bShowsCursor==false then
        bShowsCursor=true
        cursor:show()
    end
end

function hideGUI(filename)
    local currentGUI = loadedGUIs[filename]
    if currentGUI == nil then
        return
    end
    currentGUI:hide()
    if bHidePrevious[filename] == true then
        local i = nrOfActiveSheets-1
        while i>0 do
            loadedGUIs[ activeSheets[i] ]:show()
            if bHidePrevious[filename]==true then
                break
            else
                i=i-1
            end
        end
    end
    root:removeChildWindow(currentGUI.window)
    local i=1
    while activeSheets[i] do
        if activeSheets[i+1] == nil then
            if activeSheets[i-1] ~= nil then
                if cursorVisibility[ activeSheets[i-1] ] == true then
                    showCursor()
                else
                    hideCursor()
                end
            else
                hideCursor()
            end
        end
        if activeSheets[i] == filename then
            table.remove( activeSheets, i )
            nrOfActiveSheets = nrOfActiveSheets-1
        else
            i = i+1
        end
    end
    cursorVisibility[filename] = nil -- remove the cursor visibility of the current gui from the table
    bHidePrevious[filename] = nil
    if nrOfActiveSheets == 0 then
        orxonox.InputManager:getInstance():leaveState("guiMouseOnly")
        orxonox.HumanController:resumeControl()
        hideCursor()
    end
end

function hideAllGUIs()
    while nrOfActiveSheets ~= 0 do
        hideGUI(activeSheets[nrOfActiveSheets])
    end
end

function keyESC()
    if nrOfActiveSheets == 1 and activeSheets[1] == "MainMenu" then
        orxonox.execute("exit")
    elseif nrOfActiveSheets > 0 then
        orxonox.execute("hideGUI "..activeSheets[nrOfActiveSheets])
    else
        showGUI("InGameMenu")
    end
end

function setBackground(filename)
    local newroot
    if root ~= nil then
        root:rename("oldRootWindow")
    end
    if filename ~= "" then
        newroot = winMgr:loadWindowLayout(filename .. ".layout")
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

function find(table, value)
    local i=0
    while table[i] ~= nil do
        if table[i]==value then
            return i
        else
            i=i+1
        end
    end
    return nil
end

function test(e)
    debug(0, "Blubb")
end
