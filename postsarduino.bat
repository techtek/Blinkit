@echo off    
   
:: Take and put the username from username.txt and put it in variable %username%
	set /p steemusername=<config\username.txt
 
:: Get the com and baud rate set by the user in the Blinkit GUI, stored in \config\ and put it into variable %com% %baud% 
	set /p com=<config\arduinocomport.txt
	set /p baud=<config\arduinobaudrate.txt
	
:: Check if a sound notification is set On/Off,  stored in \config\ and put it into variable %soundsetting%
	set /p soundsetting=<config\soundsetting.txt	
	
:: Get the sound set by the user in the Blinkit GUI, stored in \config\ and put it into variable %sound%
	set /p sound=<config\sound.txt
	
    
:: Colour settings
	set ESC=
	set Red=%ESC%[31m
	set White=%ESC%[37m
	set Green=%ESC%[32m
	set Magenta=%ESC%[35m
	set Blue=%ESC%[94m
   
:: Display welcome message to the user welcome.txt      
	type config\welcomearduino.txt



:: Let the user know that Blinkit is going to watch for Posts by displaying the text:
	echo Blinkit - Steem Account Posts

:: Let the user know a sound is being played by displaying the text:
    echo.
	echo %Magenta%Testing %White%Play sound...
	
:: Play and test Sound notification	
	set /p sound=<config\sound.txt 
	set "file=sounds\%sound%"
	( echo Set Sound = CreateObject("WMPlayer.OCX.7"^)
	echo Sound.URL = "%file%"
	echo Sound.Controls.play
	echo do while Sound.currentmedia.duration = 0
	echo wscript.sleep 100
	echo loop
	echo wscript.sleep (int(Sound.currentmedia.duration^)+1^)*1000) >sounds\sound.vbs
	
	if %soundsetting%==On (start /min sounds\sound.vbs) else (echo Sound notifications are turned off) 
	echo.
	
:: Let the user know that led 3 is going to be blinked, on the arduino, and display the com and baudrate:  	  
	echo %Magenta%Arduino%White% %com% baudrate %baud%
	echo.

:: Test Blink the LED, by sending the u command to the Arduino
	echo %Magenta%Testing %White%Blink LED 
	echo p> config/arduinocommand.txt
	start arduinosendcommand.exe  
	echo.


::  Download the latest STEEM and SBD Price and put it inside a txt files
	powershell -Command "Invoke-WebRequest https://api.coinmarketcap.com/v1/ticker/steem/ -OutFile config\steemprice.txt"
	powershell -Command "Invoke-WebRequest https://api.coinmarketcap.com/v1/ticker/steem-dollars/ -OutFile config\sbdprice.txt"

:: Find and display the latest STEEM and SBD Price in USD from the downloaded txt files
	echo Steem Price
	for /F "delims=" %%a in ('findstr /I ""price_usd"" config\steemprice.txt') do set "batToolDir0=%%a"
	for /F "delims=" %%a in ('findstr /I ""price_usd"" config\sbdprice.txt') do set "batToolDir2=%%a"
	echo %Blue%Steem: 				"%batToolDir0%"
	echo %Blue%Steem Dollar: 			"%batToolDir2%"

:: Update the STEEM and SBD price in USD into txt files for the steem prices in the Blinkit interface
	echo "%batToolDir0%" > "config\steempricestriped.txt"
	echo "%batToolDir2%" > "config\sbdpricestriped.txt"
	echo.

:: Display Steem username
	echo %White%Steem Username 
	echo %Blue%%steemusername%%White%
	echo.
	  
:: Download the number of Followers from a user inside a txt file
	powershell -Command "Invoke-WebRequest https://api.steem.place/getFollowersCount/?a=%steemusername% -OutFile config\displayfollowers.txt"
		
:: Display the downloaded followers	
	echo %White%Followers%Blue% 
	type config\displayfollowers.txt 
	echo.

:: Display the saved Username and let the user know that the program is starting
	echo.
	echo %White%Preparing BlinkIt, 
	echo Notifications for %Blue%%steemusername%%White% on Arduino ...	  
	timeout 4 

	
:: Blinkit Posts Script 

:: Download the number of posts from a steem user, from the Web Api and save it into a txt file
	powershell -Command "Invoke-WebRequest https://api.steem.place/getPostCount/?a=%steemusername% -OutFile config\downloadposts.txt"
	
:main   


:: Download the number of posts from a steem user, from the Web Api and save it into a txt file
	powershell -Command "Invoke-WebRequest https://api.steem.place/getPostCount/?a=%steemusername% -OutFile config\downloadposts2.txt"
    

:: Compare the 2 downloaded posts txt files if different go to "notification", if the files are the same go to "next"  
    fc config\downloadposts.txt config\downloadposts2.txt > nul
	if errorlevel 1 goto notification 
	if errorlevel 0 goto next
..		
		
:next
:: let the user know the program is running by displaying the text:   
	echo.
	echo %Magenta%Blinkit is running...
	
:: Download new data to compare, into "download2.txt", and go back to "main" to compare the files again
	powershell -Command "Invoke-WebRequest https://api.steem.place/getPostCount/?a=%steemusername% -OutFile config\downloadposts2.txt" 
	goto main

	
:notification
:: Let the user know, there is a new Post! by displaying the text:  
	echo Blinkit is running... %Blue%NEW Post from %steemusername%!
	echo %White%
	echo %Magenta%ACTION LED BLINKED! 
  
:: Let the user know, there is a new Post, and blink the Post Led
	echo p> config/arduinocommand.txt
	start arduinosendcommand.exe 
	
	
:: Play the notification sound if turned on by the user 
	if %soundsetting%==On (start /min sounds\sound.vbs) else (echo Sound notifications are turned off) 
	echo.
	timeout 3	
	echo %White% 

:: Download new data to compare, and go back to "main" and continue to look for for new posts.
	powershell -Command "Invoke-WebRequest https://api.steem.place/getPostCount/?a=%steemusername% -OutFile config\downloadposts.txt"	
	goto main