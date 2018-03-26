@echo off    
   
:: Take and put the username from username.txt and put it in variable %username%
	set /p steemusername=<c:\blinkit\config\username.txt
 
:: Get the flashdrive letter set by the user in the Blinkit GUI, stored in \config\ and put it into variable %flashdrive2% 
	set /p flashdrive2=<c:\blinkit\config\drive2.txt
	
:: Get the Blink length set by the user in the Blinkit GUI, stored in \config\ and put it into variable %blinklength2%
	set /p blinklength2=<c:\blinkit\config\blinklength2.txt
	
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



:: Let the user know that Blinkit is going to watch for Upvotes by displaying the text:
	echo Blinkit - Steem Account Upvotes

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
	  
:: Let the user know the led is going to be blinked, on the %flashdrive2% letter by displaying the text:  	  
	echo %Magenta%USB Flash Drive: %White%%flashdrive2%
	echo.

:: Blink the LED, by copying the LED file from the Blinkit folder to the flashdrive
	echo %Magenta%Testing %White%Blink LED 
	xcopy c:\blinkit\ledfile\ledfile%blinklength2%.led %flashdrive2%. /Y > nul 
	echo.


::  Download the latest STEEM and SBD Price and put it inside a txt files
	powershell -Command "Invoke-WebRequest https://api.coinmarketcap.com/v1/ticker/steem/ -OutFile c:\blinkit\config\steemprice.txt"
	powershell -Command "Invoke-WebRequest https://api.coinmarketcap.com/v1/ticker/steem-dollars/ -OutFile c:\blinkit\config\sbdprice.txt"

:: Find and display the latest STEEM and SBD Price in USD from the downloaded txt files
	echo Steem Price
	for /F "delims=" %%a in ('findstr /I ""price_usd"" c:\blinkit\config\steemprice.txt') do set "batToolDir0=%%a"
	for /F "delims=" %%a in ('findstr /I ""price_usd"" c:\blinkit\config\sbdprice.txt') do set "batToolDir2=%%a"
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
	powershell -Command "Invoke-WebRequest https://api.steem.place/getFollowersCount/?a=%steemusername% -OutFile c:\blinkit\config\displayfollowers.txt"
		
:: Display the downloaded followers	
	echo %White%Followers%Blue% 
	type c:\blinkit\config\displayfollowers.txt 
	echo.

:: Display the saved Username and Flash drive letter and let the user know that the program is starting to look for new Followers
	echo.
	echo %White%Preparing BlinkIt, 
	echo USB Flash Notifications for %Blue%%steemusername%%White% on USB flash drive: %flashdrive2% ...	  
	timeout 4 

	
:: Blinkit Upvotes Script 

:: Download Upvotes data from Web Api and save it into a txt file
	powershell -Command "Invoke-WebRequest http://api.comprendre-steem.fr/getStatus?username=%steemusername% -OutFile c:\blinkit\config\downloadvotes.txt"

:main   
:: Download Upvotes data from Web Api and save it into a txt file
	powershell -Command "Invoke-WebRequest http://api.comprendre-steem.fr/getStatus?username=%steemusername% -OutFile c:\blinkit\config\downloadvotes2.txt"
    	
:: Compare the 2 downloaded follower txt files if different go to "notification", if the files are the same go to "next" 
    fc c:\blinkit\config\downloadvotes.txt c:\blinkit\config\downloadvotes2.txt > nul
	if errorlevel 1 goto notification 
	if errorlevel 0 goto next
		
		
:next
:: let the user know the program is running by displaying the text:  
	echo.
	echo %Magenta%Blinkit is running...
  	
:: Download new data to compare, into "downloadvotes2.txt", and go back to "main" to compare the files again
	::powershell -Command "Invoke-WebRequest http://api.comprendre-steem.fr/getIncomingVotes?username=%steemusername% -OutFile c:\blinkit\config\downloadvotes2.txt" 
    powershell -Command "Invoke-WebRequest http://api.comprendre-steem.fr/getStatus?username=%steemusername% -OutFile c:\blinkit\config\downloadvotes2.txt" 

	
	goto main

    
:notification
:: Let the user know, there is a new Upvote! by displaying the text:   
	echo Blinkit is running... %Blue%NEW UPVOTE for %steemusername%!
	echo %White%  
	echo %Magenta%ACTION LED BLINKED!

:: Let the user know, there is a new Upvote, and blink the LED by copying the LED file to the flash drive 
    xcopy c:\blinkit\ledfile\ledfile%blinklength2%.led %flashdrive2%. /Y > nul  
	
:: Play the notification sound if turned on by the user 
	if %soundsetting%==On (start /min C:\blinkit\sounds\sound.vbs) else (echo Sound notifications are turned off) 
	echo.
	timeout 3	
	echo %White%  

:: Download new data to compare, and go back to "main" and continue to look for for new Upvotes.
	powershell -Command "Invoke-WebRequest http://api.comprendre-steem.fr/getStatus?username=%steemusername% -OutFile c:\blinkit\config\downloadvotes.txt"	
	goto main