//Blinkit Standalone v1.0 for "Heltec WIFI KIT 8" 

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


// initialise GPIO pins
int ledPin = 0;
int piezopin = 15;



 
void setup () {

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
  delay(350); 

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Connecting..");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(350); 

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Connecting...");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(350); 


//Show on the display that the device will do a test blink action  
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Test blink action.");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(350); 

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Test blink action..");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(350); 

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tf); // select font
  u8g2.drawStr(0,30,"Test blink action...");  // write text to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(350); 

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



 
}










 
void loop() {

//main loop


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
  delay(1000);

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
  delay(1000);

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

  delay(1000);



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
  compare();

 
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



//Blink Action - Blink screen and toggle the GPIO (LED) on / off



// Blink screen and led on and make a sound
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.drawBox(0,0,200,200);  // write to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(ledPin, HIGH); // turn the LED on
  sound();
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
  sound();
  delay(900); 





// Blink screen and led on and make a sound
  u8g2.clearBuffer();         // clear the internal memory
  u8g2.drawBox(0,0,200,200);  // write to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  digitalWrite(ledPin, HIGH); // turn the LED on
  sound();
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




// Blink screen and led on and make a sound
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



// Function to play notification sound 
void sound() {

   tone(piezopin, 1000, 300); // frequency, time
   delay(200);
   tone(piezopin, 1300, 300); // frequency, time
   delay(300);
   tone(piezopin, 1600, 300); // frequency, time
   delay(300);
}


