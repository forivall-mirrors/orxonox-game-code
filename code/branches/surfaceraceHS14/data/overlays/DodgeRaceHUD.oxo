<Template name="spectatorhud">
  <OverlayGroup name = "spectatorhud" scale = "1, 1">
  </OverlayGroup>
</Template>

<Template name="DodgeRaceHUD">
  <OverlayGroup name="DodgeRaceHUD" scale = "1, 1">
    <DodgeRaceHUDinfo
     position  = "0.14, 0.02"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showlives     = true
     showlevel     = false
     showpoints     = false
    />
    <OverlayText
     position  = "0.02, 0.02"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "Lives:   "
    />

    <OverlayText
     position  = "0.02, 0.055"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "Level: "
    />

    <DodgeRaceHUDinfo
     position  = "0.14, 0.055"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showlives     = false
     showlevel     = true
     showpoints     = false
    />

    <OverlayText
     position  = "0.02, 0.1"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "Points: "
    />

    <DodgeRaceHUDinfo
     position  = "0.14, 0.1"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showlives     = false
     showlevel     = false
     showpoints     = true
    />

    <DodgeRaceHUDinfo
     position  = "0.25, 0.1"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showMultiplier = true
    />
  </OverlayGroup>
</Template>


<Template name="spaceshiphud">
  <OverlayGroup name = "spaceshiphud" scale = "1, 1">
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
 </OverlayGroup>
</Template>