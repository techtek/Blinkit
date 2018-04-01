@echo off
  
	:: Get the Philips Blink length set by the user in the Blinkit GUI, stored in \config\ and put it into variable %blinklengthphilipshue%
	set /p blinklengthphilipshue=<c:\blinkit\config\blinklengthphilipshue.txt
  

:: Blink the Philips Hue lights
	start  C:\blinkit\bat\philipshuegreen.exe 
	timeout %blinklengthphilipshue%
	start  C:\blinkit\bat\philipshueoff.exe  	
	

:: Close the program	
	cls
	
	
	
	
	