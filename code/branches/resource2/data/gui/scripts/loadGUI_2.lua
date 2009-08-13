local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()
local logger = CEGUI.Logger:getSingleton()
local system = CEGUI.System:getSingleton()
local cursor = CEGUI.MouseCursor:getSingleton()

schemeMgr:loadScheme("TaharezLookSkin.scheme")
-- load scheme with our own images
schemeMgr:loadScheme("OrxonoxGUIScheme.scheme")

system:setDefaultMouseCursor("TaharezLook", "MouseArrow")
system:setDefaultFont("BlueHighway-12")

local current = nil
local loadedGUIs = {}
-- a bit more complex: GUI is now a class
GUI = {}
GUI.__index = GUI

-- hide function for the GUI
GUI.hide = function (self)
    self.window:hide()
end

-- show function for the GUI
GUI.show = function (self)
    self.window:show()
end

-- constructor of the GUI
GUI.new = function (gui, fname)
    local newElement = { window = gui, filename = fname }
    setmetatable(newElement, GUI) -- connects new element with class
    return newElement
end

datapath = "" -- points to media-folder (set after loading the script)

-- loads the GUI with the specified filename
-- be sure to set the global variable "filename" before calling this function
function loadGUI(filename)
    -- check if it already exists
    --gui = loadedGUIs:getGUIbyName(filename)
    gui = loadedGUIs[filename]
    if gui == nil then
        dofile(datapath .. "gui/scripts/" .. filename .. ".lua")
        win = winMgr:loadWindowLayout(layoutPath)
        gui = GUI.new(win, filename)
        loadedGUIs[filename] = gui
        -- if there has no GUI been loaded yet, set new GUI as current
        if table.getn(loadedGUIs) == 1 then
            current = loadedGUIs[1]
            showing = false
        end
        -- hide new GUI as we do not want to show it accidentially
        gui:hide()
    end
    return gui
end

-- shows the specified and loads it if not loaded already
-- be sure to set the global variable "filename" before calling this function
function showGUI(filename)
    if current == nil or current.filename ~= filename then
        if current ~= nil then
        end
        current = loadedGUIs[filename]
        if current == nil then
            current = loadGUI(filename)
        end
        system:setGUISheet(current.window)
    end
    current:show()
    showing = true
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
