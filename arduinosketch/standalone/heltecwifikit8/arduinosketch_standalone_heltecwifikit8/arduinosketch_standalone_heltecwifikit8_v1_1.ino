//Blinkit Standalone v1.1 for "Heltec WIFI KIT 8" 

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

// Includes the libraries for the WIFI module "ESP8266"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>





///////////////////////////////////////
//     Change to your parameter      // 
///////////////////////////////////////
 
// Change to your Steem Usernames:
String steemuser = "techtek"; 

// Change to your WiFi Settings:
const char* ssid = "wifiname"; 
const char* password = "password";

///////////////////////////////////////
//     Edit the above parameters     //
///////////////////////////////////////


//Preparing Global Variabels to download the upvote data in
String payload1 = "empty"; 
String payload2 = "empty"; 


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





 
void setup () {

// WiFi.hostname("blinkit"); //Set hostname -- needs more testing
//wifi_station_set_hostname("blinkit"); //Set hostname -- needs more testing



//set the mode for the GPIO pins
pinMode(ledPin, OUTPUT);

//start the screen
u8g2.begin(); 

//Show on the Display a splashscreen with the word "Blinkit" filling the screen
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_fub25_tf); // select font
  u8g2.drawStr(0,30,"BLINKIT");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(3500); 

//Show the configuration on the display
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,10,"Blinkit | V 1.0");  // write text to the internal memory
  u8g2.setCursor(0, 20);
  u8g2.print("Wifi    | ");
  u8g2.print(ssid);
  u8g2.setCursor(0, 30);
  u8g2.print("User    | ");
  u8g2.print(steemuser);
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(3500);

  
 

//Start serial with baudrate 115200 
  Serial.begin(115200);

//Start and connect to the WiFi network
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
 
  delay(1000);
  Serial.println(""); //empty line
  Serial.println("Blinkit is starting..."); // send the text to serial console
  Serial.println(""); //empty line
  Serial.println("Connecting..."); // send to the serial console, the text "Connecting..."
  Serial.println(""); //empty line

//Show on the display that the device is connecting...  
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Connecting.");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(300); 

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Connecting..");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(300); 

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Connecting...");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(300); 



// Dispay on the serial monitor the wifi signal strenght in dBm  
  long rssi = WiFi.RSSI();
  Serial.print("Connected to ");
  Serial.print(ssid);
  Serial.print(" | WiFi Signal ");
  Serial.print(rssi);
  Serial.println(" (dBm)");
  Serial.println(""); //empty line

//Display on the serial monitor that the device will do a Blink test
  Serial.println("Upvote Blink action will be tested...");
  Serial.println(""); //empty line
  }




