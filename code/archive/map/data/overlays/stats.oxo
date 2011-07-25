<!-- Statistics Overlay -->
<Template name=defaultScoreboard>
    <OverlayGroup name = "Stats" scale = "1, 1" visible=false>

      <!--OverlayText
       name      = "Title"
       position  = "0.79, 0.19"
       pickpoint = "1, 1"
       font      = "Monofur"
       caption   = "Scoreboard"
       colour    = "0, 0.75, 0.2, 1"
       textsize  = 0.04
      /-->

      <OverlayText
       name      = "descr1"
       position  = "0.1, 0.20"
       pickpoint = "0, 1"
       font      = "Monofur"
       caption   = "player name"
       colour    = "0, 0.75, 0.2, 1"
       textsize  = 0.04
      />
      <OverlayText
       name      = "descr2"
       position  = "0.6, 0.20"
       pickpoint = "0, 1"
       font      = "Monofur"
       caption   = "score"
       colour    = "0, 0.75, 0.2, 1"
       textsize  = 0.04
      />
      <OverlayText
       name      = "descr3"
       position  = "0.8, 0.20"
       pickpoint = "0, 1"
       font      = "Monofur"
       caption   = "deaths"
       colour    = "0, 0.75, 0.2, 1"
       textsize  = 0.04
      />

      <!--Stats
       name     = "Statistics"
       position = "0.5, 0.7"
       size = "0.85, 0.4"
       correctaspect = false
       pickpoint = "0.5, 0.5"
      /-->

      <Scoreboard
       name     = "Scores"
      />

    </OverlayGroup>
</Template>
