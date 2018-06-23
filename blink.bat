@echo off 
   
:: Get the flashdrive letter set by the user in the Blinkit GUI, stored in \config\ and put it into variable %flashdrive% 
	set /p flashdrive=<config\drive.txt
    set /p numberofblinks=<config\numberofblinks.txt
  
echo Blinking the LED on Flashdrive %flashdrive%
  
  
:: Blink the by the user saved Flashdrive with the number saved by the user into    
	for /l %%x in (1, 1, %numberofblinks%) do (
   
    xcopy ledfile\ledfile.led %flashdrive%. /Y > nul

	)