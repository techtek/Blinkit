@echo off
  
:: Get the flashdrive letter of USB drive 2, set by the user in the Blinkit GUI, stored in \config\ and put it into variable %flashdrive2%   
	set /p flashdrive2=<c:\blinkit\config\drive2.txt

:: Get the Blink length set by the user in the Blinkit GUI, stored in \config\ and put it into variable %blinklength2%
	set /p blinklength2=<c:\blinkit\config\blinklength2.txt
	
:: Let the user know the led is going to be blinked by writing the text: 	  
	echo Blinking the LED

:: Blink the LED, by copying the LED file from the Blinkit folder to the flashdrive
	xcopy c:\blinkit\ledfile\ledfile%blinklength2%.led %flashdrive2%. /Y > nul    

	echo %blinklength2% 
	
:: wait 3 second
	:: timeout 3

:: Close the program	
	cls