//Start serial client
{
  Serial.begin(115200);
  
// Initialize the output variables as outputs
  pinMode(ledPin, OUTPUT);
// Set outputs to LOW
  digitalWrite(ledPin, LOW);
  
// Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
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

 
}










 
void loop() {

//main loop



//web-interface listen for incoming data 

  {
  WiFiClient client = server.available();                   // Listen for incoming clients

  if (client) {                                             // If a new client connects,
    Serial.println("New Client.");                          // print a message out in the serial monitor
    String currentLine = "";                                // make a String to hold incoming data from the client
    while (client.connected()) {                            // loop while the client's connected
      if (client.available()) {                             // if there's bytes to read from the client,
        char c = client.read();                             // read a byte, then
        Serial.write(c);                                    // print it out to the serial monitor
        header += c;
        if (c == '\n') {                                    // if the byte is a newline character
                                                            // if the current line is blank, you got two new line characters in a row.
                                                            // that's the end of the client HTTP request, send a response:
        if (currentLine.length() == 0) {
                
        client.println("HTTP/1.1 200 OK");                  // HTTP headers response code (HTTP/1.1 200 OK)
        client.println("Content-type:text/html");           // content-type to let the client know what is coming:
        client.println("Connection: close");
        client.println();

            
        // When the test notification light button is pressed on the client side and /5/on or /5/off is recieved by the webserver do:
        if (header.indexOf("GET /5/on") >= 0) {
           Serial.println("on");                            // write to serial monitor
           ledPinState = "on";                              // change the state of ledPinState
           blinks();                                        // start the blink actions once and stops,
         } else if (header.indexOf("GET /5/off") >= 0) {    // If a on/off toggle is needed use below: 
           Serial.println("GPIO 5 off");
           ledPinState = "off";                             
           blinks();                                        // start the blink actions once and stops,
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

            
            
            // Webpage - Display the HTML web page Head
                        
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
           
            // CSS to style the website 
            client.println("<style>html { font-family: Helvetica; color: white; display: inline-block; margin: 0px auto; background-color: #4f4e4e; text-align: center;}");
            client.println(".button { background-color: #833d95; border: none; width: 160px; color: white; padding: 8px 8px;");
            client.println("text-decoration: none; font-size: 30px; length: 2px; cursor: pointer;}");
            client.println(".button3 {background-color: #833d95; border: none; width: 320px; color: white; padding: 8px 8px}</style></head>");
                        
            // Web Page Body            
            client.println("<body>");

            // Display the Blinkit logo hosted on github
            client.println("<img src=\"https://techtek.github.io/Blinkit/images/app-promo/Blinkitlogo.svg\" alt=\"Blinkit Logo\" width=\"266\" height=\"auto\">");
            client.println("<br></br>");
                        
            // Display the Steem username and the total recieved upvotes for it            
            client.println("<p>Steem User <b>" + steemuser + "</b></p>");
            client.println("<p>Received Upvotes <b>" + payload2 + "</b></p>");  
            client.println("<p><font color=\"grey\">_____________________________________</font></p>");  // Display a line
            
            
            // Display the Notifcation sound on/off toggle button, and display the Test notification sound button, 
            // If the soundonoff state is off, it displays the ON button otherwise the OFF button       
                        
            if (soundonoff=="off") {  
              client.println("<p>Sound notifications are <b><font color=\"red\"> " + soundonoff + " </font></b></p>");           
              client.println("<p><a href=\"/3/on\"><button class=\"button\">ON</button></a>");
              client.println("<a href=\"/4/on\"><button class=\"button\">TEST</button></a></p>");
              
            } else {
              client.println("<p>Sound notifications are <b><font color=\"green\"> " + soundonoff + " </font></b></p>"); 
              client.println("<p><a href=\"/3/off\"><button class=\"button button\">OFF</button></a>");
              client.println("<a href=\"/4/on\"><button class=\"button\">TEST</button></a></p>");
            } 
                        
            
            // Display a button to test the notification light
            client.println("&nbsp;"); //single emptyline    
                        
            if (ledPinState=="off") {
              client.println("<p>Test notification light</p>");
              
              client.println("<p><a href=\"/5/on\"><button class=\"button button3\">TEST</button></a>");
            } else {
              client.println("<p>Test notification light</p>");
              
              client.println("<p><a href=\"/5/off\"><button class=\"button button3\">TEST</button></a>");
            }
           
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    
    // Clear the header variable
    header = "";
    
    client.stop(); // Close the connection
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}





//Display the Main screen and download data to compare 
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


 // function to show the mainscreen
void mainscreen() {
  //When the data is downloaded show the progress by updating the display text from "Online" to "Online."
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Blinkit | Online.");  // write text to the internal memory
     u8g2.setCursor(0, 20);
     u8g2.print("Upvotes | ");
     u8g2.print(payload1);
     u8g2.setCursor(0, 10);
     u8g2.print("User    | ");
     u8g2.print(steemuser);
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(900);

    //When the data is downloaded show the progress by updating the display text from "Online." to "Online.."
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Blinkit | Online..");  // write text to the internal memory
     u8g2.setCursor(0, 20);
     u8g2.print("Upvotes | ");
     u8g2.print(payload1);
     u8g2.setCursor(0, 10);
     u8g2.print("User    | ");
     u8g2.print(steemuser);
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(900);

      //When the data is downloaded show the progress by updating the display text from "Online.." to "Online..."
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Blinkit | Online...");  // write text to the internal memory
  u8g2.setCursor(0, 20);
     u8g2.print("Upvotes | ");
     u8g2.print(payload1);
     u8g2.setCursor(0, 10);
     u8g2.print("User    | ");
     u8g2.print(steemuser);
  u8g2.sendBuffer();          // transfer internal memory to the display

  delay(900);
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



//Blink Notification new upvote detected, Blink screen and toggle the GPIO (LED), and screen ON/OFF  

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


// compare downloaded data and do a action
//    if (payload1.equals(payload2)) {
//    Serial.println(payload1 + " equals " + payload2);
//  } else {
//    Serial.println(payload1 + " does not equal " + payload2);
//  }

 
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



// compare downloaded data and do a action
//    if (payload1.equals(payload2)) {
//    Serial.println(payload1 + " equals " + payload2);
//  } else {
//    Serial.println(payload1 + " does not equal " + payload2);
//  }

 
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
  u8g2.drawBox(0,0,200,200);  // write to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(ledPin, HIGH); // turn the LED on
  delay(900);
            } 
  
  // Blink screen and led on 
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.drawBox(0,0,200,200);  // write to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(ledPin, HIGH); // turn the LED on
  delay(900);

// show default screen, turn sound off and led off
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Blinkit | Online....");  // write text to the internal memory
  u8g2.setCursor(0, 20);
     u8g2.print("Upvotes | ");
     u8g2.print(payload1);
     u8g2.setCursor(0, 10);
     u8g2.print("User    | ");
     u8g2.print(steemuser);
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(ledPin, LOW);  // turn the LED off
  delay(900); 

// Blink screen and led on
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.drawBox(0,0,200,200);  // write to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(ledPin, HIGH); // turn the LED on
  delay(900);

// show default screen and turn the led off
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Blinkit | Online....");  // write text to the internal memory
  u8g2.setCursor(0, 20);
     u8g2.print("Upvotes | ");
     u8g2.print(payload1);
     u8g2.setCursor(0, 10);
     u8g2.print("User    | ");
     u8g2.print(steemuser);
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(ledPin, LOW);  // turn the LED off
  delay(900); 

// Blink screen and led on 
  u8g2.clearBuffer();       // clear the internal memory
  u8g2.drawBox(0,0,200,200);  // write to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(ledPin, HIGH); // turn the LED on
  delay(900); 
  
// show default screen, turn sound off and led off
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Blinkit | Online....");  // write text to the internal memory
  u8g2.setCursor(0, 20);
     u8g2.print("Upvotes | ");
     u8g2.print(payload1);
     u8g2.setCursor(0, 10);
     u8g2.print("User    | ");
     u8g2.print(steemuser);
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(ledPin, LOW);  // turn the LED off
  delay(900);  
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


