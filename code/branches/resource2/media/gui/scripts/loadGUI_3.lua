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

-- datapath is set before executing the script and points to media-folder
package.path = package.path .. ";" .. datapath .. "gui/scripts/?.lua"

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

function showGUI(filename, ptr)
    gui = showGUI(filename)
    gui.overlay = ptr
end

-- shows the specified and loads it if not loaded already
-- be sure to set the global variable "filename" before calling this function
function showGUI(filename)
    if current == nil or current.filename ~= filename then
        current = loadedGUIs[filename]
        if current == nil then
            current = loadGUI(filename)
        end
        system:setGUISheet(current.window)
    end
    current:show()
    showing = true
    return current
end

function toggleGUI()
    if showing == true then
        current:hide()
        cursor:hide()
        showing = false
    else
        current:show()
        cursor:show()
        showing = true
    end
    return showing
end

function hideCursor()
    cursor:hide()
end

function showCursor()
    cursor:show()
end

function hideGUI(filename)
    current = loadedGUIs[filename]
    if current ~= nil then
        current:hide()
        showing = false
    end
end
