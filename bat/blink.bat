  @echo off
  
:: Get the flashdrive letter set by the user in the Blinkit GUI, stored in /config/ and put it into variable %flashdrive%   
set /p flashdrive=<c:\blinkit\config\drive.txt

:: Let the user know the led is going to be blinked by writing the text: 	  
echo Blinking the LED

:: Blink the LED, by copying the LED file from the Blinkit folder to the flashdrive
xcopy c:\blinkit\ledfile\ledfile20MB.led %flashdrive%. /Y > nul    

:: wait 3 second
timeout 3

:: Close the program	
cls
   
   
   