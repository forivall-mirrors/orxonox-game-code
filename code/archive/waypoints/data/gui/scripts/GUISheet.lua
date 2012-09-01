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

-- Override this function if you need to do work on quit
function P:onQuit()
end

-- Override this function if you want to react on keystrokes
function P:onKeyPressed()
end

-- Override this function if you want to update the gui after the window was resized
function P:onWindowResized()
end

-- show function for the GUI
function P:show()
    self.window:show()
    self.bVisible = true

    -- set the selected button's state
    self:setSelectedButtonsStateToSelected()

    self:onShow()
end

-- hide function for the GUI
function P:hide()
    self.window:hide()
    self.bVisible = false

    self:onHide()
end

function P:quit()
    -- reset the selected button
    if self.buttons then
        self:resetSelection()
    end

    self:onQuit()
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
    if self.loadAlong and orxonox.GUIManager:preloadMenuSheets() then
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
            self:moveSelectionRow(1)
        elseif code == "200" then -- key up
            self:moveSelectionRow(-1)
        elseif code == "205" then -- key right
            self:moveSelectionColumn(1)
        elseif code == "203" then -- key left
            self:moveSelectionColumn(-1)
        elseif code == "28" or code == "156"  then -- key enter or key numpad enter
            self:pressSelectedButton()
        end
    end

    self:onKeyPressed()
end

function P:windowResized()
    self:onWindowResized()
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
    self.ratio = 1
end

-- ratio: the button's with divided by the button's height (used to calculate distance between buttons - adjust this until you get the desired behavior)
function P:setRatio(ratio)
    self.ratio = ratio
end

-- Defines the button for a given position in the table. The upper-left button is at position (1, 1)
function P:setButton(row, column, button)
    if not self.buttons then
        -- init the table
        self:initButtons(row, column)
    elseif row > self.rows or column > self.columns then
        -- rearrange the table
        local maxRows = math.max(self.rows, row)
        local maxColumns = math.max(self.columns, column)

        for r = self.rows, 1, -1 do
            for c = self.columns, 1, -1 do
                local b = self:getButton(r, c)
                if b then
                    self.buttons[(r - 1) * self.columns + (c - 1)] = nil
                    self.buttons[(r - 1) * maxColumns + (c - 1)] = b
                end
            end
        end

        self.rows = maxRows
        self.columns = maxColumns
    end

    self.buttons[(row - 1) * self.columns + (column - 1)] = button
end

-- Returns the button at a given position in the table. The upper-left button is at position (1, 1)
function P:getButton(row, column)
    if self.buttons then
        return self.buttons[(row - 1) * self.columns + (column - 1)]
    else
        return nil
    end
end

-- Returns the selected button
function P:getSelectedButton()
    if self:hasSelection() then
        return self:getButton(self.selectedRow, self.selectedColumn)
    else
        return nil
    end
end

-- Presses the selected button if any
function P:pressSelectedButton()
    if self:getSelectedButton() then
        self.pressedEnter = true
        self:getSelectedButton().callback()
        self.pressedEnter = false
    end
end

-- Sets the selection to a given row and column. The upper-left button is at position (1, 1)
function P:setSelection(row, column)
    if not self.buttons then
        return
    end

    assert(row > 0 and column > 0 and row <= self.rows and column <= self.columns, "(" .. row .. "/" .. column .. ") is not in the valid bounds of the table (1/1)-(" .. self.rows .. "/" .. self.columns .. ")")

    self:setSelectedButtonsStateToNormal()

    self.selectedRow = row
    self.selectedColumn = column

    self:setSelectedButtonsStateToSelected()
end

-- Sets the selection to the button closest to the given row and column. The upper-left button is at position (1, 1)
function P:setSelectionNear(row, column)
    if not self.buttons then
        return
    end

    assert(row > 0 and column > 0 and row <= self.rows and column <= self.columns, "(" .. row .. "/" .. column .. ") is not in the valid bounds of the table (1/1)-(" .. self.rows .. "/" .. self.columns .. ")")

    if self:getButton(row, column) then
        self:setSelection(row, column)
    else
        local min = 1000000
        local minRow, minColumn

        for r = 1, self.rows do
            for c = 1, self.columns do
                if self:getButton(r, c) then
                    local distance = math.sqrt((row - r)^2 + ((column - c) * self.ratio)^2)
                    if distance < min then
                        min = distance; minRow = r; minColumn = c
                    end
                end
            end
        end

        if minRow and minColumn then
            self:setSelection(minRow, minColumn)
        else
            self:resetSelection()
        end
    end
