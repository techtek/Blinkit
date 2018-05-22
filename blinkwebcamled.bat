@echo off    
  
:: Turn on the Webcam for the set time, and turn it off
  
 set /p time=<config\webcamledlength.txt
  	
	:: call bat\webcamled.exe 	
	start /d "\" webcamled.exe
	 	
	timeout %time%
		
	:: call bat\webcamled.exe 	
	taskkill /im webcamled.exe
	
