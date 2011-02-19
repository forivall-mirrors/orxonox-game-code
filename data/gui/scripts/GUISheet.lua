-- GUISheet.lua

local P = {}
_G[_REQUIREDNAME or "GUISheet"] = P
P.__index = P

-- Don't use directly --> use HUDSheet.new or MenuSheet.new
function P.new(_name)
    local newSheet = { name = _name }
    setmetatable(newSheet, P)
    return newSheet
end

-- Override this function if you need to do work on load
function P:onLoad()
end

-- Override this function if you need to do work on show
function P:onShow()
end

-- Override this function if you need to do work on hide
function P:onHide()
end

-- Override this function if you need to do work just after the sheet has been hidden
function P:onAfterHide()
end

-- show function for the GUI
function P:show()
    self.window:show()
    self.bVisible = true

    -- set the selected button's state
    if self.buttons and self:hasSelection() then
        self:setButtonStateSelected()
    end

    self:onShow()
end

-- hide function for the GUI
function P:hide()
    self.window:hide()
    self.bVisible = false

    self:onHide()
end

function P:afterHide()
    -- reset the selected button
    if self.buttons then
        self:resetSelection()
    end

    self:onAfterHide()
end

function P:load()
    -- Load the layout that describes the sheet
    self.window = winMgr:loadWindowLayout(self.name .. ".layout")
    if self.window == nil then
        error("Could not load layout file for GUI sheet '"..self.name.."'")
    end
    -- Hide it at first
    self:hide()
    -- Allow sheets to do some work upon loading
    self:onLoad()

    -- Also load additional sheets to avoid display lags
    if self.loadAlong then
        for k, sheet in pairs(self.loadAlong) do
            loadSheet(sheet)
        end
    end
    return self
end

-- Handles key pressed while the gui sheed is displayed
function P:keyPressed()
    if self.buttons then
        if code == "208" then     -- key down
            self:moveSelection(1, 0)
        elseif code == "200" then -- key up
            self:moveSelection(-1, 0)
        elseif code == "205" then -- key right
            self:moveSelection(0, 1)
        elseif code == "203" then -- key left
            self:moveSelection(0, -1)
        elseif code == "28"  then -- key enter
            self:pressSelectedButton()
        end
    end

    self.onKeyPressed()
end

-- Override this function if you want to ract on keystrokes
function P:onKeyPressed()
end


-------------------------------------------------------------------------------
-- Keyboard control -----------------------------------------------------------
-------------------------------------------------------------------------------

-- Initializes the buttons table, used to control the menu with the keyboard
function P:initButtons(rows, columns)
    self.rows = rows
    self.columns = columns
    self.buttons = {}
    self.selectedRow = 0
    self.selectedColumn = 0
end

-- Defines the button for a given position in the table. The upper-left button is at position (1, 1)
function P:setButton(row, column, button)
    assert(self.rows ~= nil and self.columns ~= nil and self.buttons ~= nil, "You have to call initButtons() before using setButton()")
    assert(row > 0 and column > 0 and row <= self.rows and column <= self.columns, "(" .. row .. "/" .. column .. ") is not in the valid bounds of the table (1/1)-(" .. self.rows .. "/" .. self.columns .. ")")

    self.buttons[(row - 1) * self.columns + (column - 1)] = button
end

-- Returns the button at a given position in the table. The upper-left button is at position (1, 1)
function P:getButton(row, column)
    assert(row > 0 and column > 0 and row <= self.rows and column <= self.columns, "(" .. row .. "/" .. column .. ") is not in the valid bounds of the table (1/1)-(" .. self.rows .. "/" .. self.columns .. ")")

    return self.buttons[(row - 1) * self.columns + (column - 1)]
end

-- Returns the selected button
function P:getSelectedButton()
    assert(self.selectedRow > 0 and self.selectedColumn > 0, "no button selected")

    return self:getButton(self.selectedRow, self.selectedColumn)
end

-- Presses the selected button if any
function P:pressSelectedButton()
    if self:hasSelection() then
        self:getSelectedButton().callback()
    end
