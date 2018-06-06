@echo off
  
:: Get the IP or Hostname set by the user in the Blinkit GUI, stored in \config\sonoffip.txt and put it into variable %ip%   
	set /p ip=<config\sonoffip.txt
	
:: Get the Blink length set by the user in the Blinkit GUI, stored in \config\ and put it into variable %blinklengthsonoff%
	set /p blinklengthsonoff=<config\blinklengthsonoff.txt

:: Let the user know the Sonoff Switch is going to blink by writing the text: 	  
	echo Blinking the Sonoff Smart Switch on IP / Hostname: %ip%
	echo Blink length set in Blinkit: %blinklengthsonoff% seconds
	echo. 
	echo Sending the request to Sonoff switch %ip%

:: Toggle the Sonoff Switch by giving the toggle command over the URL with the %IP% set by the user
	powershell -noprofile -command Invoke-WebRequest -Uri "http://%ip%/cm?cmnd=Power%%20TOGGLE"

:: Wait for X seconds, get the seconds from the blink length slider variable %blinklengthsonoff% 
	timeout %blinklengthsonoff%

:: Toggle the Sonoff Switch back to the previous state by giving the toggle command over the URL with the IP set by the user %ip%
	powershell -noprofile -command Invoke-WebRequest -Uri "http://%ip%/cm?cmnd=Power%%20TOGGLE"

:: Close the program	
	cls
	
	exit
	
	
	
	
	