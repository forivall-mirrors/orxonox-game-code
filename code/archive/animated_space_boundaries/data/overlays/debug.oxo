<!-- DEBUG OVERLAY -->
<OverlayGroup
 name    = "Debug"
 scale   = "1.0, 1.0"
 scroll  = "0, 0"
 visible = false
>
  <OverlayText
   name     = "FPSTextCaption"
   position = "0.03, 0.05"
   font     = "Monofur"
   caption  = "Frames per second:"
   textsize = 0.03
  />

  <DebugFPSText
   name     = "FPSText"
   position = "0.3, 0.05"
   font     = "Monofur"
   textsize = 0.03
  />

  <OverlayText
   name     = "RTRTextCaption"
   position = "0.03, 0.09"
   font     = "Monofur"
   caption  = "Tick time in ms: "
   textsize = 0.03
  />

  <DebugRTRText
   name     = "RTRText"
   position = "0.3, 0.09"
   font     = "Monofur"
   textsize = 0.03
  />

</OverlayGroup>

<Template name="defaultHUD">
  <OverlayGroup name = "defaultHUD" scale = "1, 1">
    <ChatOverlay
     name     = "chat"
     position = "0.03, 0.08"
     font     = "VeraMono"
     caption  = ""
     textsize = 0.025
    />

    <KillMessage
     name     = "killmessage"
     position = "0.5, 0.15"
     font     = "VeraMono"
     textsize = 0.035
     colour   = "1.0, 1.0, 0.5, 1.0"
     align    = "center"
    />

    <DeathMessage
     name     = "deathmessage"
     position = "0.5, 0.20"
     font     = "VeraMono"
     textsize = 0.035
     colour   = "1.0, 0.3, 0.3, 1.0"
     align    = "center"
    />

    <PauseNotice
     name     = "pausenotice"
     position = "0.5, 0.1"
     font     = "VeraMono"
     textsize = 0.03
     colour   = "1.0, 1.0, 1.0, 1.0"
     align    = "center"
    />

    <AnnounceMessage
     name     = "announcemessage"
     position = "0.5, 0.75"
     font     = "VeraMono"
     textsize = 0.05
     colour   = "0.5, 0.5, 1.0, 1.0"
     align    = "center"
    />

  </OverlayGroup>
</Template>
