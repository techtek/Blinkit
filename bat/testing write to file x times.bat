@echo off 
   
      :: Get the flashdrive letter set by the user in the Blinkit GUI, stored in \config\ and put it into variable %flashdrive% 
	set /p flashdrive=<c:\blinkit\config\drive.txt
	set /p flashdrive2=<c:\blinkit\config\drive2.txt
	set /p flashdrive3=<c:\blinkit\config\drive3.txt
	

	set "file=C:\blinkit\sounds\sound1.wav"
( echo Set Sound = CreateObject("WMPlayer.OCX.7"^)
  echo Sound.URL = "%file%"
  echo Sound.Controls.play
  echo do while Sound.currentmedia.duration = 0
  echo wscript.sleep 100
  echo loop
  echo wscript.sleep (int(Sound.currentmedia.duration^)+1^)*1000) >sound.vbs
start /min sound.vbs
	
	:: Sound file created by Thoribass https://freesound.org/people/Thoribass/sounds/254819/

   
   
for /l %%x in (1, 1, 20) do (
   echo %%x Blinking the LED on Flashdrive %flashdrive%, %flashdrive2%, %flashdrive3%
   start /min sound.vbs
   xcopy c:\blinkit\ledfile\ledfile0.led %flashdrive3%. /Y > nul
   timeout 1 
   xcopy c:\blinkit\ledfile\ledfile0.led %flashdrive2%. /Y > nul
   timeout 1
   xcopy c:\blinkit\ledfile\ledfile0.led %flashdrive%. /Y > nul
   timeout 1
)












pause