#include <FS.h>                   //this needs to be first, and is needed to store values
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson  


#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino 


//needed for WiFiManager and webserver 
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager (use the WiFiManager library provided with this ino file)

// Includes the libraries for the OLED Display "U8g2"
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ 16);

  //If the screen didnt turn on, you might have a different version of the board. Try the below one by removing the above line, and removing the // from the below line.:
// U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/2 /* reset=*/ 4);


// Includes the libraries for the WiFi module "ESP8266"
#include <ESP8266HTTPClient.h>



char blynk_token[34] = "Steem user"; // needed to store the values in that WiFiManger accepts.   Example of this implementation and extra info is found here: https://community.blynk.cc/t/feeding-wifi-ssid-passwords-and-blynk-keys-via-a-webpage-in-the-esp8266-eeprom/3019/8

bool shouldSaveConfig = false; //flag for saving data

void saveConfigCallback () {  //callback notifying us of the need to save config
  Serial.println("Should save config");
  shouldSaveConfig = true;  
}



//Preparing Global Variabels to download the upvote data in
  String payload1 = "empty"; 
  String payload2 = "empty"; 

//Preparing Global Variabels steemuser
  String steemuser = "empty"; 

// Variable to store the current output state for the notification LED
  String ledPinState = "off";

// Variable to store the current settings. Lights when notification is comes in
  String lightonoff = "on";

// Variable to store the current sound setting
  String soundonoff = "on";

// Assign output variables to GPIO pins
  const int ledPin = 0;       //notification LED light
  const int piezopin = 15;    //piezo speaker

//webserver
  WiFiServer server(80);  // Set web server port number to 80
  String header;          // Variable to store the HTTP request
  


// Blinkit splashscreen logo
#define u8g_logo_width 128
#define u8g_logo_height 24
static unsigned char u8g_logo_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x10, 
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x0F, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1F, 0x08, 0x04, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x3F, 0x04, 
  0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xF0, 0xFF, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xF8, 0xFF, 0x07, 0x08, 0xFC, 0x3F, 0x1C, 
  0x00, 0x0E, 0x06, 0x38, 0x38, 0x78, 0xB0, 0x7F, 0x0E, 0xF8, 0xFF, 0x07, 
  0x0C, 0xFC, 0x3F, 0x1C, 0x00, 0x0E, 0x1E, 0x38, 0x38, 0x3C, 0x30, 0x0C, 
  0x7E, 0xF0, 0xFF, 0x0F, 0x1C, 0x1C, 0x70, 0x1C, 0x00, 0x0E, 0x3E, 0x38, 
  0x38, 0x1F, 0x30, 0x0C, 0xFC, 0xFE, 0xFF, 0x1F, 0x08, 0x1C, 0x70, 0x1C, 
  0x00, 0x0E, 0x7E, 0x38, 0xB8, 0x07, 0x30, 0x0C, 0xFC, 0xFF, 0xFF, 0x3F, 
  0x00, 0x1C, 0x70, 0x1C, 0x00, 0x0E, 0xFE, 0x38, 0xF8, 0x03, 0x30, 0x0C, 
  0xFC, 0xFF, 0xFF, 0x3F, 0x00, 0xFC, 0x3F, 0x1C, 0x00, 0x0E, 0xEE, 0x39, 
  0xF8, 0x01, 0x30, 0x0C, 0xFC, 0xFF, 0xFF, 0x1F, 0x00, 0x1C, 0x70, 0x1C, 
  0x00, 0x0E, 0xCE, 0x3F, 0xF8, 0x03, 0x30, 0x0C, 0x7C, 0xF8, 0xFF, 0x07, 
  0x00, 0x1C, 0x70, 0x1C, 0x00, 0x0E, 0x8E, 0x3F, 0xB8, 0x07, 0x30, 0x0C, 
  0x1C, 0xFC, 0xFF, 0x03, 0x00, 0x1C, 0x70, 0x1C, 0x00, 0x0E, 0x0E, 0x3E, 
  0x38, 0x1F, 0x30, 0x0C, 0x04, 0xF8, 0xFF, 0x01, 0x00, 0xFC, 0x7F, 0xFC, 
  0x3F, 0x0E, 0x0E, 0x3C, 0x38, 0x3E, 0x30, 0x0C, 0x00, 0xF8, 0xFF, 0xF1, 
  0x01, 0xFC, 0x3F, 0xFC, 0x3F, 0x0E, 0x0E, 0x38, 0x38, 0x78, 0x30, 0x0C, 
  0x00, 0x98, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xFC, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  };

