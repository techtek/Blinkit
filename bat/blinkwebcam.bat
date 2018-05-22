@echo off    
   
 set /p time=<config\webcamledlength.txt
  
:: Let the user know, there is a new Post, blink the light connected to the Sonoff device	
	
	start /d "bat\" webcamled.exe
	:: call bat\webcamled.exe 
	
timeout %time%

	taskkill /im webcamled.exe