@echo off 
   
   :: Check if a sound notification is set On/Off,  stored in \config\ and put it into variable %soundsetting%
	set /p soundsetting=<c:\blinkit\config\soundsetting.txt	
	
:: Get the sound set by the user in the Blinkit GUI, stored in \config\ and put it into variable %sound%
	set /p sound=<c:\blinkit\config\sound.txt
   
:: Play the sound if it is turned on
  
	set "file=C:\blinkit\sounds\%sound%"
	
	( echo Set Sound = CreateObject("WMPlayer.OCX.7"^)
	echo Sound.URL = "%file%"
	echo Sound.Controls.play
	echo do while Sound.currentmedia.duration = 0
	echo wscript.sleep 100
	echo loop
	echo wscript.sleep (int(Sound.currentmedia.duration^)+1^)*1000) >C:\blinkit\sounds\sound.vbs
	
	if %soundsetting%==On (start /min C:\blinkit\sounds\sound.vbs) else (echo Sound notifications are turned off) 
	

   


  





