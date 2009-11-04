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

-- loads the GUI with the specified filename
-- be sure to set the global variable "filename" before calling this function
function loadGUI(filename)
    -- check if it already exists
    loadedGui = loadedGUIs[filename]
    if loadedGui == nil then
        loadedGuiNS = require(filename)
        loadedGui = loadedGuiNS:load()
        loadedGUIs[filename] = loadedGui
        -- if there has no GUI been loaded yet, set new GUI as current
        if table.getn(loadedGUIs) == 1 then
            current = loadedGUIs[1]
            showing = false
        end
        -- hide new GUI as we do not want to show it accidentially
        loadedGui:hide()
    end
    return loadedGui
end

function showGUI(filename, bCursorVisible, ptr)
    gui = showGUI(filename, bCursorVisible)
    gui.overlay = ptr
end

-- shows the specified and loads it if not loaded already
-- be sure to set the global variable "filename" before calling this function
function showGUI(filename, bCursorVisible)
--     bCursorVisibile=false
    if bCursorVisible == nil then
        cursorVisibility= true
    end

    if root == nil then
        root = winMgr:createWindow("TaharezLook/StaticImage", "AbsoluteRootWindow")
        root:setProperty("Alpha", "0.0")
        root:setSize(CEGUI.UVector2(CEGUI.UDim(1.0,0),CEGUI.UDim(1.0,0)))
        system:setGUISheet(root)
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
    cursorVisibility[filename]=bCursorVisible
    
    nrOfActiveSheets = nrOfActiveSheets + 1
    table.insert(activeSheets, filename)
    activeSheets[nrOfActiveSheets] = filename
    
    currentGUI:show()
    showing = true
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
    root:removeChildWindow(currentGUI.window)
    showing = false
    i=1
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
end
