<Template name="LastmanstandingHUD">
  <OverlayGroup name="DynamicmatcHUD" scale = "1, 1">
   
 <GametypeStaticMessage
   name     = "staticmessage"
   position = "0.5, 0.01"
   font     = "VeraMono"
   caption  = ""
   textsize = 0.033
   colour   = "1.0, 1.0, 0.5, 1.0"
   align    = "center"
  />

<GametypeFadingMessage
     name     = "fadingmessage"
     position = "0.5, 0.05"
     font     = "VeraMono"
     textsize = 0.04
     colour   = "1.0, 1.0, 0.5, 1.0"
     align    = "center"
    />
    <!--LastManStandingInfos
     position  = "0.63, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "0.8, 0.8, 1.0, 1.0"
     align     = "right"

     showlives     = true
     showplayers     = false
    /-->

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
     caption   = "Players: "
    />

    <OverlayText
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
    />

  </OverlayGroup>
</Template>