end

-- Sets the selection to a given row and column. The upper-left button is at position (1, 1)
function P:setSelection(row, column)
    assert(row > 0 and column > 0 and row <= self.rows and column <= self.columns, "(" .. row .. "/" .. column .. ") is not in the valid bounds of the table (1/1)-(" .. self.rows .. "/" .. self.columns .. ")")

    if self:hasSelection() then
        self:setButtonStateNormal()
    end

    self.selectedRow = row
    self.selectedColumn = column

    self:setButtonStateSelected()
end

-- Moves the selection by a given number of rows and columns (positive values mean increasing row/column, negative values mean decreasing row/column)
function P:moveSelection(relRow, relColumn)
    -- if there's no selection yet, prepare it such that the selection enters the table from the desired side
    if self:hasSelection() == false then
        -- note: we assume either relRow or relColumn is 0, thus no diagonal movement. therefore the following checks can be separated
        if relRow > 0 then
            self.selectedRow = 0
            self.selectedColumn = 1
        elseif relRow < 0 then
            self.selectedRow = self.rows + 1
            self.selectedColumn = 1
        end

        if relColumn > 0 then
            self.selectedRow = 1
            self.selectedColumn = 0
        elseif relColumn < 0 then
            self.selectedRow = 1
            self.selectedColumn = self.columns + 1
        end
    else
        self:setButtonStateNormal()
    end

    -- move the selection according to the parameters
    self.selectedRow = self.selectedRow + relRow
    self.selectedColumn = self.selectedColumn + relColumn

    -- wrap around on overflow
    while self.selectedRow > self.rows do
        self.selectedRow = self.selectedRow - self.rows
    end
    while self.selectedColumn > self.columns do
        self.selectedColumn = self.selectedColumn - self.columns
    end

    -- wrap around on underflow
    while self.selectedRow <= 0 do
        self.selectedRow = self.selectedRow + self.rows
    end
    while self.selectedColumn <= 0 do
        self.selectedColumn = self.selectedColumn + self.columns
    end

    -- if the button is deactivated, call this function again
    if self:getSelectedButton() == nil then
        self:moveSelection(relRow, relColumn)
    else
        self:setButtonStateSelected()
    end
end

-- Resets the selection
function P:resetSelection()
    if self:hasSelection() then
        self:setButtonStateNormal()
    end

    self.selectedRow = 0
    self.selectedColumn = 0
end

-- Determines if a button is selected
function P:hasSelection()
    if self.selectedRow == 0 or self.selectedColumn == 0 then
        return false
    else
        return true
    end
end

-- Sets the selected button's state to normal
function P:setButtonStateNormal()
    self:setButtonState("Normal")
end

-- Sets the selected button's state to selected
function P:setButtonStateSelected()
    self:setButtonState("Selected")
end

-- Sets the selected button's state to pushed
function P:setButtonStatePushed()
    self:setButtonState("Pushed")
end

-- Sets the selected button's state
function P:setButtonState(state)
    if self:getSelectedButton() then
        local element = self:getSelectedButton().button
        local offset = getElementStateOffset(element)

        if offset then
            element:setProperty("NormalImageRightEdge",  string.sub(element:getProperty("NormalImageRightEdge"),  1, offset) .. state)
            element:setProperty("NormalImageLeftEdge",   string.sub(element:getProperty("NormalImageLeftEdge"),   1, offset) .. state)
            element:setProperty("NormalImageBackground", string.sub(element:getProperty("NormalImageBackground"), 1, offset) .. state)
        end
    end
end

-- Gets the offset of the button's current state
function getElementStateOffset(element)
    local property = element:getProperty("NormalImageRightEdge")

    if string.sub(property, string.len(property) - 5, string.len(property)) == "Normal" then
        return -7
    elseif string.sub(property, string.len(property) - 7, string.len(property)) == "Selected" then
        return -9
    elseif string.sub(property, string.len(property) - 5, string.len(property)) == "Pushed" then
        return -7
    else
        return nil
    end
end

return P
