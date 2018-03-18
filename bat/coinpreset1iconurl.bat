@echo off

:: Take and put the Saved coin from preset1 and put it in variable %preset1%
	set /p preset1=<c:\blinkit\config\pricepreset1.txt
	
:: put the saved coin inside the URL for the GUI to load the icon of the coin. 
	echo https://coincodex.com/en/resources/images/admin/coins/%preset1%.png > "C:\blinkit\config\pricepreset1iconurl.txt"