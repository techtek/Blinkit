@echo off
  
:: Get the flashdrive letter of USB drive 3, set by the user in the Blinkit GUI, stored in \config\ and put it into variable %flashdrive3%   
	set /p flashdrive3=<c:\blinkit\config\drive3.txt

:: Get the Blink length set by the user in the Blinkit GUI, stored in \config\ and put it into variable %blinklength3%
	set /p blinklength3=<c:\blinkit\config\blinklength3.txt
	
:: Let the user know the led is going to be blinked by writing the text: 	  
	echo Blinking the LED

:: Blink the LED, by copying the LED file from the Blinkit folder to the flashdrive
	xcopy c:\blinkit\ledfile\ledfile%blinklength3%.led %flashdrive3%. /Y > nul    

::	echo %blinklength3% 
	
:: wait 3 second
	:: timeout 3

:: Close the program	
	cls