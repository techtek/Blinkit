Blinkit Standalone for Heltec - WiFi KIT 8


How to use 

- Open the arduino IDE, and the Blinkit Arduino Sketch
- With the arduino IDE search for and download and install the ESP8266 (wifi) and U8g2 (display) and ArduinoJson libraries.
- Copy the custom WiFi manager library folder to your Arduino IDE library folder
- Upload the Blinkit Arduino sketch to your Heltec WiFi Kit 8 

- Restart the module, the Blinkit splashscreen with Blinkit Logo should show up.
- Connect to the device it's WiFi network "Blinkit Setup" with password 87654321 and browse to http://192.168.4.1 to setup the device and make it connect 
to your home WiFi network and the internet. Add the steem username you want to recieve notifications for and press the save button.

- The device will restart and try to connect to the saved WiFi network, if it fails it will switch to accespoint mode / setup state again. 
- To reconnect your phone to the device once everything is setup, make sure your phone and the device are on the same WiFi network. and to connnect your phone 
and know the address to browse to, the device wil display it's IP while starting, use this to browse to the device it's web interface with your mobile phone, tablet or PC.



To reset the saved WiFi settings and saved Steem username, browse to the Blinkit webinterface and press the reset link presented at the bottom of the page.





Light and sound

Connect the notification LED light to GPIO 0, on the board printed as pin 03
and connect the piezo speaker to GPIO 15, on the board printed as pin 05

If you need assistance with how to do this safely without possibly damaging the board, 
and for technical support, please get in contact in the Blinkit Discord chat server:

https://discord.gg/73h8XqP