end

-- Moves the selection by a given number of rows (a positive value means down, a negative value means up)
function P:moveSelectionRow(relRow)
    self:moveSelection(relRow, "selectedRow", "selectedColumn", "rows", "columns", true)
end

-- Moves the selection by a given number of columns (a positive value means right, a negative value means left)
function P:moveSelectionColumn(relColumn)
    self:moveSelection(relColumn, "selectedColumn", "selectedRow", "columns", "rows", false)
end

-- Generic move function, the values are determined at runtime depending on the arguments
function P:moveSelection(relMove, selectedThis, selectedOther, limitThis, limitOther, isRow)
    if not self.buttons then
        return
    end

    -- if there's no selection yet, prepare it such that the selection enters the table from the desired side
    if self.selectedRow > 0 or self.selectedColumn > 0 then
        self:setSelectedButtonsStateToNormal()
    else
        if relMove > 0 then
            self[selectedThis] = 0
            self[selectedOther] = 1
        elseif relMove < 0 then
            self[selectedThis] = self[limitThis] + 1
            self[selectedOther] = 1
        else
            return
        end
    end

    -- move the selection according to the parameters
    self[selectedThis] = self[selectedThis] + relMove

    -- wrap around on overflow or underflow
    while self[selectedThis] > self[limitThis] do self[selectedThis] = self[selectedThis] - self[limitThis] end
    while self[selectedThis] <= 0              do self[selectedThis] = self[selectedThis] + self[limitThis] end

    -- if the button is deactivated, search the button closest to the desired location
    if self:getSelectedButton() == nil then
        local min = 1000000
        local minV1, minV2
        local limit, step

        if relMove > 0 then
            limit = self[limitThis]
            step = 1
        else
            limit = 1
            step = -1
        end

        for v1 = self[selectedThis], limit, step do
            for v2 = 1, self[limitOther] do
                local button
                if isRow == true then
                    button = self:getButton(v1, v2)
                else
                    button = self:getButton(v2, v1)
                end
                if button then
                    local distance
                    if isRow == true then
                        distance = math.sqrt((self[selectedThis] - v1)^2 + ((self[selectedOther] - v2) * self.ratio)^2)
                    else
                        distance = math.sqrt(((self[selectedThis] - v1) * self.ratio)^2 + (self[selectedOther] - v2)^2)
                    end
                    if distance < min then
                        min = distance; minV1 = v1; minV2 = v2
                    end
                end
            end
        end

        if minV1 and minV2 then
            self[selectedThis] = minV1
            self[selectedOther] = minV2
        elseif self:hasButtons() then
            -- no suitable button found - wrap around and search again
            if relMove > 0 then
                self[selectedThis] = 0
            else
                self[selectedThis] = self[limitThis] + 1
            end
            self:moveSelection(relMove, selectedThis, selectedOther, limitThis, limitOther, isRow)
        end
    end

    self:setSelectedButtonsStateToSelected()
end

-- Resets the selection
function P:resetSelection()
    self:setSelectedButtonsStateToNormal()

    self.selectedRow = 0
    self.selectedColumn = 0
end

-- Checks if there's at least one button in the table
function P:hasButtons()
    local count = 0
    for r = 1, self.rows do
        for c = 1, self.columns do
            if self:getButton(r, c) then
                count = count + 1
            end
        end
    end

    return (count > 0)
end

-- Determines if a button is selected
function P:hasSelection()
    if self.selectedRow and self.selectedRow > 0 and self.selectedColumn and self.selectedColumn > 0 then
        return true
    else
        return false
    end
end

-- Sets the selected button's state to normal
function P:setSelectedButtonsStateToNormal()
    self:setSelectedButtonsState("Normal")
end

-- Sets the selected button's state to selected
function P:setSelectedButtonsStateToSelected()
    self:setSelectedButtonsState("Selected")
end

-- Sets the selected button's state to pushed
function P:setSelectedButtonsStateToPushed()
    self:setSelectedButtonsState("Pushed")
end

-- Sets the selected button's state
function P:setSelectedButtonsState(state)
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
