-- SheetManager.lua

local cursor = CEGUI.MouseCursor:getSingleton()
local loadedSheets = {}
local activeMenuSheets = {size = 0, topSheetTuple = nil}
local menuSheetsRoot = guiMgr:getMenuRootWindow()
local bInGameConsoleClosed = false
local mainMenuLoaded = false
orxonox.GUIManager:subscribeEventHelper(menuSheetsRoot, "KeyDown", "keyPressed")
orxonox.GUIManager:subscribeEventHelper(menuSheetsRoot, "Sized", "windowResized")

------------------------
--- Global functions ---
------------------------

function hideCursor()
    if cursor:isVisible() then
        cursor:hide()
    end
end

function showCursor()
    if not cursor:isVisible() and inputMgr:isMouseExclusive() then
        cursor:show()
    end
end

-- Loads the GUI with the specified name
-- The name corresponds to the filename of the *.lua and *.layout files
-- but without the extension
function loadSheet(name)
    -- Check if it has already been loaded
    local sheet = loadedSheets[name]
    if sheet == nil then
        -- Load the sheet
        sheet = require(name)
        sheet:load()
        loadedSheets[name] = sheet
    end
    return sheet
end

-- ?
function showMenuSheet(name, bHidePrevious, bNoInput, ptr)
    local sheet = showMenuSheet(name, bHidePrevious, bNoInput)
    sheet.overlay = ptr
    return sheet
end

-- Shows the specified menu sheet and loads it if neccessary
function showMenuSheet(name, bHidePrevious, bNoInput)
    if name == "" then
        return nil
    end
    -- Get sheet (or load it)
    local menuSheet = loadSheet(name)

    -- Use sheet's value if nil was provided
    if bHidePrevious == nil then
        bHidePrevious = menuSheet.bHidePrevious
        assert(bHidePrevious ~= nil)
    end

    -- Set bNoInput to false if it hasn't been set.
    if bNoInput == nil then
        bNoInput = false
    end

    -- Count the number of sheets that don't need input till the first that does.
    local counter = noInputSheetIndex()
    -- Pause game control if this is the first menu to be displayed
    -- HUGE HACK?
    if bNoInput == false and counter == 0 then
        orxonox.HumanController:pauseControl()
    end

    -- Hide if already displayed (to make sure it is up front in the end)
    if activeMenuSheets[name] ~= nil then
        hideMenuSheet(name)
    end

    if bNoInput == true then
        menuSheet.tShowCursor = TriBool.Dontcare
    end

    -- Add the sheet in a tuple of additional information
    local sheetTuple =
    {
        ["sheet"]          = menuSheet,
        ["bHidePrevious"]  = bHidePrevious,
        ["bNoInput"]       = bNoInput,
        ["name"]           = name
    }
    table.insert(activeMenuSheets, sheetTuple) -- indexed array access
    activeMenuSheets[name] = sheetTuple -- name access
    activeMenuSheets.size = activeMenuSheets.size + 1
    activeMenuSheets.topSheetTuple = sheetTuple

    -- Add sheet to the root window
    menuSheetsRoot:addChildWindow(menuSheet.window)

    -- If sheet is the MainMenu
    if name == "MainMenu" then
        mainMenuLoaded = true
    end

    -- Handle input distribution
    if bNoInput == false then
        inputMgr:enterState(menuSheet.inputState)
    end

    -- Only change cursor situation if menuSheet.tShowCursor ~= TriBool.Dontcare
    if menuSheet.tShowCursor == TriBool.True then
        showCursor()
    elseif menuSheet.tShowCursor == TriBool.False then
        hideCursor()
    end

    -- Hide all previous sheets if necessary
    local previous
    if bHidePrevious then
        for i = 1, activeMenuSheets.size - 1 do
            previous = activeMenuSheets[i].sheet
            previous:hide()
        end
    end

    menuSheet:show()
    menuSheetsRoot:activate()

    -- select first button if the menu was opened with the keyboard
    if previous and previous.pressedEnter and menuSheet:hasSelection() == false then
        menuSheet:setSelectionNear(1, 1)
    end

    return menuSheet
end

