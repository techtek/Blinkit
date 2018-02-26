@echo off
      
:: Let the user know that the current steemprice is going to be downloaded by displaying the text:	  
	echo.
	echo Getting steemprice

::  Download the latest STEEM and SBD Price and put it inside a txt files
	powershell -Command "Invoke-WebRequest https://api.coinmarketcap.com/v1/ticker/steem/ -OutFile C:\blinkit\config\steemprice.txt"
	powershell -Command "Invoke-WebRequest https://api.coinmarketcap.com/v1/ticker/steem-dollars/ -OutFile C:\blinkit\config\sbdprice.txt"
	  
:: Find and display the latest STEEM and SBD Price in USD from the downloaded txt files	  
	for /F "delims=" %%a in ('findstr /I ""price_usd"" C:\blinkit\config\steemprice.txt') do set "batToolDir0=%%a"
	for /F "delims=" %%a in ('findstr /I ""price_usd"" C:\blinkit\config\sbdprice.txt') do set "batToolDir2=%%a"
	echo Steem 				"%batToolDir0%"
	echo Steem Dollar 			"%batToolDir2%"

	 
:: Update the STEEM and SBD price in USD into txt files for the steem prices in the Blinkit interface
	echo %batToolDir0%>C:\blinkit\config\steempricestriped.txt
	echo %batToolDir2% > C:\blinkit\config\sbdpricestriped.txt
	

:: Deletes unwanted characters( "price_usd": "" ) from the steempricestriped.txt and sbdpricestriped.txt and keep only the numeric value
	SETLOCAL enabledelayedexpansion
	:: Delete characters from steempricestriped.txt
	FOR /f "delims=" %%i IN (C:\blinkit\config\steempricestriped.txt) DO (
	SET line=%%i
	SET line=!line:"=!
	SET line=!line:price_usd=!
	SET line=!line::=!
	SET line=!line:,=!
	echo !line! > C:\blinkit\config\steempricestriped.txt
	)
	
	:: Delete characters from sbdpricestriped.txt
	FOR /f "delims=" %%i IN (C:\blinkit\config\sbdpricestriped.txt) DO (
	SET line=%%i
	SET line=!line:"=!
	SET line=!line:price_usd=!
	SET line=!line::=!
	SET line=!line:,=!
	echo !line! > C:\blinkit\config\sbdpricestriped.txt  
	)
	

	 