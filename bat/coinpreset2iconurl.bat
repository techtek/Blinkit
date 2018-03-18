@echo off

:: Take and put the Saved coin from preset1 and put it in variable %preset2%
	set /p preset2=<c:\blinkit\config\pricepreset2.txt
	
:: put the saved coin inside the URL for the GUI to load the icon of the coin. 
	echo https://coincodex.com/en/resources/images/admin/coins/%preset2%.png > "C:\blinkit\config\pricepreset2iconurl.txt"