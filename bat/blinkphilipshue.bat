@echo off
  
	:: Get the Philips Blink length set by the user in the Blinkit GUI, stored in \config\ and put it into variable %blinklengthphilipshue%
	set /p blinklengthphilipshue=<bat\config\blinklengthphilipshue.txt
  

:: Blink the Philips Hue lights
	start  philipshuegreen.exe 
	timeout %blinklengthphilipshue%
	start  philipshueoff.exe  	
	
	
	

:: Close the program	
	cls
	
	
	
	
	