<Template name="PongHUD">
  <OverlayGroup name="PongHUD" scale = "1, 1">
    <PongScore
     position  = "0.05, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.06
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showleftplayer  = true
     showrightplayer = false
     showname        = true
     showscore       = false
    />

    <PongScore
     position  = "0.49, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.06
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "right"

     showleftplayer  = true
     showrightplayer = false
     showname        = false
     showscore       = true
    />

    <OverlayText
     position  = "0.505, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.06
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "center"

     caption   = ":"
    />

    <PongScore
     position  = "0.51, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.06
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     showleftplayer  = false
     showrightplayer = true
     showname        = false
     showscore       = true
    />

    <PongScore
     position  = "0.95, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.06
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "right"

     showleftplayer  = false
     showrightplayer = true
     showname        = true
     showscore       = false
    />
  </OverlayGroup>
</Template>
