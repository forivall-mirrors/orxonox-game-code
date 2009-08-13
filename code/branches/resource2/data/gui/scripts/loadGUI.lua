local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton()
local logger = CEGUI.Logger:getSingleton()
local system = CEGUI.System:getSingleton()

schemeMgr:loadScheme("TaharezLookSkin.scheme")
system:setDefaultMouseCursor("TaharezLook", "MouseArrow")
system:setDefaultFont("BlueHighway-12")


local rootSheet = winMgr:createWindow("DefaultGUISheet", "orxonox/Sheet")

local leftOffset = 0.11
local topOffset  = 0.3
local distance   = 0.076
local index      = 0

local standalone = winMgr:createWindow("TaharezLook/Button", "orxonox/StandaloneButton")
standalone:setText("Standalone")
standalone:setSize(CEGUI.UVector2(CEGUI.UDim(0.15, 0), CEGUI.UDim(0.05, 0)))
standalone:setPosition(CEGUI.UVector2(CEGUI.UDim(leftOffset, 0), CEGUI.UDim(topOffset + index * distance, 0)))
standalone:subscribeEvent("Clicked","button_standalone_clicked")
index = index + 1

local dedicated = winMgr:createWindow("TaharezLook/Button", "orxonox/DedicatedButton")
dedicated:setText("Dedicated")
dedicated:setSize(CEGUI.UVector2(CEGUI.UDim(0.15, 0), CEGUI.UDim(0.05, 0)))
dedicated:setPosition(CEGUI.UVector2(CEGUI.UDim(leftOffset, 0), CEGUI.UDim(topOffset + index * distance, 0)))
dedicated:subscribeEvent("Clicked","button_dedicated_clicked")
index = index + 1

local server = winMgr:createWindow("TaharezLook/Button", "orxonox/ServerButton")
server:setText("Server")
server:setSize(CEGUI.UVector2(CEGUI.UDim(0.15, 0), CEGUI.UDim(0.05, 0)))
server:setPosition(CEGUI.UVector2(CEGUI.UDim(leftOffset, 0), CEGUI.UDim(topOffset + index * distance, 0)))
server:subscribeEvent("Clicked","button_server_clicked")
index = index + 1

local client = winMgr:createWindow("TaharezLook/Button", "orxonox/ClientButton")
client:setText("Client")
client:setSize(CEGUI.UVector2(CEGUI.UDim(0.15, 0), CEGUI.UDim(0.05, 0)))
client:setPosition(CEGUI.UVector2(CEGUI.UDim(leftOffset, 0), CEGUI.UDim(topOffset + index * distance, 0)))
client:subscribeEvent("Clicked","button_client_clicked")
index = index + 1

local quit = winMgr:createWindow("TaharezLook/Button", "orxonox/QuitButton")
quit:setText("Quit")
quit:setSize(CEGUI.UVector2(CEGUI.UDim(0.15, 0), CEGUI.UDim(0.05, 0)))
quit:setPosition(CEGUI.UVector2(CEGUI.UDim(leftOffset, 0), CEGUI.UDim(topOffset + index * distance, 0)))
quit:subscribeEvent("Clicked","button_quit_clicked")
index = index + 1

local backgroundImage = CEGUI.ImagesetManager:getSingleton():createImagesetFromImageFile("GUI/Background", "main_menu_1.jpg")
local background = winMgr:createWindow("TaharezLook/StaticImage", "orxonox/Background")
background:setProperty("FrameEnabled", "set: true")
background:setProperty("BackgroundEnabled", "set: false")
background:setProperty("Image", "set: GUI/Background image:full_image")

rootSheet:addChildWindow(quit)
rootSheet:addChildWindow(standalone)
rootSheet:addChildWindow(server)
rootSheet:addChildWindow(dedicated)
rootSheet:addChildWindow(client)
background:addChildWindow(rootSheet)


function button_quit_clicked(e)
  hideGUI()
  orxonox.CommandExecutor:execute("exit")
end

function button_standalone_clicked(e)
  orxonox.CommandExecutor:execute("selectGameState standalone")
  hideGUI()
end

function button_server_clicked(e)
  orxonox.CommandExecutor:execute("selectGameState server")
  hideGUI()
end

function button_dedicated_clicked(e)
  orxonox.CommandExecutor:execute("selectGameState dedicated")
  hideGUI()
end

function button_client_clicked(e)
  orxonox.CommandExecutor:execute("selectGameState client")
  hideGUI()
end

showBackground = false

function showMainMenu()
  if showBackground == true then
    system:setGUISheet(background)
  else
    system:setGUISheet(rootSheet)
  end
  return 0;
end

function hideGUI()
  system:setGUISheet(nil)
  orxonox.GUIManager:getInstance():hideGUI()
end
