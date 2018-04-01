@echo off    
   
:: Take and put the username from username.txt and put it in variable %username%
	set /p steemusername=<c:\blinkit\config\username.txt
 
:: Get the Philips hue IP set by the user in the Blinkit GUI, stored in \config\ and put it into variable %iphue% 
	set /p iphue=<c:\blinkit\config\philipshueip.txt


:: Get the Philips Blink length set by the user in the Blinkit GUI, stored in \config\ and put it into variable %blinklengthphilipshue%
	set /p blinklengthphilipshue=<c:\blinkit\config\blinklengthphilipshue.txt
	
:: Check if a sound notification is set On/Off,  stored in \config\ and put it into variable %soundsetting%
	set /p soundsetting=<c:\blinkit\config\soundsetting.txt	
	
:: Get the sound set by the user in the Blinkit GUI, stored in \config\ and put it into variable %sound%
	set /p sound=<c:\blinkit\config\sound.txt
    
:: Colour settings
	set ESC=
	set Red=%ESC%[31m
	set White=%ESC%[37m
	set Green=%ESC%[32m
	set Magenta=%ESC%[35m
	set Blue=%ESC%[94m
   
:: Display welcome message to the user welcome.txt      
	type c:\blinkit\config\welcome.txt
	
:: Let the user know that Blinkit is going to watch for Posts by displaying the text:
	echo Blinkit - Steem Account Posts

:: Let the user know a sound is being played by displaying the text:
	echo.
	echo %Magenta%Testing %White%Play sound...

:: Play and test Sound notification	
	set /p sound=<c:\blinkit\config\sound.txt 
	set "file=C:\blinkit\sounds\%sound%"
	( echo Set Sound = CreateObject("WMPlayer.OCX.7"^)
	echo Sound.URL = "%file%"
	echo Sound.Controls.play
	echo do while Sound.currentmedia.duration = 0
	echo wscript.sleep 100
	echo loop
	echo wscript.sleep (int(Sound.currentmedia.duration^)+1^)*1000) >C:\blinkit\sounds\sound.vbs
	
	if %soundsetting%==On (start /min C:\blinkit\sounds\sound.vbs) else (echo Sound notifications are turned off) 
	echo.
		 
:: Let the user know the Philips Hue is going to be blinked, on the %iphue%  by displaying the text:  	  
	echo %Magenta%Philips Hue IP: %White%%iphue%
	echo.
	echo %Magenta%Testing %White%Blink light
	start  C:\blinkit\bat\philipshuered.exe 
	timeout %blinklengthphilipshue%
	start  C:\blinkit\bat\philipshueoff.exe 	
	echo.


::  Download the latest STEEM and SBD Price and put it inside a txt files
	powershell -Command "Invoke-WebRequest https://api.coinmarketcap.com/v1/ticker/steem/ -OutFile C:\blinkit\config\steemprice.txt"
	powershell -Command "Invoke-WebRequest https://api.coinmarketcap.com/v1/ticker/steem-dollars/ -OutFile C:\blinkit\config\sbdprice.txt"

:: Find and display the latest STEEM and SBD Price in USD from the downloaded txt files
	echo Steem Price
	for /F "delims=" %%a in ('findstr /I ""price_usd"" C:\blinkit\config\steemprice.txt') do set "batToolDir0=%%a"
	for /F "delims=" %%a in ('findstr /I ""price_usd"" C:\blinkit\config\sbdprice.txt') do set "batToolDir2=%%a"
	echo %Blue%Steem: 				"%batToolDir0%"
	echo %Blue%Steem Dollar: 			"%batToolDir2%"

:: Update the STEEM and SBD price in USD into txt files for the steem prices in the Blinkit interface
	echo "%batToolDir0%" > "C:\blinkit\config\steempricestriped.txt"
	echo "%batToolDir2%" > "C:\blinkit\config\sbdpricestriped.txt"
	echo.


:: Display Steem username
	echo %White%Steem Username 
	echo %Blue%%steemusername%%White%
	echo.
	  
:: Download the number of Followers from a user inside a txt file
	powershell -Command "Invoke-WebRequest https://api.steem.place/getFollowersCount/?a=%steemusername% -OutFile C:\blinkit\config\displayfollowers.txt"

:: Display the downloaded followers	
	echo %White%Followers%Blue% 
	type C:\blinkit\config\displayfollowers.txt 
	echo.
	  
:: Display the saved Username and IP/Hostname and let the user know that the program is starting to look for new Posts
	echo.
	echo %White%Preparing BlinkIt, 
	echo Notifications for %Blue%%steemusername%%White% on Sonoff: %ip% ...	  
	timeout 4 
	  

:: Blinkit Posts Script

:: Download the number of posts from a steem user, from the Web Api and save it into a txt file
	powershell -Command "Invoke-WebRequest https://api.steem.place/getPostCount/?a=%steemusername% -OutFile C:\blinkit\config\downloadposts.txt"
	
:main   


:: Download the number of posts from a steem user, from the Web Api and save it into a txt file
	powershell -Command "Invoke-WebRequest https://api.steem.place/getPostCount/?a=%steemusername% -OutFile C:\blinkit\config\downloadposts2.txt"
    

:: Compare the 2 downloaded posts txt files if different go to "notification", if the files are the same go to "next"  
    fc C:\blinkit\config\downloadposts.txt C:\blinkit\config\downloadposts2.txt > nul
	if errorlevel 1 goto notification 
	if errorlevel 0 goto next
..		
		
:next
:: let the user know the program is running by displaying the text:   
	echo.
	echo %Magenta%Blinkit is running...
	
:: Download new data to compare, into "downloadposts2.txt", and go back to "main" to compare the files again
	powershell -Command "Invoke-WebRequest https://api.steem.place/getPostCount/?a=%steemusername% -OutFile C:\blinkit\config\downloadposts2.txt" 
	goto main

	
:notification
:: Let the user know, there is a new Post! by displaying the text:  
	echo Blinkit is running... %Blue%NEW POST from %steemusername%!
	echo %White%
	echo %Magenta%ACTION Philips Hue BLINKED!
  
:: Play the notification sound if turned on by the user 
	if %soundsetting%==On (start /min C:\blinkit\sounds\sound.vbs) else (echo Sound notifications are turned off) 
	echo.
	echo %White%   
  
:: Let the user know, there is a new Post, blink the Philips Hue lights	
	start  C:\blinkit\bat\philipshuered.exe 
	timeout %blinklengthphilipshue%
	start  C:\blinkit\bat\philipshueoff.exe 
	echo.
	echo %White%

:: Download new data to compare, and go back to "main" and continue to look for for new Posts.
	powershell -Command "Invoke-WebRequest https://api.steem.place/getPostCount/?a=%steemusername% -OutFile C:\blinkit\config\downloadposts.txt"	
	goto main  