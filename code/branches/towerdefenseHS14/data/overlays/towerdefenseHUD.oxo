<Template name="TowerDefenseHUD">


  <OverlayGroup name="TowerDefenseHUD" scale = "1, 1">
  
  
    <OverlayText
     position  = "0.83, 0.05"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.05
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     caption = "Cash:"
    />
    
    
     <TowerDefenseHUDController
     position  = "0.88, 0.10"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.05
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
		showlives = false
		showcredits = true
		showwaves = false
    />
    
        <OverlayText
     position  = "0.83, 0.22"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.05
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     caption = "Lives:"
    />
    
         <TowerDefenseHUDController
     position  = "0.88, 0.27"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.05
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
		showlives = true
		showcredits = false
		showwaves = false
    />
    
            <OverlayText
     position  = "0.83, 0.39"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.05
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"

     caption = "Waves:"
    />
    
         <TowerDefenseHUDController
     position  = "0.88, 0.44"
     pickpoint = "0.0, 0.0"
     font      = "ShareTechMono"
     textsize  = 0.05
     colour    = "1.0, 1.0, 1.0, 1.0"
     align     = "left"
     	showlives = false
		showcredits = false
		showwaves = true

    />
    
    
  </OverlayGroup>
</Template>


