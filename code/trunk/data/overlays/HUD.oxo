<!-- HUD OVERLAY -->
<OverlayGroup name = "HUD" scale = "1, 1">

  <HUDSpeedBar
   name       = "SpeedBar1"
   background = "Orxonox/BarBackground"
   size       = "0.35, 0.05"
   position   = "0.0 , 1.0 "
   pickPoint  = "0, 1"
   value      = 0
  >
    <BarColour position = 0.7 colour = "0.2,0.7,0.2" />
    <BarColour position = 0.4 colour = "0.7,0.5,0.2" />
    <BarColour position = 0.1 colour = "0.7,0.2,0.2" />
  </HUDSpeedBar>

  <!--HUDSpeedBar
   name       = "SpeedBar2"
   background = "Orxonox/BarBackground"
   size       = "0.35, 0.05"
   position   = "1.0 , 1.0 "
   pickPoint  = "1, 1"
   value      = 0
   rightToLeft = true
  >
    <BarColour position = 0.7 colour = "0.2,0.7,0.2" />
    <BarColour position = 0.4 colour = "0.7,0.5,0.2" />
    <BarColour position = 0.1 colour = "0.7,0.2,0.2" />
  </HUDSpeedBar>

  <HUDSpeedBar
   name       = "SpeedBar3"
   background = "Orxonox/BarBackground"
   size       = "0.35, 0.05"
   position   = "1.0 , 0.0 "
   pickPoint  = "1, 0"
   value      = 0
   rightToLeft = true
  >
    <BarColour position = 0.7 colour = "0.2,0.7,0.2" />
    <BarColour position = 0.4 colour = "0.7,0.5,0.2" />
    <BarColour position = 0.1 colour = "0.7,0.2,0.2" />
  </HUDSpeedBar>

  <HUDSpeedBar
   name       = "SpeedBar4"
   background = "Orxonox/BarBackground"
   size       = "0.35, 0.05"
   position   = "0.0 , 0.0 "
   pickPoint  = "0, 0"
   rotation   = 0
   value      = 0
  >
    <BarColounar position = 0.7 colour = "0.2,0.7,0.2" />
    <BarColour position = 0.4 colour = "0.7,0.5,0.2" />
    <BarColour position = 0.1 colour = "0.7,0.2,0.2" />
  </HUDSpeedBar-->

  <HUDNavigation
   name          = "Navigation"
   correctAspect = true
   font          = "Monofur"
   textSize      = 0.02
   navMarkerSize = 0.03
   aimMarkerSize = 0.02
  />

  <HUDRadar
   name          = "Radar"
   background    = "Orxonox/radar"
   correctAspect = true
   size          = "0.17, 0.17"
   position      = "0.5, 1.0"
   pickPoint     = "0.5, 1.0"
   rotation      = 0
   sensitivity   = 1.0
   halfDotSizeDistance = 3000
   maximumDotSize      = 0.1
  />

  <ChatOverlay
   name     = "chat"
   position = "0.03, 0.5"
   font     = "VeraMono"
   caption  = ""
   textSize = 0.025
  />
</OverlayGroup>


<!-- DEBUG OVERLAY -->
<OverlayGroup name = "Debug" scale = "1.0, 1.0" scroll = "0, 0" visible=false>

  <DebugFPSText
   name     = "FPSText"
   position = "0.03, 0.05"
   font     = "Monofur"
   caption  = "Frames per second: "
   textSize = 0.03
  />

  <DebugRTRText
   name     = "RTRText"
   position = "0.03, 0.09"
   font     = "Monofur"
   caption  = "Tick time in ms: "
   textSize = 0.03
  />
</OverlayGroup>
