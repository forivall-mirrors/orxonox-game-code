<Template name="lastTeamStandingHUD">
  <OverlayGroup name="lastTeamStandingHUD" scale = "1, 1">
    <OverlayText
     position  = "0.02, 0.02"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "Lives:   "
    />

    <LastTeamStandingInfos
     position  = "0.14, 0.02"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showlives     = true
     showteams     = false
    />

    <OverlayText
     position  = "0.02, 0.055"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "Teams: "
    />

    <LastTeamStandingInfos
     position  = "0.14, 0.055"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showlives     = false
     showteams     = true
    />

  </OverlayGroup>
</Template>
