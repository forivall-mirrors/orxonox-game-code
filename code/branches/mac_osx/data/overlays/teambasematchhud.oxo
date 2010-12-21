<Template name="TeamBaseMatchHUD">
  <OverlayGroup name="TeamBaseMatchHUD" scale = "1, 1">
    <OverlayText
     position  = "0.2, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 0.5, 0.5, 1.0"
     align     = "left"
     caption   = "Red team"
    />

    <TeamBaseMatchScore
     position  = "0.37, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 0.8, 0.8, 1.0"
     align     = "left"

     showleftteam  = true
     showrightteam = false
     showbases     = true
     showscore     = false
    />

    <TeamBaseMatchScore
     position  = "0.49, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 0.8, 0.8, 1.0"
     align     = "right"

     showleftteam  = true
     showrightteam = false
     showbases     = false
     showscore     = true
    />

    <OverlayText
     position  = "0.505, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "0.8, 0.8, 0.8, 1.0"
     align     = "center"

     caption   = ":"
    />

    <TeamBaseMatchScore
     position  = "0.51, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "0.8, 0.8, 1.0, 1.0"
     align     = "left"

     showleftteam  = false
     showrightteam = true
     showbases     = false
     showscore     = true
    />

    <TeamBaseMatchScore
     position  = "0.63, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "0.8, 0.8, 1.0, 1.0"
     align     = "right"

     showleftteam  = false
     showrightteam = true
     showbases     = true
     showscore     = false
    />

    <OverlayText
     position  = "0.8, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "0.5, 0.5, 1.0, 1.0"
     align     = "right"
     caption   = "Blue team"
    />
  </OverlayGroup>
</Template>