//WiFi signal icons
#define u8g_wifistrongsignalicon_width 10
#define u8g_wifistrongsignalicon_height 8
static unsigned char u8g_wifistrongsignalicon_bits[] = {
  0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xB0, 0x01, 0xB0, 0x01, 0xB0, 0x01, 
  0xB6, 0x01, 0xB6, 0x01, };

#define u8g_wifigoodsignalicon_width 10
#define u8g_wifigoodsignalicon_height 8
static unsigned char u8g_wifigoodsignalicon_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 
  0x36, 0x00, 0x36, 0x00, };

#define u8g_wifibadsignalicon_width 10
#define u8g_wifibadsignalicon_height 8
static unsigned char u8g_wifibadsignalicon_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x06, 0x00, 0x06, 0x00, };



// Sound icons
#define u8g_soundofficon_width 10
#define u8g_soundofficon_height 8
static unsigned char u8g_soundofficon_bits[] = {
  0x08, 0x00, 0x2C, 0x02, 0x4F, 0x01, 0x9F, 0x00, 0x5F, 0x01, 0x2F, 0x02, 
  0x0C, 0x00, 0x08, 0x00, };

#define u8g_soundonicon_width 10
#define u8g_soundonicon_height 8
static unsigned char u8g_soundonicon_bits[] = {
  0x88, 0x00, 0x2C, 0x01, 0x4F, 0x01, 0x5F, 0x01, 0x5F, 0x01, 0x4F, 0x01, 
  0x2C, 0x01, 0x88, 0x00, };


// Upvote icon
#define u8g_upvotesicon_width 15
#define u8g_upvotesicon_height 15
static unsigned char u8g_upvotesicon_bits[] = {
  0xE0, 0x03, 0xF8, 0x0F, 0xFC, 0x1F, 0x7E, 0x3F, 0x3F, 0x7E, 0x1F, 0x7C, 
  0x0F, 0x78, 0xC7, 0x71, 0xE7, 0x73, 0xF7, 0x77, 0xFF, 0x7F, 0xFE, 0x3F, 
  0xFC, 0x1F, 0xF8, 0x0F, 0xE0, 0x03, };




