@echo off    
:set   
	
	
	:: Colour settings
	set ESC=
	set Red=%ESC%[91m
	set White=%ESC%[37m
	set Green=%ESC%[32m
	set Magenta=%ESC%[35m
	set Blue=%ESC%[94m
    set Grey=%ESC%[90m

	
	
  
	:: Blinkit Menu   
:menu   
    cls
	echo.
	echo %Grey%Blinkit Light Notifications, %Grey%choose one of the options: %White% 
	echo %Grey%_____________________________________________________________________%White%
	echo.
	echo 1 Steem %Grey%(STEEM)%White%
	echo 2 Hive %Grey%(HIVE)%White%  
	echo 3 Exit
	echo %Grey%_____________________________________________________________________%White%
	echo.
	echo %Grey%Blinkit is made by Techtek, steemit.com/@techtek hive.blog/@techtek%White%
	echo.

	
	
	set /p see=Choose one of the options from the menu:

	if %see%==1 goto Steem
	if %see%==2 goto Hive
    if %see%==3 exit
  

	
	
:Steem   
    cls
	echo.
	echo %Grey%Blinkit %Blue%Steem %Grey%Light Notifications, %Grey%choose one of the options: %White% 
	echo %Grey%_____________________________________________________________________%White%
	echo.
	echo 1 Blink on new votes and comments on your two latest post
	echo 2 Blink when you get a new follower
	echo 3 Back
	echo 4 Exit
	echo %Grey%_____________________________________________________________________%White%
	echo.
	echo %Grey%Blinkit is made by Techtek, steemit.com/@techtek%White%
	echo.
	

	
	
	set /p see=Choose one of the options from the menu:

	if %see%==1 call %~dp0\Steem\SteemCommentsVotes.bat && cls
	if %see%==2 call %~dp0\Steem\SteemFollowers.bat && cls 
    if %see%==3 call Blinkit.bat && cls
    if %see%==4 exit
	



	
:Hive   
    cls
	echo.
	echo %Grey%Blinkit %Red%Hive %Grey%Light Notifications %Grey%Choose one of the options: %White% 
	echo %Grey%_____________________________________________________________________%White%
	echo.
	echo 1 Blink on new votes and comments on your two latest post
	echo 2 Blink when you get a new follower
	echo 3 Back
	echo 4 Exit
	echo %Grey%_____________________________________________________________________%White%
	echo.
	echo %Grey%Blinkit is made by Techtek, hive.blog/@techtek%White%
	echo.
	

	
	
	set /p see=Choose one of the options from the menu:

	if %see%==1 call %~dp0\Hive\HiveCommentsVotes.bat && cls
	if %see%==2 call %~dp0\Hive\HiveFollowers.bat && cls 
    if %see%==3 call Blinkit.bat && cls
    if %see%==4 exit
	
	
	

	
	
	