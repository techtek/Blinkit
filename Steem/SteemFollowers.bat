@echo off    
:set   
	
	:: Get the flashdrive letter set by the user in yourusbdrive.txt
	set /p flashdrive=<%~dp0\yourusbdrive.txt
	
	:: Get the Blink length set by the user in yourblinklength.txt 
	set /p blinklength=<%~dp0\yourblinklength.txt
	
	:: Get the Steem username from steemusername.txt 
	set /p steemusername=<%~dp0\steemusername.txt
    
	:: Steem API request JSON
    set jsonbody='{\"jsonrpc\":\"2.0\", \"method\":\"condenser_api.get_followers\", \"params\":[\"%steemusername%\",null,\"blog\",10], \"id\":1}' 
	
	
	
	:: Colour settings
	set ESC=
	set Red=%ESC%[31m
	set White=%ESC%[37m
	set Green=%ESC%[32m
	set Magenta=%ESC%[35m
	set Blue=%ESC%[94m
    set Grey=%ESC%[90m

  
	:: Blinkit Menu
:menu   
    cls
	echo.
	echo %Grey%Blinkit USB Flash Notifications %Blue%Steem %Grey%Account Followers%White% 
	echo %Grey%_____________________________________________________________________%White%
	echo.
	echo 1 Start Blinkit
	echo 2 Set LED Blink Length
	echo 3 Set Steem Account
	echo 4 Set USB Drive Letter
	echo 5 Test Sound Notification	
	echo 6 Test USB LED Light Blink 
	echo 7 Back
	echo 8 Exit
	echo %Grey%_____________________________________________________________________%White%
	echo.
	echo %Grey%Blinkit is made by Techtek, steemit.com/@techtek%White%
	echo.
	
	::  Download the latest Steem Price and put it inside a txt file
	powershell -Command "Invoke-WebRequest https://api.coingecko.com/api/v3/simple/price?ids=steem'&'vs_currencies=usd -OutFile %~dp0\data\steemprice.txt"

	:: Find and display the latest Steem Price from the downloaded txt files	
	for /F "delims=" %%a in ('findstr /I ""usd"" %~dp0\data\steemprice.txt') do set "batToolDir2=%%a"    

	:: Update the Steem price into txt file and make the price look pretty by stripping out unwanted characters
	echo "%batToolDir2%" > "%~dp0\data\steempricestriped.txt"
	powershell -Command "(gc %~dp0\data\steempricestriped.txt) -replace '^................' | Set-Content %~dp0\data\steempricestriped.txt -Force
	powershell -Command "(gc %~dp0\data\steempricestriped.txt) -replace '.......$' | Set-Content %~dp0\data\steempricestriped.txt -Force	
    set /p steemprice=<%~dp0\data\steempricestriped.txt
	echo %Grey%STEEM Price: %steemprice% USD %White% 
	echo.
	
	
	set /p see=Choose one of the options from the menu:

	if %see%==1 goto start
	if %see%==2 start %~dp0\yourblinklength.txt && cls && goto set
	if %see%==3 start %~dp0\steemusername.txt && cls && goto set
	if %see%==4 start %~dp0\yourusbdrive.txt && cls && goto set
	if %see%==5 powershell -c echo `a && cls && goto set
    if %see%==6 set /p blinklength=<%~dp0\yourblinklength.txt && set /p flashdrive=<%~dp0\yourusbdrive.txt && goto testblink
    if %see%==7 call Blinkit.bat && cls
    if %see%==8 exit
  

:testblink
	set loop1=0
	:loop1
	xcopy %~dp0\data\ledfile.led %flashdrive%. /Y > nul
	set /a loop1=%loop1%+1 
	if "%loop1%"=="%blinklength%" goto menu
	goto loop1

  
  
  
  
  
   
:start

   
:: Display welcome message to the user welcome.txt      
	type %~dp0\data\welcome.txt

	
:: Let the user know that Blinkit is going to watch for Upvotes by displaying the text:
	echo %Blue%Steem account followers
	echo.
	echo %Grey%Steem username: %steemusername%
    echo.
	  
:: Let the user know the led is going to be blinked, on the %flashdrive% letter by displaying the text:  	  
	echo %Grey%Your USB Flash Drive: %flashdrive%
	echo.

:: Blink the LED, by copying the LED file from the Blinkit folder to the flashdrive
	echo %Grey%Testing LED Blink... 
	xcopy %~dp0\data\ledfile.led %flashdrive%. /Y > nul 
	echo.
	
:: Let the user know a sound is being played by displaying the text:
	echo %Grey%Testing Notification Sound...%White%
	
:: Play and test windows notification sound	
	powershell -c echo `a	

:: Display the saved Username and Flash drive letter and let the user know that the program is starting to look for new Followers
	echo.
	echo %White%Blinkit is now connecting your USB flash drive to the %Blue%Steem%White% Blockchain...	  
	echo.
	
:: Blinkit Followers Script 

:: Download followers data from Steem API and save it into a txt file
	 powershell Invoke-RestMethod -ContentType 'application/json' -Method Post -Uri https://api.steemit.com -Body %jsonbody% -UserAgent "curl" -OutFile %~dp0\data\downloadedfollowers.txt"
	PING localhost -n 4 >NUL
	

:main   
:: Download followers data from Steem API and save it into a txt file
	 powershell Invoke-RestMethod -ContentType 'application/json' -Method Post -Uri https://api.steemit.com -Body %jsonbody% -UserAgent "curl" -OutFile %~dp0\data\downloadedfollowers2.txt"
	 PING localhost -n 4 >NUL

    	
:: Compare the 2 downloaded follower txt files if different go to "notification", if the files are the same go to "next" 
    fc %~dp0\data\downloadedfollowers.txt %~dp0\data\downloadedfollowers2.txt > nul
	if errorlevel 1 goto notification 
	if errorlevel 0 goto next
		
		
:next
:: let the user know the program is running by displaying the text:  
	echo.
	echo %White%Blinkit is running...
	

	
:: Download followers data from Steem API and save it into a txt file
	 powershell Invoke-RestMethod -ContentType 'application/json' -Method Post -Uri https://api.steemit.com -Body %jsonbody% -UserAgent "curl" -OutFile %~dp0\data\downloadedfollowers2.txt"
	

	 :: 8 seconds silent delay (works by pinging local host)
	 PING localhost -n 7 >NUL
	
	goto main

	
    
:notification

:: Let the user know, there is a new follower! by displaying the text:   
	echo.
	echo Blinkit %White%New follower for %Blue%Steem %White%user %steemusername% detected, %Green%Light blink! %White% 
	

:: Let the user know, there is a new Follower, and blink the LED by copying the LED file to the flash drive 
    	
	set loop=0
	:loop
	xcopy %~dp0\data\ledfile.led %flashdrive%. /Y > nul
	set /a loop=%loop%+1 
	if "%loop%"=="%blinklength%" goto sound
	goto loop

	:sound
	
	
	
	
:: Play windows notification sound
    powershell -c echo `a  >NUL		
	

:: Download followers data from Steem API and save it into a txt file
	 powershell Invoke-RestMethod -ContentType 'application/json' -Method Post -Uri https://api.steemit.com -Body %jsonbody% -UserAgent "curl" -OutFile %~dp0\data\downloadedfollowers.txt"
	
	 PING localhost -n 5 >NUL

	goto main