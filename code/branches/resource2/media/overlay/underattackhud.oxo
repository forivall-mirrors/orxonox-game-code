<Template name="UnderAttackHUD">
  <OverlayGroup name="UnderAttackHUD" scale = "1, 1">
    <UnderAttackHealthBar
     name                 = "HealthBar"
     background           = "Orxonox/HealthBarBackground"
     size                 = "0.45, 0.0875"
     position             = "0.99 , 0.01"
     pickpoint            = "1, 0"
     bartexture           = "healthbar_bar.tga"
     textfont             = "VeraMono"
     textusebarcolour     = true
     textsize             = 0.039
     textoffset           = "0.315, 0.05"
     textpickpoint        = "0, 0"
     textalign            = "right"
     correctaspect        = false
     textcorrectaspect    = false
     descriptionpickpoint = "0, 0"
     descriptionoffset    = "0.5, 1"
    >
      <BarColour position = 0.0 colour = "0.7,0.2,0.2" />
      <BarColour position = 0.5 colour = "0.7,0.7,0.2" />
      <BarColour position = 1.0 colour = "0.2,0.7,0.2" />
    </UnderAttackHealthBar>
  </OverlayGroup>
</Template>