void setup() {
    
    Serial.begin(115200);

    //start the screen
    u8g2.begin(); 

    //Show splashscreen with the Blinkit logo 
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.drawXBM( 0, 6, u8g_logo_width, u8g_logo_height, u8g_logo_bits);
    u8g2.sendBuffer();          // transfer internal memory to the display
    delay(3500);

    //set the mode for the GPIO pins
    pinMode(ledPin, OUTPUT);
    // Set outputs to LOW
    digitalWrite(ledPin, LOW);


 

//read configuration from filesystem

        //SPIFFS.format();    //clean FS, for testing
 
  Serial.println("Mounting FS...");    

  if (SPIFFS.begin()) {
    Serial.println("Mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("Reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("Opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(blynk_token, json["blynk_token"]);

        } else {
          Serial.println("Failed to load json config");
        }
      }
    }
  } else {
    Serial.println("Failed to mount FS");
  }
  //end read

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_blynk_token("blynk", "Steem user", blynk_token, 33);   // was 32 length
  
  Serial.println(blynk_token);



    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.addParameter(&custom_blynk_token);

    // uncomment the below lines to:
    
    //reset saved settings 
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    //wifiManager.autoConnect("Blinkit Setup");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();
    wifiManager.autoConnect("Blinkit Setup", "87654321");

  strcpy(blynk_token, custom_blynk_token.getValue());    //read updated parameters

  if (shouldSaveConfig) {      //save the custom parameters to FS
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["blynk_token"] = blynk_token;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("Failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

Serial.println("token saved: ");
Serial.println(blynk_token);
    
   //if get here it's connected to the WiFi
   Serial.println("WiFi connected...");
   server.begin();                       // start the webserver 


  // Start the web server and print the local IP address to serial monitor
  Serial.println("");                   // print empty line of text to the serial monitor
  Serial.println("WiFi connected.");    // print text to the serial monitor 
  Serial.println("IP address: ");       // print text to the serial monitor
  Serial.println(WiFi.localIP());       // show the Local ip in the serial monitor
  server.begin();                       // start the webserver 

  //Show the local IP adres of the Blinkit device on the display and notify about the webserver being ready
  u8g2.clearBuffer();                       // clear the internal memory of the display
  u8g2.setFont(u8g2_font_6x10_tf);          // select font
  u8g2.drawStr(0,10,"Connect your phone?"); // write first line of text to the internal memory
  u8g2.drawStr(0,20,"Browse to: ");         // write second line of text to the internal memory
  u8g2.setFont(u8g2_font_5x8_tf);           // select font (smaller font is use for the third line to fit the full IP with "http://"  
  u8g2.setCursor(0, 30);                    // write on the third line
  u8g2.print("http://");                    // write on the third line
  u8g2.print(WiFi.localIP());               // write local IP on the third line
  u8g2.sendBuffer();                        // transfer internal memory to the display
  delay(7000);                              // long delay to make the message to the user readable, but not to long to bother 

  //Show on the display that the device will do a test blink action  
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Test blink action.");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(250); 

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Test blink action..");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(250); 

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Test blink action...");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(250); 

}








void loop() {
    
//Put the saved blynk_token inside steemuser
steemuser = blynk_token;


//web-interface listen for incoming data 

  {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              ledPinState = "on";
              blinks();
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              ledPinState = "off";
              blinks();
            }


            // Toggle the sound ON/OFF
            if (header.indexOf("GET /3/on") >= 0) {
              Serial.println("on");
              soundonoff = "on"; 
              pinMode(piezopin, OUTPUT);
            } else if (header.indexOf("GET /3/off") >= 0) {
              Serial.println("off");
              soundonoff = "off";
              pinMode(piezopin, INPUT); 
            } 


            // Plays notification Sound
              else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("playsound");
              sound();
            } 




            // if /reset is recieved, reset all setting Wifi-ssid, Password, Steemusername and let the user know it is resetting by display the text on the display "Reset done: Disconnect power, and reconnect power"
            if (header.indexOf("GET /reset") >= 0) {
              Serial.println("resetting...");
              u8g2.clearBuffer();          // clear the internal memory
              u8g2.setFont(u8g2_font_6x10_tf); // select font 
              u8g2.setCursor(0, 10);
              u8g2.print("Reset done.");
              u8g2.setCursor(0, 20);
              u8g2.print("Disconnect power,");
              u8g2.setCursor(0, 30);
              u8g2.print("and reconnect power.");
              u8g2.sendBuffer();          // transfer internal memory to the display
              client.stop();              // disconnect clients
              WiFi.disconnect(true);      // erases store credentially
              SPIFFS.format();            // erases stored values
              ESP.restart();
            }

            
            
            // HTML Blinkit web interface  
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            // CSS to style the web-interface
            client.println("<style>html { font-family: Helvetica; color: white; display: inline-block; margin: 0px auto; background-color: #4f4e4e; text-align: center;}");
            client.println(".button { background-color: #833d95; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("a {color: grey;}");
            client.println(".button2 {background-color: #833d95;}</style></head>");
        
            // Web Page Heading
            client.println("<body>");
            client.println("<img src=\"https://cdn.steemitimages.com/DQmdaUvqdxEvtKeZgtZ1qGcrU8Aew4LqQAKMq8zz5vf6haR/Blinkitlogo-01-01.png\" alt=\"Blinkit Logo\" width=\"266\" height=\"auto\">");
            client.println("<br><br />");

            // Display the Steem username and the total recieved upvotes for it            
            client.println("<p>Steem User <b>" + steemuser + "</b></p>");
            client.println("<p>Received Upvotes <b>" + payload2 + "</b></p>");  
            client.println("<p><font color=\"grey\">_____________________________________</font></p>");  // Display a line
            
            // Display the Notifcation sound on/off toggle button, and display the Test notification sound button, If the soundonoff state is off, it displays the ON button otherwise the OFF button       
              if (soundonoff=="off") {  
              client.println("<p>Sound notifications are <b><font color=\"red\"> " + soundonoff + " </font></b></p>");           
              client.println("<p><a href=\"/3/on\"><button class=\"button\">ON</button></a>");
              client.println("<a href=\"/4/on\"><button class=\"button\">TEST</button></a></p>");
              
            } else {
              client.println("<p>Sound notifications are <b><font color=\"green\"> " + soundonoff + " </font></b></p>"); 
              client.println("<p><a href=\"/3/off\"><button class=\"button button\">OFF</button></a>");
              client.println("<a href=\"/4/on\"><button class=\"button\">TEST</button></a></p>");
            } 
            
            client.println("&nbsp;"); //single emptyline    


            // Display a button to test the notification light
            if (ledPinState=="off") {
              client.println("<p>Test notification light</p>");
              
              client.println("<p><a href=\"/5/on\"><button class=\"button button3\">TEST</button></a>");
            } else {
              client.println("<p>Test notification light</p>");
              
              client.println("<p><a href=\"/5/off\"><button class=\"button button3\">TEST</button></a>");
            }


            client.println("<br><br />"); //single emptyline  
            client.println("<br><br />"); //single emptyline  
            client.println("<p><font color=\"grey\">_____________________________________</font></p>");   // Display a line
            
            // Show a link to reset the device 
            client.println("<p><a href=\"/reset\">Reset the device</a></p>");                             
            
           


            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}




//Display the Main screen and download upvote data to compare 
     if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
  
     //If Wifi is connected get the data from the API 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://api.comprendre-steem.fr/getStatus?username="+ steemuser);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request


 
    if (httpCode > 0) { //Check the returning code 
     payload1 = http.getString();   //Get the request response payload
          //Serial.println(payload1);     Print the response payload

      payload1.replace("username", "");
      payload1.replace("votes", "");
      payload1.replace("received", "");
      payload1.replace(steemuser, "");
      payload1.replace("{", "");
      payload1.replace("}", "");
      payload1.replace(":", "");
      payload1.replace(",", "");
      payload1.replace("\"", "");
      //Serial.println(payload1);    Print the response payload

 mainscreen();
 
    }
 
    http.end();   //Close connection
 
  }
  
  delay(100);    
  compare();
    
}


 
// function to show the mainscreen on the display
void mainscreen() {

              u8g2.clearBuffer();          // clear the internal memory
 
 //WiFi.RSSI();
 long rssi = WiFi.RSSI();


//Screen 1 (the mainscreen loops to 3 screens)
if (rssi<= -60) {  //Strong signal 
             u8g2.drawXBM( 116, 0, u8g_wifistrongsignalicon_width, u8g_wifistrongsignalicon_height, u8g_wifistrongsignalicon_bits);  
}

if (rssi<= -70) { // Good signal
             u8g2.drawXBM( 116, 0, u8g_wifigoodsignalicon_width, u8g_wifigoodsignalicon_height, u8g_wifigoodsignalicon_bits);
}

if (rssi<= -80) { // Bad signal
             u8g2.drawXBM( 116, 0, u8g_wifibadsignalicon_width, u8g_wifibadsignalicon_height, u8g_wifibadsignalicon_bits);
}

// display the sound icon
  if (soundonoff=="on") {
              u8g2.drawXBM( 103, 0, u8g_soundonicon_width, u8g_soundonicon_height, u8g_soundonicon_bits);
            } else {
              u8g2.drawXBM( 103, 0, u8g_soundofficon_width, u8g_soundofficon_height, u8g_soundofficon_bits);
            }       

             u8g2.drawStr( 0, 0, "drawLine");
             u8g2.drawLine(0, 12, 128, 12);
  

             u8g2.setFont(u8g2_font_freedoomr10_tu); // select font
             u8g2.drawXBM( 0, 17, u8g_upvotesicon_width, u8g_upvotesicon_height, u8g_upvotesicon_bits);
             u8g2.setCursor(20, 31);
             u8g2.print(payload1);
             u8g2.print(".");
             u8g2.setFont(u8g2_font_6x10_tf); // select font
             u8g2.setCursor(0, 10);
             u8g2.print(steemuser);
             u8g2.sendBuffer();          // transfer internal memory to the display
             delay(900);
             u8g2.clearBuffer();          // clear the internal memory





// Screen 2
if (rssi<= -60) {  //Strong signal 
             u8g2.drawXBM( 116, 0, u8g_wifistrongsignalicon_width, u8g_wifistrongsignalicon_height, u8g_wifistrongsignalicon_bits);
}

if (rssi<= -70) { // Good signal
             u8g2.drawXBM( 116, 0, u8g_wifigoodsignalicon_width, u8g_wifigoodsignalicon_height, u8g_wifigoodsignalicon_bits);
}

if (rssi<= -80) { // Bad signal
             u8g2.drawXBM( 116, 0, u8g_wifibadsignalicon_width, u8g_wifibadsignalicon_height, u8g_wifibadsignalicon_bits);
}

// display the sound icon
  if (soundonoff=="on") {
              u8g2.drawXBM( 103, 0, u8g_soundonicon_width, u8g_soundonicon_height, u8g_soundonicon_bits);
            } else {
              u8g2.drawXBM( 103, 0, u8g_soundofficon_width, u8g_soundofficon_height, u8g_soundofficon_bits);
            }       

             u8g2.drawStr( 0, 0, "drawLine");
             u8g2.drawLine(0, 12, 128, 12);
  

             u8g2.setFont(u8g2_font_freedoomr10_tu); // select font
             u8g2.drawXBM( 0, 17, u8g_upvotesicon_width, u8g_upvotesicon_height, u8g_upvotesicon_bits);
             u8g2.setCursor(20, 31);
             u8g2.print(payload1);
             u8g2.print("..");
             u8g2.setFont(u8g2_font_6x10_tf); // select font
             u8g2.setCursor(0, 10);
             u8g2.print(steemuser);
             u8g2.sendBuffer();          // transfer internal memory to the display
             delay(900);
             u8g2.clearBuffer();          // clear the internal memory



//screen 3
if (rssi<= -60) {  //Strong signal 
             u8g2.drawXBM( 116, 0, u8g_wifistrongsignalicon_width, u8g_wifistrongsignalicon_height, u8g_wifistrongsignalicon_bits);
}

if (rssi<= -70) { // Good signal
             u8g2.drawXBM( 116, 0, u8g_wifigoodsignalicon_width, u8g_wifigoodsignalicon_height, u8g_wifigoodsignalicon_bits);
}

if (rssi<= -80) { // Bad signal
             u8g2.drawXBM( 116, 0, u8g_wifibadsignalicon_width, u8g_wifibadsignalicon_height, u8g_wifibadsignalicon_bits);
}

// display the sound icon
  if (soundonoff=="on") {
              u8g2.drawXBM( 103, 0, u8g_soundonicon_width, u8g_soundonicon_height, u8g_soundonicon_bits);
            } else {
              u8g2.drawXBM( 103, 0, u8g_soundofficon_width, u8g_soundofficon_height, u8g_soundofficon_bits);
            }       

             u8g2.drawStr( 0, 0, "drawLine");
             u8g2.drawLine(0, 12, 128, 12);
  

             u8g2.setFont(u8g2_font_freedoomr10_tu); // select font
             u8g2.drawXBM( 0, 17, u8g_upvotesicon_width, u8g_upvotesicon_height, u8g_upvotesicon_bits);
             u8g2.setCursor(20, 31);
             u8g2.print(payload1);
             u8g2.print("...");
             u8g2.setFont(u8g2_font_6x10_tf); // select font
             u8g2.setCursor(0, 10);
             u8g2.print(steemuser);
             u8g2.sendBuffer();          // transfer internal memory to the display
             delay(900);
             u8g2.clearBuffer();          // clear the internal memory
}









