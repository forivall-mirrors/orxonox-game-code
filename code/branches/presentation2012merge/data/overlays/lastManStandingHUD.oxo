<Template name="LastmanstandingHUD">
  <OverlayGroup name="LastmanstandingHUD" scale = "1, 1">
    <OverlayText
     position  = "0.02, 0.02"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "Lives:   "
    />

    <LastManStandingInfos
     position  = "0.14, 0.02"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showlives     = true
     showplayers     = false
    />

    <OverlayText
     position  = "0.02, 0.055"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "Players: "
    />

    <LastManStandingInfos
     position  = "0.14, 0.055"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showlives     = false
     showplayers     = true
    />

    <!--OverlayText
     position  = "0.02, 0.07"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "__________|"
    />

    <OverlayText
     position  = "0.022, 0.07"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "__________"
    />

    <OverlayText
     position  = "0.1733, 0.038"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "|"
    /-->

  </OverlayGroup>
</Template>
