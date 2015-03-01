<!--
<Template name="defaultHUD">
  <OverlayGroup name = "defaultHUD" scale = "1, 1">
    <ChatOverlay
     name     = "chat"
     position = "0.03, 0.5"
     font     = "VeraMono"
     caption  = ""
     textsize = 0.025
    />

    <GametypeStatus
     name     = "state"
     position = "0.5, 0.85"
     font     = "VeraMono"
     textsize = 0.05
     colour   = "1.0, 1.0, 0.0, 0.8"
     align    = "center"
    />
  </OverlayGroup>
</Template>
-->

<Template name="spectatorhud">
  <OverlayGroup name = "spectatorhud" scale = "1, 1">
  </OverlayGroup>
</Template>

<Template name="spaceshiphud">
  <OverlayGroup name = "spaceshiphud" scale = "1, 1">
    <Map />

    <HUDHealthBar
     name              = "HealthBar1"
     background        = "Orxonox/HealthBarBackground"
     size              = "0.35, 0.0875"
     position          = "0.0 , 0.9 "
     pickpoint         = "0, 1"
     bartexture        = "healthbar_bar.png"
     textfont          = "VeraMono"
     textusebarcolour  = true
     textsize          = 0.039
     textoffset        = "0.315, 0.05"
     textpickpoint     = "0, 0"
     textalign         = "right"
     correctaspect     = false
     textcorrectaspect = false
    >
      <BarColour position = 0.0 colour = "0.7,0.2,0.2" />
      <BarColour position = 0.5 colour = "0.7,0.7,0.2" />
      <BarColour position = 1.0 colour = "0.2,0.7,0.2" />
    </HUDHealthBar>

    <HUDSpeedBar
     name          = "SpeedBar1"
     background    = "Orxonox/BarBackground"
     size          = "0.35, 0.05"
     position      = "0.0 , 0.95 "
     pickpoint     = "0, 1"
     correctaspect = false
    >
      <BarColour position = 0.0 colour = "0.7,0.5,0.2" />
      <BarColour position = 0.5 colour = "0.2,0.7,0.2" />
      <BarColour position = 1.0 colour = "0.7,0.2,0.2" />
    </HUDSpeedBar>

    <HUDBoostBar
     name          = "BoostBar1"
     backgroundtex = "bar1.png"
     size          = "0.35, 0.05"
     position      = "0.0 , 1.0 "
     pickpoint     = "0, 1"
     correctaspect = false
    >
      <BarColour position = 0.0 colour = "0.7,0.2,0.2" />
      <BarColour position = 0.5 colour = "0.7,0.7,0.2" />
      <BarColour position = 1.0 colour = "0.2,0.7,0.2" />
    </HUDBoostBar>

    <HUDEnemyHealthBar
     name              = "EnemyHealthBar"
     background        = "Orxonox/HealthBarBackground"
     size              = "0.35, 0.0875"
     position          = "1 ,0.1"
     pickpoint         = "1, 1"
     bartexture        = "healthbar_bar.png"
     textfont          = "VeraMono"
     textusebarcolour  = true
     textsize          = 0.039
     textoffset        = "0.315, 0.05"
     textpickpoint     = "0, 0"
     textalign         = "right"
     correctaspect     = false
     textcorrectaspect = false
    >
      <BarColour position = 0.0 colour = "0.7,0.2,0.2" />
      <BarColour position = 0.5 colour = "0.7,0.7,0.2" />
      <BarColour position = 1.0 colour = "0.2,0.7,0.2" />
    </HUDEnemyHealthBar>

    <HUDNavigation
     name          = "Navigation"
     correctaspect = true
     font          = "Monofur"
     textsize      = 0.02
     navMarkerSize = 0.03
     aimMarkerSize = 0.02
    />

	<HUDRadar
     name          			= "Radar"
     background    			= "Orxonox/Radar3D"
     material2D    			= "Orxonox/Radar"
     material3DMiddle   	= "Orxonox/Radar3D"
     material3DFront		= "Orxonox/Radar3DFront"
     material3DBack			= "Orxonox/Radar3DBack"
     correctaspect 			= true
     size          			= "0.17, 0.17"
     position      			= "1.0, 1.0"
     pickpoint     			= "1.0, 1.0"
     rotation      			= 0
     sensitivity   			= 1.0
     halfDotSizeDistance 	= 3000
     detectionLimit 		= 10000.0
     maximumDotSize 		= 0.1
     maximumDotSize3D      	= 0.06
     mapAngle3D   			= 0.6435011
    />
    
    <HUDTimer
     name     = "Timer"
     position = "0.5, 0.85"
     font     = "VeraMono"
     textsize = 0.1
     colour   = "1.0, 1.0, 0.0, 0.8"
     align    = "center"
    />

    <GUIOverlay
      name = "QuestGUI"
      guiname = "QuestGUI"
      visible = "false"
    />
    
    <GUIOverlay
      name = "PickupInventory"
      guiname = "PickupInventory"
      visible = "false"
    />

  </OverlayGroup>
</Template>