// function to compare and download new data to compare with.
void compare()
{
    // compare downloaded data and do a action
    if (payload1.equals(payload2)) {
    // Serial.println(payload1 + " equals " + payload2 + " Continue detecting");


    
    Serial.println(""); //empty line
    Serial.println("Blinkit is running...");
  } else {
    //Serial.println(payload1 + " does not equal " + payload2 + " NEW UPVOTE!!!!!!!!!!!!!!!!!!!!!!!");
    Serial.println("Blinkit NEW UPVOTE...!");

//Blink Notification: New upvote detected, Blink screen and toggle the GPIO (LED), and trigger the notification sound.

//play notification sound
sound();

//triggers the blink action and Blinks the LED and screen
blinks();


// download new data into payload1
     if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status


  
     //If Wifi is connected get the data from the API 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://api.comprendre-steem.fr/getStatus?username="+ steemuser);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request


 
    if (httpCode > 0) { //Check the returning code 
     payload1 = http.getString();   //Get the request response payload
          //Serial.println(payload1);     Print the response payload

      payload1.replace("username", "");
      payload1.replace("votes", "");
      payload1.replace("received", "");
      payload1.replace(steemuser, "");
      payload1.replace("{", "");
      payload1.replace("}", "");
      payload1.replace(":", "");
      payload1.replace(",", "");
      payload1.replace("\"", "");
          //Serial.println(payload1);    Print the response payload
 
    }
 
    http.end();   //Close connection
 
  }
  
  delay(3000);    //Send a request every 3 seconds

// download new dat into payload2    
  download2();
   
   
  }
  
}



