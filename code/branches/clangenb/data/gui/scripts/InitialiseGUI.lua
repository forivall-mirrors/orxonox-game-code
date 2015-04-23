-- Define some global shortcuts for common Managers
guiMgr    = orxonox.GUIManager:getInstance()
inputMgr  = orxonox.InputManager:getInstance()
schemeMgr = CEGUI.SchemeManager:getSingleton()
winMgr    = CEGUI.WindowManager:getSingleton()

local scheme = orxonox.CommandExecutor:query("getConfig GUIManager guiScheme_")
-- Load all required skins
--schemeMgr:create("TaharezGreenLook.scheme")
schemeMgr:create(scheme .. "Look.scheme")
--schemeMgr:create("TaharezLook.scheme")
--schemeMgr:create("WindowsLook.scheme")
--schemeMgr:create("VanillaLook.scheme")
--schemeMgr:create("SleekSpaceLook.scheme")

-- Connect skin specific window types with our own window types
-- By loading a different file (if there is) you can change the skin
-- of the menus or the HUD independently
--schemeMgr:create("TaharezGreenMenuWidgets.scheme")
--menuImageSet = "TaharezGreenLook"
--schemeMgr:create("TaharezGreenHUDWidgets.scheme")
--hudImageSet = "TaharezGreenLook"
schemeMgr:create(scheme .. "MenuWidgets.scheme")
menuImageSet = scheme .. "Look"
schemeMgr:create(scheme .. "HUDWidgets.scheme")
hudImageSet = scheme .. "Look"

-- Just a remaining test hack
schemeMgr:create("OrxonoxGUIScheme.scheme")

local system = CEGUI.System:getSingleton()
system:setDefaultMouseCursor(menuImageSet, "MouseArrow")
system:setDefaultFont("BlueHighway-12")
system:setDefaultTooltip("MenuWidgets/Tooltip")

-- Convenience function and additional tools
require("GUITools")
