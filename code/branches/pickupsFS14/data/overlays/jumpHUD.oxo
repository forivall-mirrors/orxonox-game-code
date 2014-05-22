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
     showMessages = true
    />    

  </OverlayGroup>
</Template>
