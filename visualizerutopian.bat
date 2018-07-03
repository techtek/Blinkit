@echo off    
 
:: Get the saved device to use for the Utopian Upvote Bot Visualizer to blink
    set /p utopiandevice=<config\utopiandevice.txt 
	
:: Check if a sound notification is set On/Off,  stored in \config\ and put it into variable %soundsetting%
	set /p soundsetting=<config\soundsetting.txt	
	
:: Get the sound set by the user in the Blinkit GUI, stored in \config\ and put it into variable %sound%
	set /p sound=<config\sound.txt

:: Get the by the user saved logitech mode LED or LCD	
	set /p mode=<config\logitechmode.txt	
	
	
    
:: Colour settings
	set ESC=
	set Red=%ESC%[31m
	set White=%ESC%[37m
	set Green=%ESC%[32m
	set Magenta=%ESC%[35m
	set Blue=%ESC%[94m
   
:: Display welcome message to the user welcome.txt      
	type config\welcome.txt

:: Let the user know that Blinkit is going to watch for Upvotes by displaying the text:
	echo Blinkit - %Blue%Utopian Upvote Visualizer
	echo.
	echo %Magenta%Testing blink device...
	
:: Test sound

	:: Play the notification sound if turned on by the user 
	if %soundsetting%==On (start /min sounds\sound.vbs) else (echo Sound notifications are turned off) 
	echo.
	echo %White%
	timeout 3	


:: Test blink the selected device
	echo.
	echo %magenta%Preparing the Visualizer...
 
 


 	:: USB stick
IF "%utopiandevice%"=="usbstick" (
    call blink.bat
)




 	:: Logitech
IF "%utopiandevice%"=="logitech" (

	IF "%mode%"=="LED" (
    start /min logitechblink.exe
)

	IF "%mode%"=="LCD" (
    start /min logitechlcd.exe
)

	IF "%mode%"=="LED,LCD" (
    start /min logitechblink.exe
	start /min logitechlcd.exe
	
)
)



 	:: Webcam	
IF "%utopiandevice%"=="webcam" (
    (start bat\blinkwebcam.bat)
)


 	:: Sonoff
IF "%utopiandevice%"=="sonoff" (
    (start bat\blinksonoff.bat)
)


 	:: Philips Hue
IF "%utopiandevice%"=="philipshue" (
    (start blinkphilipshue.bat)
)


 	:: Arduino
IF "%utopiandevice%"=="arduino" (
    echo u u u > config\arduinocommand.txt
	(start arduinosendcommand.exe)
)



 
	
	
	
	
	

	
:: Blinkit - Utopian Upvote Bot Visualizer Script 

:: Download Upvotes data from Web Api and save it into a txt file
	powershell -Command "Invoke-WebRequest https://steem-sincerity.dapptools.info/s/api/account-outgoing-votes/utopian-io -OutFile config\downloadutopianvotes.txt"

:main   
:: Download Upvotes data from Web Api and save it into a txt file
	powershell -Command "Invoke-WebRequest https://steem-sincerity.dapptools.info/s/api/account-outgoing-votes/utopian-io -OutFile config\downloadutopianvotes2.txt"
    	
:: Compare the 2 downloaded follower txt files if different go to "notification", if the files are the same go to "next" 
    fc config\downloadutopianvotes.txt config\downloadutopianvotes2.txt > nul
	if errorlevel 1 goto notification 
	if errorlevel 0 goto next
		
		
:next
:: let the user know the program is running by displaying the text:  
	echo.
	echo %Magenta%Blinkit is running...
  	
:: Download new data to compare, into "downloadvotes2.txt", and go back to "main" to compare the files again
	powershell -Command "Invoke-WebRequest https://steem-sincerity.dapptools.info/s/api/account-outgoing-votes/utopian-io -OutFile config\downloadutopianvotes2.txt"
	goto main

    
:notification
:: Let the user know, that the Utopian Upvote Bot is active by displaying the text:   
	echo Blinkit is running... %Blue% Utopian Upvote bot is Upvoting!
	echo %White%  
	echo %Magenta%ACTION LED BLINKED!

:: Let the user know, that the Utopian Upvote Bot is active by blinking the Upvote led of the saved device
  
  
  
 	:: USB stick
IF "%utopiandevice%"=="usbstick" (
    call blink.bat
)


 	:: Logitech
IF "%utopiandevice%"=="logitech" (

	IF "%mode%"=="LED" (
    start /min logitechblink.exe
)

	IF "%mode%"=="LCD" (
    start /min logitechlcd.exe
)

	IF "%mode%"=="LED,LCD" (
    start /min logitechblink.exe
	start /min logitechlcd.exe
	
)
)


 	:: Webcam	
IF "%utopiandevice%"=="webcam" (
    (start bat\blinkwebcam.bat)
)


 	:: Sonoff
IF "%utopiandevice%"=="sonoff" (
    (start bat\blinksonoff.bat)
)


 	:: Philips Hue
IF "%utopiandevice%"=="philipshue" (
    (start blinkphilipshue.bat)
)



 	:: Arduino
IF "%utopiandevice%"=="arduino" (
    echo u u u > config\arduinocommand.txt
	(start arduinosendcommand.exe)
)


 

   
   
:: Play the notification sound if turned on by the user 
	if %soundsetting%==On (start /min sounds\sound.vbs) else (echo Sound notifications are turned off) 
	echo.
	timeout 3	
	echo %White%
	


:: Download new data to compare, and go back to "main" and continue to look for for new Upvotes.
	powershell -Command "Invoke-WebRequest https://steem-sincerity.dapptools.info/s/api/account-outgoing-votes/utopian-io -OutFile config\downloadutopianvotes.txt"	
	goto main