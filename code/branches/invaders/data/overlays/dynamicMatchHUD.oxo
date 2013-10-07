<Template name="DynamicmatchHUD">
  <OverlayGroup name="DynamicmatcHUD" scale = "1, 1">
   
    <!--OverlayText
     position  = "0.02, 0.02"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "Target:"
    />

    <OverlayText
     position  = "0.02, 0.055"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.04
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     caption   = "Score:"
    /-->


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

  </OverlayGroup>
</Template>
