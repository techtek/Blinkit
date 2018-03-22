@echo off 
   
   	set /p sound=<c:\blinkit\config\sound.txt 
	set "file=C:\blinkit\sounds\%sound%"
	( echo Set Sound = CreateObject("WMPlayer.OCX.7"^)
	echo Sound.URL = "%file%"
	echo Sound.Controls.play
	echo do while Sound.currentmedia.duration = 0
	echo wscript.sleep 100
	echo loop
	echo wscript.sleep (int(Sound.currentmedia.duration^)+1^)*1000) >sound.vbs
	start /min sound.vbs
   
	:: Sound file created by Thoribass https://freesound.org/people/Thoribass/sounds/254819/

  