function hideMenuSheet(name)
    local sheetTuple = activeMenuSheets[name]
    if sheetTuple == nil then
        return
    end

    -- Hide the sheet
    sheetTuple.sheet:hide()

    -- Show sheets that were hidden by the sheet to be removed
    local i = activeMenuSheets.size
    -- Only do something if all sheets on top of sheetTuple
    -- have bHidePrevious == true and sheetTuple.bHidePrevious == true
    while i > 0 do
        if activeMenuSheets[i].bHidePrevious then
            if activeMenuSheets[i] == sheetTuple then
                i = i - 1
                while i > 0 do
                    activeMenuSheets[i].sheet:show()
                    if activeMenuSheets[i].bHidePrevious then
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
    menuSheetsRoot:removeChildWindow(sheetTuple.sheet.window)
    table.remove(activeMenuSheets, table.findIndex(activeMenuSheets, sheetTuple))
    activeMenuSheets[name] = nil
    activeMenuSheets.size = activeMenuSheets.size - 1
    activeMenuSheets.topSheetTuple = activeMenuSheets[activeMenuSheets.size]

    -- If sheet is the MainMenu
    if name == "MainMenu" then
        mainMenuLoaded = false
    end

    -- Leave the input state
    if not sheetTuple.bNoInput then
        inputMgr:leaveState(sheetTuple.sheet.inputState)
    end

    -- CURSOR SHOWING
    local i = activeMenuSheets.size
    -- Find top most sheet that doesn't have tShowCusor == TriBool.Dontcare
    while i > 0 and activeMenuSheets[i].sheet.tShowCursor == TriBool.Dontcare do
        i = i - 1
    end
    if i > 0 and activeMenuSheets[i].sheet.tShowCursor == TriBool.True then
        showCursor()
    else
        hideCursor()
    end

    -- Count the number of sheets that don't need input till the first that does.
    local counter = noInputSheetIndex()
    -- Resume control if the last (non-noInput) menu is hidden
    if counter == 0 then
        orxonox.HumanController:resumeControl()
        hideCursor()
    end

    sheetTuple.sheet:quit()
end

-- Hides all menu GUI sheets
function hideAllMenuSheets()
    while activeMenuSheets.size ~= 0 do
        hideMenuSheet(activeMenuSheets.topSheetTuple.sheet.name)
    end
end

function keyESC()
    -- HUGE, very HUGE hacks!

    -- If the InGameConsole is active, ignore the ESC command.
    if bInGameConsoleClosed == true then
        bInGameConsoleClosed = false
        if activeMenuSheets[1] and activeMenuSheets[1].sheet.name == "MainMenu" then
            return
        end
    end

    -- Count the number of sheets that don't need input till the first that does.
    local counter = noInputSheetIndex()

    -- If the first sheet that needs input is the MainMenu.
    if noInputSheetCounter() == 1 and activeMenuSheets[counter].sheet.name == "MainMenu" then
        orxonox.execute("exit")
    -- If there is at least one sheet that needs input.
    elseif counter > 0 then
        orxonox.execute("hideGUI "..activeMenuSheets[counter].sheet.name)
    else
        showMenuSheet("InGameMenu")
    end
end

function keyPressed(e)
    local we = tolua.cast(e, "CEGUI::KeyEventArgs")
    local sheet = activeMenuSheets[activeMenuSheets.size]
    code = tostring(we.scancode)
    -- Some preprocessing
    if not mainMenuLoaded and not sheet.bNoInput then
        if code == "1" then
            keyESC()
        elseif code == "0"then
            orxonox.CommandExecutor:execute("InGameConsole openConsole")
        end
    end
    sheet.sheet:keyPressed()
end

function windowResized(e)
    for name, sheet in pairs(loadedSheets) do
        if orxonox.GraphicsManager:getInstance():isFullScreen() or sheet.tShowCursor == TriBool.False then
            inputMgr:setMouseExclusive(sheet.inputState, TriBool.True)
        else
            inputMgr:setMouseExclusive(sheet.inputState, TriBool.False)
        end
    end
    local sheetTuple = activeMenuSheets[activeMenuSheets.size]
    if sheetTuple then
        if orxonox.GraphicsManager:getInstance():isFullScreen() and sheetTuple.sheet.tShowCursor ~= TriBool.False then
            showCursor()
        else
            hideCursor()
        end
        sheetTuple.sheet:windowResized()
    end
end

function setBackgroundImage(imageSet, imageName)
    guiMgr:setBackgroundImage(imageSet, imageName)
end

function noInputSheetIndex()
    -- Count the number of sheets that don't need input till the first that does.
    local index = activeMenuSheets.size
    while index > 0 and activeMenuSheets[index].bNoInput do
        index = index - 1
    end
    return index
end

function noInputSheetCounter()
    -- Count the number of sheets that do need input.
    local counter = activeMenuSheets.size
    for i = 1,activeMenuSheets.size do
        if activeMenuSheets[i].bNoInput then
            counter = counter - 1
        end
    end
    return counter
end

function inGameConsoleClosed()
    bInGameConsoleClosed = not bInGameConsoleClosed;
end

function getGUIFirstActive(name, bHidePrevious, bNoInput)
    local sheet = activeMenuSheets.topSheetTuple
    -- If the topmost gui sheet has the input name
    if sheet ~= nil and sheet.name == name then
        guiMgr:toggleGUIHelper(name, bHidePrevious, bNoInput, false);
    else
        guiMgr:toggleGUIHelper(name, bHidePrevious, bNoInput, true);
    end
end

----------------------
--- Initialisation ---
----------------------

hideCursor()
