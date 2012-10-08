<Template name="TetrisHUD">
  <OverlayGroup name="TetrisHUD" scale = "1, 1">
    <TetrisScore
     position  = "0.9, 0.1"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.05
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
    />

    <OverlayText
     position  = "0.7, 0.10"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.05
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     caption   = "Points: "
    />
    
    <OverlayText
     position  = "0.7, 0.20"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.05
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     caption   = "Next Brick"
    />
    
    

  </OverlayGroup>
</Template>