//Download data in global var payload2, when downloaded go back to compare function
void download2()
{     if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
  
     //If Wifi is connected get the data from the API 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://api.comprendre-steem.fr/getStatus?username="+ steemuser);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code 
     payload2 = http.getString();   //Get the request response payload
     //Serial.println(payload2);                     //Print the response payload

      payload2.replace("username", "");
      payload2.replace("votes", "");
      payload2.replace("received", "");
      payload2.replace(steemuser, "");
      payload2.replace("{", "");
      payload2.replace("}", "");
      payload2.replace(":", "");
      payload2.replace(",", "");
      payload2.replace("\"", "");
     //Serial.println(payload2);
 
    }
 
    http.end();   //Close connection
 
  }
 
  delay(3000);    //wait 3 seconds
  compare();
 
}

//Function to blink the led and screen
void blinks() {

            if (lightonoff=="off") {
              // do nothing
            } else {
    // Blink screen and led on 
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.drawBox(0,0,128,32);   // write to the internal memory
    u8g2.sendBuffer();          // transfer internal memory to the display
    digitalWrite(ledPin, HIGH); // turn the LED on
    delay(900);
              } 

  // show black screen, turn sound off and led off
    u8g2.clearBuffer();         // clear the internal memory / display
    u8g2.sendBuffer();          // transfer internal memory to the display
    digitalWrite(ledPin, LOW);  // turn the LED off
    delay(900); 
  
  // Blink screen and led on
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.drawBox(0,0,128,32);   // write to the internal memory
    u8g2.sendBuffer();          // transfer internal memory to the display
    digitalWrite(ledPin, HIGH); // turn the LED on
    delay(900);
  
  // show black screen, turn sound off and led off
    u8g2.clearBuffer();         // clear the internal memory / display
    u8g2.sendBuffer();          // transfer internal memory to the display
    digitalWrite(ledPin, LOW);  // turn the LED off
    delay(900); 
  
  // Blink screen and led on
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.drawBox(0,0,128,32);   // write to the internal memory
    u8g2.sendBuffer();          // transfer internal memory to the display
    digitalWrite(ledPin, HIGH); // turn the LED on
    delay(900);  

  // show black screen, turn sound off and led off
    u8g2.clearBuffer();         // clear the internal memory / display
    u8g2.sendBuffer();          // transfer internal memory to the display
    digitalWrite(ledPin, LOW);  // turn the LED off
    delay(500);     
 
}


// Function to store notification sound 
void sound() {
   
            if (soundonoff=="off") {
              // do nothing
            } else {
   tone(piezopin, 600, 100); // frequency, time
   delay(100);
   tone(piezopin, 1300, 100); // frequency, time
   delay(100);
   tone(piezopin, 1600, 300); // frequency, time
   delay(300);
            }    

   

}
