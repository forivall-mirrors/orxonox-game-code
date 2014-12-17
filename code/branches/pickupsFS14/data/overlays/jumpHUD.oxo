<Template name="JumpHUD">
  <OverlayGroup name="JumpHUD" scale = "1, 1">

    <JumpScore
     position  = "0.5, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.06
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "center"
     showScore = true
     showFuel = false
     showMessages = false
    />

    <JumpScore
     position  = "0.1, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.05
     colour    = "1.0, 0.5, 0.0, 1.0"
     align     = "left"
     showScore = false
     showFuel = true
     showMessages = false
    /> 

    <JumpScore
     position  = "0.5, 0.5"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.2
     colour    = "1.0, 0.0, 0.0, 1.0"
     align     = "center"
     gameOverText = "GAME OVER"
     showScore = false
     showFuel = false
     showMessages = true
    />       

  <HUDBar
     name          = "SpeedBar1"
     background    = "Orxonox/BarBackground"
     size          = "0.35, 0.05"
     position      = "0.0 , 1.0 "
     pickpoint     = "0, 1"
     correctaspect = false

     initialvalue = 50

     >
      <BarColour position = 0.0 colour = "0.7,0.5,0.2" />
      <BarColour position = 0.5 colour = "0.2,0.7,0.2" />
      <BarColour position = 1.0 colour = "0.7,0.2,0.2" />
    </HUDBar>

  </OverlayGroup>
</Template>
