# Blinkit Arduino integration


### Blinkit

Blinkit is a Steem notification software for Windows, that notifies you in real time with sound and light about Steem account activity. Blinkit can blink the lights of the following products:

#### Supported devices:
* USB Sticks (status light)
* Philips HUE lamps
* Sonoff devices
* Arduino Boards or compatible (Genuino, etc...)
* Camera status LED blink
* Take photos on upvote/post/follow
* Logitech RGB/Backlight Keyboards (new)

### Installation instructions for Windows

Download the content of the [Blinkit folder](https://github.com/techtek/Blinkit)
Extract the Blinkit folder
Open the Blinkit folder, and start Blinkit.exe

### Arduino 


In order to be able to use a Arduino with Blinkit, the custom made sketch needs to be downloaded through this repo and needs to be uploaded to your Arduino board with the [Arduino IDE](https://www.arduino.cc/en/main/software). 
The wiring diagram to follow is located in the root folder.

How to upload the sketch to your board using Arduino IDE:

- After having installed Arduino IDE and restarted your PC/Laptop connect to it your Arduino board with its data/power USB cable
- If you have an Arduino Clone board (chinese boards) with integrated chip marked CH340 you can follow my tutorial on how to install its driver [here](https://steemit.com/blinkit/@electronicsworld/blinkit-or-electronicsworld-tutorial-2-how-to-set-up-arduino-for-blinkit-interaction)
- If you have an Original Arduino board or a Genuino board the above step is not required
- Once your PC/laptop recognize the board you can double click the ArduinoBlinkitSketch.ino file which by default will be opened with Arduino IDE
- Check on the bottom right section if you can see your device name and if the COM port selected is the one where your Arduino board is connected to (you can refer to the tutorial mentioned above to double check the COM port to which it is connected)
- If the COM port is not the right one then go to Tools -> Port -> and select the right COM port
- You can now click the UPLOAD button which you can find directly below Edit in the Button Bar.
- Wait for the upload to finish (it will be notified in the bottom loading bar)

Now your board has the sketch uploaded to it and you can now start to use it with Blinkit.


Arduino sketch has been developed by @electronicsworld https://steemit.com/@electronicsworld
Blinkit is made by @techtek https://steemit.com/@techtek