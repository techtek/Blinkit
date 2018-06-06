@echo off
  
:: Get the flashdrive letter set by the user in the Blinkit GUI, stored in \config\ and put it into variable %flashdrive%   
	set /p flashdrive=<config\drive.txt

:: Get the Blink length set by the user in the Blinkit GUI, stored in \config\ and put it into variable %blinklength%
	set /p blinklength=<config\blinklength.txt
	
:: Let the user know the led is going to be blinked by writing the text: 	  
	echo Blinking the LED

:: Blink the LED, by copying the LED file from the Blinkit folder to the flashdrive
	xcopy ledfile\ledfile%blinklength%.led %flashdrive%. /Y > nul    

	echo %blinklength% 
:: wait 3 second
	timeout 3

:: Close the program	
	exit