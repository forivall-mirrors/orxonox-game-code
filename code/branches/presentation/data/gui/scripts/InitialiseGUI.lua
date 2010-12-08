-- Define some global shortcuts for common Managers
guiMgr    = orxonox.GUIManager:getInstance()
inputMgr  = orxonox.InputManager:getInstance()
schemeMgr = CEGUI.SchemeManager:getSingleton()
winMgr    = CEGUI.WindowManager:getSingleton()

-- Load all required skins
schemeMgr:loadScheme("TaharezGreenLook.scheme")
--schemeMgr:loadScheme("OrxonoxLook.scheme")
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
--schemeMgr:loadScheme("OrxonoxLookMenuWidgets.scheme")
--menuImageSet = "OrxonoxLook"
--schemeMgr:loadScheme("OrxonoxLookHUDWidgets.scheme")
--hudImageSet = "OrxonoxLook"

-- Just a remaining test hack
schemeMgr:loadScheme("OrxonoxGUIScheme.scheme")

local system = CEGUI.System:getSingleton()
system:setDefaultMouseCursor(menuImageSet, "MouseArrow")
system:setDefaultFont("BlueHighway-12")
system:setDefaultTooltip("MenuWidgets/Tooltip")

-- Convenience function and additional tools
require("GUITools")
