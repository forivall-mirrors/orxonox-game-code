<Template name="spectatorhud">
  <OverlayGroup name = "spectatorhud" scale = "1, 1">
  </OverlayGroup>
</Template>

<Template name="DodgeRaceHUD">
  <OverlayGroup name="DodgeRaceHUD" scale = "1, 1">

    <OverlayText
     position  = "0.02, 0.02"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "Points: "
    />

    <DodgeRaceHUDinfo
     position  = "0.14, 0.02"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showpoints     = true
    />

    
  </OverlayGroup>
</Template>

<Template name="spaceshiphud">
  <OverlayGroup name = "spaceshiphud" scale = "1, 1">
    
 </OverlayGroup>
</Template>