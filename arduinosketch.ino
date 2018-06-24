#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define LCD_HEIGHT 64

char buffer[17];

#define RedPIN  3 //pin for led 1 (single colour led) or pin for the Red led in the RGB led
#define GreenPIN  5 //pin for led 2 (single colour led) or pin for the Green led in the RGB led
#define BluePIN 6  //pin for led 3 (single colour led) or pin for the Blue led in the RGB led
//above pins must be DIGITAL PWM PINS (in Arduino Uno, PIN 3, 5, 6, 9, 10, 11 are PWM) in order to obtain the colours shades with the RGB led
 byte y=0;
 byte transferdata[3]; //array to be used to store the incoming settings and transfer them to the right setting array
 byte ledmode[1]; //this array will store the information regarding the ledmode (mode 1 for 3x single colour leds and mode 2 for 1x RGB led )
 byte blinkdelay[3]; //this array will store the information to be used for the blink speed (in this case it is a delay between blinks)
 byte Nofblinks[3]; //this array will store the information to be used for the number of triggers/blinks to do (number of repeated triggers)
 byte BRIGHTU[3]; // This array will store the information regarding the colour of the RGB led for an incoming upvote trigger (3 values for the colour)
 byte BRIGHTF[3];// This array will store the information regarding the colour of the RGB led for an incoming follower trigger (3 values for the colour)
 byte BRIGHTP[3]; // This array will store the information regarding the colour of the RGB led for an incoming post trigger (3 values for the colour)
 byte EFFECT[3]; // This array will store the information regarding the effect chosen for each led/notification (1 for normal Blink, 2 for Fading, 3 for rainbow, 4 for ice, 5 for fire)
unsigned long StartTime; //will be used to calculate timing for fading effect
unsigned long CurrentTime; //will be used to calculate timing for fading effect

// 'Angler fish logo oriented to right', 64x32px
const unsigned char ANGLER_TO_RIGHT [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x0c, 0x02, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0xe0, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x18, 0x01, 0x00, 
  0x00, 0x00, 0x00, 0x7f, 0xe0, 0x10, 0x01, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x10, 0x01, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xff, 0x10, 0x01, 0x80, 0x03, 0x80, 0x07, 0xff, 0xff, 0xf0, 0x01, 0x80, 
  0x03, 0xe0, 0x07, 0xff, 0xff, 0xf0, 0x01, 0x80, 0x01, 0xf8, 0x07, 0xff, 0xff, 0xf8, 0x01, 0xc0, 
  0x01, 0xfe, 0x03, 0xff, 0xff, 0xfc, 0x01, 0xc0, 0x00, 0xff, 0x8f, 0xff, 0xff, 0xfe, 0x01, 0xc0, 
  0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 
  0x00, 0xff, 0x9f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0xfc, 0x03, 0xff, 0xff, 0xf8, 0x00, 0x00, 
  0x00, 0xf8, 0x0f, 0xff, 0xff, 0xd0, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0xff, 0xff, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x30, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xf5, 0x70, 0x00, 
  0x00, 0x00, 0x07, 0xbf, 0xff, 0xf7, 0xf8, 0x00, 0x00, 0x00, 0x02, 0x1f, 0xff, 0xff, 0xf8, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00
};

// 'Angler fish logo oriented to left', 64x32px
const unsigned char ANGLER_TO_LEFT [] PROGMEM = {
 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x20, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x30, 0x07, 0xe0, 0x00, 0x00, 0x00, 
  0x00, 0x40, 0x10, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x18, 0x07, 0xf8, 0x00, 0x00, 0x00, 
  0x00, 0x80, 0x08, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x80, 0x08, 0x7f, 0xff, 0x80, 0x00, 0x00, 
  0x01, 0x80, 0x08, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x0f, 0xff, 0xff, 0xe0, 0x01, 0xc0, 
  0x01, 0x80, 0x0f, 0xff, 0xff, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x1f, 0xff, 0xff, 0xe0, 0x1f, 0x80, 
  0x03, 0x80, 0x3f, 0xff, 0xff, 0xc0, 0x7f, 0x80, 0x03, 0x80, 0x7f, 0xff, 0xff, 0xf1, 0xff, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x00, 
  0x00, 0x00, 0x3f, 0xff, 0xff, 0xf9, 0xff, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xc0, 0x3f, 0x00, 
  0x00, 0x00, 0x0b, 0xff, 0xff, 0xf0, 0x1f, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xf0, 0x07, 0x00, 
  0x00, 0x0c, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x0e, 0xaf, 0xff, 0xff, 0xe0, 0x00, 0x00, 
  0x00, 0x1f, 0xef, 0xff, 0xfd, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x40, 0x00, 0x00, 
  0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00
};

void setup(){
    Serial.begin(9600);//initialize serial comunication
    Serial.flush();    
        pinMode(RedPIN, OUTPUT); 
        pinMode(GreenPIN, OUTPUT);//set the selected pins as output 
        pinMode(BluePIN, OUTPUT); 
      
   ////////////////////////////////////////////////////////////
   //   initialize with the I2C addr 0x3C (for the 128x64)   //
   //    check your display address and modify accordingly   //
   ////////////////////////////////////////////////////////////  
   
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  // init done
  // Show image buffer on the display hardware.
  // Since the buffer has been intialized with an the Blinkit logo splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
}
 
void loop(){ //collecting the incoming data and filling the buffer with it
       StartTime = millis(); // updating time
  if (Serial.available() > 0) { //if there is any incoming serial data of any sort (any setting or any notification)
       display.setTextSize(2);
       display.setTextColor(WHITE);
       display.setCursor(25,20);
       display.println(F("INCOMING    DATA")); //advise on display that new data is incoming
       display.display();
       int index=0;
       delay(100); // let the buffer fill up
       int numChar = Serial.available(); 
    if (numChar>22) {
       numChar=22;
       }
     while (numChar--) {
        buffer[index++] = Serial.read(); //read serial data and put them in the buffer
        }
        splitString(buffer);
   }
 if (Serial.available() == NULL){    //
    for (int r=1; r<=10; r++){          // if within 10 seconds  
      if (Serial.available() == NULL){  // no serial data is detected
        delay (1000);                    //(thus no settings or no notification) -> activate the function breath
        }
      else{ // if serial data is detected then stop the process
        break;
        }
        }
        
     standby(); // activate the function called standby (standby fading light pattern and display slide)
     
     }
}

void standby(){ 
  display.stopscroll(); //stop any previous scroll (just to make sure)
  display.clearDisplay();// clear the display
  
   int redIntensity = 0;   //variable to handle the brightness of the RED colour of the RGB led
   int blueIntensity = 0;  //variable to handle the brightness of the BLUE colour of the RGB led
   int greenIntensity = 0; //variable to handle the brightness of the GREEN colour of the RGB led

while (Serial.available() == NULL){ //run below fading pattern till some data is detected over serial
               
 for (int i = 0; i <=64; i+=1){ //This loop will light up/dim linearly the led till it goes from off state to a value close to
                                 //R=128 G=0 B=255 the starting value for our fading pattern
   if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display 
      break; //and exit the loop
      }
      
      redIntensity = 2*i;
      blueIntensity = 4*i ;

      if(blueIntensity>=255){
        blueIntensity=255;
      }

      
       
      analogWrite(RedPIN, redIntensity);     //
      analogWrite(GreenPIN, greenIntensity);  //activate with proper delay the led using the currently stored values for R G B
      analogWrite(BluePIN, blueIntensity);   //      

      //////////////////////////////////////////////////
      //   slide the Blinkit text on the display      //
      //////////////////////////////////////////////////
      
       int PERIOD= 4000;
       double OMEGA= 2*PI/PERIOD;
       CurrentTime= millis();
       int y=((LCD_HEIGHT/3)+(LCD_HEIGHT/3)*(cos(OMEGA*CurrentTime))); //use the COS function to let the text oscillate up and down following the sine wave
       display.setTextSize(2); //set text size
       display.setTextColor(WHITE);//set colour of text
       display.setCursor(25,y); //set where to write the text
       display.println(F("BLINKiT"));// display the text
       display.display();
       display.clearDisplay();//clear display
      
   }
      
 for (int i =0; i <=128; i+=1){    
   
  if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }

      ///////////////////////////////////////////////////////////////////////////////////////////////
      //     first transition from R=128 G=0 B=255 (darker Violet) to R=255 G=10 B=255             //
      //(much brighter Violet) and have chosen to use the redIntensity as the variable part of the //    
      // FOR loop based on the value of R = i+ (shifting) we will calculate the Green brightness.  //
      ///////////////////////////////////////////////////////////////////////////////////////////////
      
    
       redIntensity = i+127;
       greenIntensity = ((118937/10000)*log(i+127)-(559050/10000)); //The neperien logarithmic function usually wrote as ln(x) 
                                                              //is not recognized in Arduino coding by using ln but log 
    if(greenIntensity> 10){ //based on the aproximation in calculations we do this just in case not to pass the value
        greenIntensity =10;
      }
    if(greenIntensity< 2){ //based on the aproximation in the calculations this serves to not let
        greenIntensity =0;   //the Green pin turns negative 
      }
  

    analogWrite(RedPIN, redIntensity);     //
    analogWrite(GreenPIN, greenIntensity); //activate with proper delay the led using the currently stored values for R G B
    analogWrite(BluePIN, blueIntensity);   //     

       unsigned int PERIOD= 4000;
       double OMEGA= 2*PI/PERIOD;
       CurrentTime= millis();
       int y=((LCD_HEIGHT/3)+(LCD_HEIGHT/3)*(cos(OMEGA*CurrentTime)));  //use the COS function to let the text oscillate up and down following the sine wave
       int x= i-60;
       display.drawBitmap(x, y, ANGLER_TO_RIGHT, 64, 32, 1); 
       display.setTextSize(2); //set text size
       display.setTextColor(WHITE);//set colour of text
       display.setCursor(25,y); //set where to write the text
       display.println(F("BLINKiT")); // display the text
       display.display();
       display.clearDisplay(); //clear display                    
}

 ////////////////////////////////////////////////////////////////////
 // linear transition between the bright Violet (R=255 G=10 B=255) //
 //       and the White colour (R=255 G=200 B=255)                 //
 ////////////////////////////////////////////////////////////////////
  for(int i=5; i<=100; i++){

    if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
      
     redIntensity=255;
     blueIntensity=255;
     greenIntensity= 2*i;

     analogWrite(RedPIN, redIntensity);     //
     analogWrite(GreenPIN, greenIntensity); //activate with proper delay the led using the currently stored values for R G B
     analogWrite(BluePIN, blueIntensity);   // 
       
       int PERIOD= 4000;
       double OMEGA= 2*PI/PERIOD;
       CurrentTime= millis();
       int y=((LCD_HEIGHT/3)+(LCD_HEIGHT/3)*(cos(OMEGA*CurrentTime))); //use the COS function to let the text oscillate up and down following the sine wave
       int x= i+63;
       display.drawBitmap(x, y, ANGLER_TO_RIGHT, 64, 32, 1); //display the logo
       display.setTextSize(2); //set text size
       display.setTextColor(WHITE);//set colour of text
       display.setCursor(25,y); //set where to write the text
       display.println(F("BLINKiT")); // display the text
       display.display();
       display.clearDisplay(); //clear display     
       
    }  
  
 ///////////////////////////////////////////////////////////////////    
 //linear transition between the White colour (R=255 G=220 B=255) //
 //       and the bright Violet (R=255 G=10 B=255)                //
 ///////////////////////////////////////////////////////////////////
 
    for(int i=100; i>=5; i--){

    if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
 
        redIntensity=255;
        blueIntensity=255;
        greenIntensity= 2*i;

     analogWrite(RedPIN, redIntensity);     //
     analogWrite(GreenPIN, greenIntensity); //activate with proper delay the led using the currently stored values for R G B
     analogWrite(BluePIN, blueIntensity);   // 
       
       int PERIOD= 4000;
       double OMEGA= 2*PI/PERIOD;
       CurrentTime= millis();
       int y=((LCD_HEIGHT/3)+(LCD_HEIGHT/3)*(cos(OMEGA*CurrentTime))); //use the COS function to let the text oscillate up and down following the sine wave
       int x= i+28;
       display.drawBitmap(x, y, ANGLER_TO_LEFT, 64, 32, 1);//display the logo
       display.setTextSize(2); //set text size
       display.setTextColor(WHITE);//set colour of text
       display.setCursor(25,y); //set where to write the text
       display.println(F("BLINKiT")); // display the text
       display.display();
       display.clearDisplay(); //clear display     
       
    }

////////////////////////////////////////////////////////////////////////
// below we reverse the loop used before to fade from Violet to White // 
//    and use it to do the other way (from White to Violet)           //
////////////////////////////////////////////////////////////////////////

for (int i = 128; i >=0; i-=1){
                                 
  if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
     

      ///////////////////////////////////////////////////////////////////
      //       transition from R=255 G=10 B=255 to R=127 G=0 B=255     //
      ///////////////////////////////////////////////////////////////////

       redIntensity = i+127;
       greenIntensity = ((118937/10000)*log(i+127)-(559050/10000)); //The neperien logarithmic function usually wrote as ln(x) 
                                                                    //is not recognized in Arduino coding by using ln but log 
    if(greenIntensity>= 10){ //based on the aproximation in calculations we do this just in case not to pass the value
        greenIntensity =10;
      }
    if(greenIntensity< 2){ //based on the aproximation in the calculations this serves to not let
        greenIntensity =0;   //the Green pin turns negative 
      }

      analogWrite(RedPIN, redIntensity);     //
      analogWrite(GreenPIN, greenIntensity); //activate with proper delay the led using the currently stored values for R G B
      analogWrite(BluePIN, blueIntensity);   // 

       unsigned int PERIOD= 4000;
       double OMEGA= 2*PI/PERIOD;
       CurrentTime= millis();
       int y=((LCD_HEIGHT/3)+(LCD_HEIGHT/3)*(cos(OMEGA*CurrentTime))); //use the COS function to let the text oscillate up and down following the sine wave
       int x= i-95;
       display.drawBitmap(x, y, ANGLER_TO_LEFT, 64, 32, 1);//display the logo
       display.setTextSize(2); //set text size
       display.setTextColor(WHITE);//set colour of text
       display.setCursor(25,y); //set where to write the text
       display.println(F("BLINKiT")); // display the text
       display.display();
       display.clearDisplay(); //clear display   
      }




for (int i = 63; i >=0; i-=1){ //This loop will dim linearly the led till it goes totally off
                                //since we went back to R=127 G=0 B=255 with a quick calculation and a good aproximation 
                                //we set the step to take for each brightness decrease so that they get dimmed simultaneously and of the same ammount each time

if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
      
    redIntensity = i*2;
    blueIntensity = i*4 ;


        analogWrite(RedPIN, redIntensity);     //
        analogWrite(GreenPIN, greenIntensity); //activate with proper delay the led using the currently stored values for R G B
        analogWrite(BluePIN, blueIntensity);   //  

     unsigned int PERIOD= 4000;
       double OMEGA= 2*PI/PERIOD;
       CurrentTime= millis();
       int y=((LCD_HEIGHT/3)+(LCD_HEIGHT/3)*(cos(OMEGA*CurrentTime))); //use the COS function to let the text oscillate up and down following the sine wave
       display.setTextSize(2); //set text size
       display.setTextColor(WHITE);//set colour of text
       display.setCursor(25,y); //set where to write the text
       display.println(F("BLINKiT")); // display the text
       display.display();
       display.clearDisplay(); //clear display  
       }
   }
} 

void splitString(char* data) { //splitting the data received based on a fixed separator (in this case the blank space " ")
       Serial.print("Data entered: "); 
       Serial.println(data); // display data entered/received
       display.clearDisplay();
       char* info;
       info = strtok (data, " "); //using strtok split in tokens the data entered in a sequences of contiguous characters separated by the delimiter " " (empty space)
  while (info != NULL) {
        setallsettings(info);
        setLEDon(info);
        info = strtok (NULL, " ");
        }
// Clear the text and serial buffers
   for (int x=0; x<21; x++) {
       buffer[x]='\0';
     }
  Serial.flush();
}

//here we will store the splitted data received over the serial comunication to store our set-ups
void setallsettings(char* data) { 
  
if (data[0] == 'm') {  // if incoming data has marker "m" store in the array lemode the mode ( m1 for 3x single colour led, m2 for 1x RGB led)
       int blinkdata = strtol(data+1, NULL, 10);
       blinkdata = constrain(blinkdata,1,2); 
       transferdata[y] = blinkdata;
       y=y+1;
  if (y==1){ // when transferdata is full of data (we expect 1 data for this setting)
       memcpy(ledmode, transferdata, sizeof(transferdata)); //copying transferdata to ledmode then clearing transferdata and resetting y, 
    for (int i=0; i<=sizeof(transferdata); i++){
       transferdata[i]= '\0';
       }
       y=0;
   }
}
if (data[0] == 'n') { //if incoming data has marker "n" store in an array the number of repeated triggers we want for each led/notification (ex. n10 n15 n1)
       int blinkdata = strtol(data+1, NULL, 10);
       blinkdata = constrain(blinkdata,1,30); 
       transferdata[y] = blinkdata;
       y=y+1;
  if (y==3){                                       // when transferdata is full of data (we expect 3 values)
       memcpy(Nofblinks, transferdata, sizeof(transferdata));   //copying transferdata to Nofblinks then clearing transferdata and resetting y, 
    for (int i=0; i<=sizeof(transferdata); i++){
       transferdata[i]= '\0';
       }
       y=0;
   }
}
if (data[0] == 'd') { //if incoming data has marker "d" store in an array called blinkdelay the delay/speed in the blinking/trigger
        int blinkdata = strtol(data+1, NULL, 10);
        blinkdata = constrain(blinkdata,1,1000); 
        transferdata[y] = blinkdata;
        y=y+1;
  if (y==3){                                                // when transferdata is full of data (we expect 3 values)
        memcpy(blinkdelay, transferdata, sizeof(transferdata));   //copying transferdata to blinkdelay then clearing transferdata and resetting y, 
    for (int i=0; i<=sizeof(transferdata); i++){
        transferdata[i]= '\0';
        }
       y=0;
   }
}
if (data[0] == 'U') { //if incoming data has marker "U" store in an array called BRIGHTU the rgb colour code assigned for upvote (ex. U255 U0 U0 for red)
        int blinkdata = strtol(data+1, NULL, 10);
        blinkdata = constrain(blinkdata,0,255); 
        transferdata[y] = blinkdata;
        y=y+1;
  if (y==3){                                                // when transferdata is full of data (we expect 4 values, 3 regarding the colour code and the fourth for the effect)
        memcpy(BRIGHTU, transferdata, sizeof(transferdata));   //copying transferdata to BRIGHTU then clearing transferdata and resetting y, 
     for (int i=0; i<=sizeof(transferdata); i++){
        transferdata[i]= '\0';
        }
       y=0;
    }
}
if (data[0] == 'F') { //if incoming data has marker "F" store in an array called BRIGHTF the rgb colour code assigned for follower (ex. F0 F255 F0 for green)
        int blinkdata = strtol(data+1, NULL, 10);
        blinkdata = constrain(blinkdata,0,255); 
        transferdata[y] = blinkdata;
        y=y+1;
  if (y==3){                                                // when transferdata is full of data (we expect 4 values, 3 regarding the colour code and the fourth for the effect)
        memcpy(BRIGHTF, transferdata, sizeof(transferdata));   //copying transferdata to BRIGHTF then clearing transferdata and resetting y, 
    for (int i=0; i<=sizeof(transferdata); i++){
        transferdata[i]= '\0';
       }
      y=0;
    }
}
if (data[0] == 'P') {  //if incoming data has marker "P" store in an array called BRIGHTP the rgb colour code assigned for post (ex. P0 P0 P255 for blue)
        int blinkdata = strtol(data+1, NULL, 10);
        blinkdata = constrain(blinkdata,0,255); 
        transferdata[y] = blinkdata;
        y=y+1;
  if (y==3){                                                // when transferdata is full of data (we expect 4 values, 3 regarding the colour code and the fourth for the effect)
        memcpy(BRIGHTP, transferdata, sizeof(transferdata));   //copying transferdata to BRIGHTP then clearing transferdata and resetting y, 
    for (int i=0; i<=sizeof(transferdata); i++){
        transferdata[i]= '\0';
        }
      y=0;
   }
}
if (data[0] == 'E') {  //if incoming data has marker "E" store in an array called EFFECT the effect chosen for each notification
        int blinkdata = strtol(data+1, NULL, 10);
        blinkdata = constrain(blinkdata,1,5); 
        transferdata[y] = blinkdata;
        y=y+1;
  if (y==3){                                                // when transferdata is full of data (we expect 3 values, one for each notification)
        memcpy(EFFECT, transferdata, sizeof(transferdata));   //copying transferdata to EFFECT then clearing transferdata and resetting y, 
    for (int i=0; i<=sizeof(transferdata); i++){
        transferdata[i]= '\0';
        }
      y=0;
   }
}
 
if (data[0] == '?'){   //send command set to view on serial all the current setups
      Serial.println(F("Current Settings: "));
      Serial.println(F("---------------------------------------------------- "));
      Serial.println(F(" "));  
if(ledmode[0]== 1){     
        Serial.println(F("ledmode is set to: 3x single colour LED "));
        Serial.println(F(" "));  
        
    for (int i=0;i<=2;i++){
        Serial.print(F("effect for led "));
        Serial.print(i+1);
        Serial.print(F(" is set to: "));
      if(EFFECT[i]==1){
        Serial.println(F("NORMAL BLINK"));
        }
      if(EFFECT[i]==2){
        Serial.println(F("FADING"));
        }
      }
      Serial.println(F(" "));
    
  for (int i=0;i<=2;i++){
      Serial.print(F("Nofblinks for led"));
      Serial.print(i+1);
      Serial.print(F(" is:"));
      Serial.print(Nofblinks[i]);
      Serial.println(F(" Blinks"));
    if (i==2){
      Serial.println(F(" "));
      }
   }     
  for (int i=0;i<=2;i++){
        Serial.print(F("blinkdelay for led "));
        Serial.print(i+1);
        Serial.print(F(" is:"));
        Serial.print(blinkdelay[i]);
        Serial.println(F(" Millis"));
     if (i==2){
        Serial.println(F(" "));
        }
   }
}
   if(ledmode[0]==2){
              Serial.println(F("ledmode is set to: 1x RGB LED "));
              Serial.println(F(" "));
                      
              Serial.print(F("number of blinks for upvote trigger is: "));
              Serial.print(Nofblinks[0]);
              Serial.println(F(" Blinks"));
              Serial.print(F("number of blinks for follower trigger is: "));
              Serial.print(Nofblinks[1]);
              Serial.println(F(" Blinks"));
              Serial.print(F("number of blinks for post trigger is: "));
              Serial.print(Nofblinks[2]);
              Serial.println(F(" Blinks"));
              Serial.println(F(" "));
        
              Serial.print(F("blinkdelay for upvote trigger is: "));
              Serial.print(blinkdelay[0]);
              Serial.println(F(" Millis"));
              Serial.print(F("blinkdelay for follower trigger is: "));
              Serial.print(blinkdelay[1]);
              Serial.println(F(" Millis"));
              Serial.print(F("blinkdelay for post trigger is: "));
              Serial.print(blinkdelay[2]);
              Serial.println(F(" Millis"));
              Serial.println(F(" "));
              
              Serial.println(F("RGB value for upvote trigger is set to"));
              Serial.println(F(" "));
              Serial.print(F("R= "));
              Serial.println(BRIGHTU[0]);
              Serial.print(F("G= "));
              Serial.println(BRIGHTU[1]);
              Serial.print(F("B= "));
              Serial.println(BRIGHTU[2]);
              Serial.println(F(""));
             
              if (EFFECT[0]==1){
                Serial.println(F("Effect for upvote is: NORMAL BLINKING"));
                Serial.println(F(" "));
                 }
              if (EFFECT[0]==2){
                Serial.println(F("Effect for upvote is: FADING"));
                Serial.println(F(" "));
                 }
              if (EFFECT[0]==3){
                Serial.println(F("Effect for upvote is: RAINBOW"));
                Serial.println(F(" "));
              }
              if (EFFECT[0]==4){
                Serial.println(F("Effect for upvote is: ICE"));
                Serial.println(F(" "));
              }
              if (EFFECT[0]==4){
                Serial.println(F("Effect for upvote is: FIRE 1"));
                Serial.println(F(" "));
              }
                          
              Serial.println(F("RGB value for follower trigger is set to"));
              Serial.println(F(" "));
              Serial.print(F("R= "));
              Serial.println(BRIGHTF[0]);
              Serial.print(F("G= "));
              Serial.println(BRIGHTF[1]);
              Serial.print(F("B= "));
              Serial.println(BRIGHTF[2]);
              Serial.println(F(""));
              if (EFFECT[1]==1){
                Serial.println(F("Effect for follower is: NORMAL BLINKING"));
                Serial.println(F(" "));
                 }
              if (EFFECT[1]==2){
                Serial.println(F("Effect for follower is: FADING"));
                Serial.println(F(" "));
                 }
              if (EFFECT[1]==3){
                Serial.println(F("Effect for follower is: RAINBOW"));
                Serial.println(F(" "));
              }
              if (EFFECT[1]==4){
                Serial.println(F("Effect for follower is: ICE"));
                Serial.println(F(" "));
              }
              if (EFFECT[1]==5){
                Serial.println(F("Effect for follower is: FIRE 1"));
                Serial.println(F(" "));
              }
                            
              Serial.println(F("RGB value for post trigger is set to"));
              Serial.println(F(" "));
              Serial.print(F("R= "));
              Serial.println(BRIGHTP[0]);
              Serial.print(F("G= "));
              Serial.println(BRIGHTP[1]);
              Serial.print(F("B= "));
              Serial.println(BRIGHTP[2]);
              Serial.println(F(""));
              if (EFFECT[2]==1){
                Serial.println(F("Effect for post is: NORMAL BLINKING"));
                Serial.println(F(" "));
                }
              if (EFFECT[2]==2){
                Serial.println(F("Effect for post is: FADING"));
                Serial.println(F(" "));
               }
              if (EFFECT[2]==3){
                Serial.println(F("Effect for post is: RAINBOW"));
                Serial.println(F(" "));
              }  
              if (EFFECT[2]==4){
                Serial.println(F("Effect for post is: ICE"));
                Serial.println(F(" "));
              }
              if (EFFECT[2]==5){
                Serial.println(F("Effect for post is: FIRE 1"));
                Serial.println(F(" "));
              }
         }
    }
}

void setLEDon(char* data) {
  //if Arduino receives the command u from serial (upvote detected)
if (data[0] == 'u') { 
      //if ledmode is set to 3 single colour led mode      
   if(ledmode[0]==1){ 
           // if the effect selected is normal blinking
       if (EFFECT[0]== 1){ 
          blink1(RedPIN, Nofblinks[0], blinkdelay[0]);
         }
           // if the effect selected is fading effect
       if (EFFECT[0]== 2){ 
          fade1(RedPIN, Nofblinks[0], blinkdelay[0]); 
         }
      }
     // if ledmode is set to rgb led mode 
  if(ledmode[0]==2){
        // if the effect selected is normal blinking
    if (EFFECT[0]== 1){ 
        blink2(Nofblinks[0], blinkdelay[0], BRIGHTU[0], BRIGHTU[1], BRIGHTU[2]);
       }
        // if the effect selected is fading effect
    if (EFFECT[0]== 2){ 
       unsigned int P = (blinkdelay[0]*10);
       double O = 2*PI/P; 
       fade2(P, O, Nofblinks[0], BRIGHTU[0], BRIGHTU[1], BRIGHTU[2]);               
      }
        // if the effect selected is rainbow effect          
    if (EFFECT[0]== 3){ 
       rainbow( Nofblinks[0], blinkdelay[0]);
      }
       // if the effect selected is ice effect
    if (EFFECT[0]==4){ 
      ice(Nofblinks[0], blinkdelay[0]);
      }
        // if the effect selected is fire effect
    if (EFFECT[0]==5){ 
      fire(Nofblinks[0], blinkdelay[0]);
      }  
  }
       
       display.setTextSize(2);
       display.setTextColor(WHITE);
       display.setCursor(30,20);
       display.println(F(" NEW      UPVOTE"));
       display.display();
       display.startscrollright(0x00, 0x0F); 
       display.clearDisplay();
 Serial.println(F("you got an upvote "));
 Serial.println(F(" ")); 
 
}
      
//if Arduino receives the command f from serial (follower)   
if (data[0] == 'f') { 
      // if ledmode is set to 3 single colour led mode    
   if(ledmode[0]==1){  
            // if the effect selected is normal blinking  
        if (EFFECT[1]== 1){ 
           blink1(GreenPIN, Nofblinks[1], blinkdelay[1]);
          }
            // if the effect selected is fading effect
        if (EFFECT[1]== 2){ 
           fade1(GreenPIN, Nofblinks[1], blinkdelay[1]); 
          }
     }
      // if ledmode is set to 1 RGB led
   if(ledmode[0]==2){ 
           // if the effect selected is normal blinking
       if (EFFECT[1]== 1){ 
           blink2(Nofblinks[1], blinkdelay[1], BRIGHTF[0], BRIGHTF[1], BRIGHTF[2]);
          }
           // if the effect selected is fading effect
       if (EFFECT[1]== 2){ 
          unsigned int P = (blinkdelay[1]*10);
          double O = 2*PI/P; 
          fade2(P, O, Nofblinks[1], BRIGHTF[0], BRIGHTF[1], BRIGHTF[2]);             
          }
           // if the effect selected is rainbow effect
       if (EFFECT[1]== 3){ 
          rainbow(Nofblinks[1], blinkdelay[1]);
          }
           // if the effect selected is ice effect
       if (EFFECT[1]==4){ 
          ice(Nofblinks[1], blinkdelay[1]);
          }
           // if the effect selected is fire effect
       if (EFFECT[1]==5){ 
          fire(Nofblinks[1], blinkdelay[1]);
         }  
  }
       display.setTextSize(2);
       display.setTextColor(WHITE);
       display.setCursor(30,20);
       display.println(F(" NEW     FOLLOWER"));
       display.display();
       display.startscrollright(0x00, 0x0F); 
       display.clearDisplay();
 Serial.println(F("you got a new follower "));
 Serial.println(F(" ")); 
}
     
  
   //if Arduino receives the command p from serial (new post) 
if (data[0] == 'p') { 
       // if ledmode is set to 3x single led mode
    if(ledmode[0]==1){  
             // if the effect selected is normal blinking  
         if (EFFECT[2]== 1){ 
            blink1(BluePIN, Nofblinks[2], blinkdelay[2]);
            }
             // if the effect selected is fading effect
         if (EFFECT[2]== 2){ 
            fade1(BluePIN, Nofblinks[2], blinkdelay[2]);  
            }
        }
       //if ledmode is set to RGB led mode 
    if(ledmode[0]==2){
             // if the effect selected is normal blinking
         if (EFFECT[2]== 1){ 
             blink2(Nofblinks[2], blinkdelay[2], BRIGHTP[0], BRIGHTP[1], BRIGHTP[2]);
            }
             // if the effect selected is fading effect
         if (EFFECT[2]== 2){ 
             unsigned int P = (blinkdelay[2]*10);
             double O = 2*PI/P; 
             fade2(P, O, Nofblinks[2], BRIGHTP[0], BRIGHTP[1], BRIGHTP[2]);                 
            }
             // if the effect selected is rainbow effect
         if (EFFECT[2]== 3){ 
             rainbow( Nofblinks[2], blinkdelay[2]);
            }
             // if the effect selected is ice effect
         if (EFFECT[2]==4){ 
            ice(Nofblinks[2], blinkdelay[2]);
           }
           // if the effect selected is fire effect
        if (EFFECT[2]==5){ 
            fire(Nofblinks[2], blinkdelay[2]);
           }
     } 
       display.setTextSize(2);
       display.setTextColor(WHITE);
       display.setCursor(30,20);
       display.println(F("NEW       POST"));
       display.display();
       display.startscrollright(0x00, 0x0F); 
       display.clearDisplay();
      Serial.println(F("there is a new post "));
      Serial.println(F(" ")); 
  }
}

void blink1(int Pin, int Nblink, int LEDdelay ){
             display.stopscroll();
             display.clearDisplay();
             display.setTextSize(2);
             display.setTextColor(WHITE);
             display.setCursor(20,20);
             display.println(F("BLINKING"));
             display.display();

   for (int a= 0; a< Nblink; a++){ //repeat the blinking effect as per the Nofblinks[0] value
     if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
                    
              //do the blinking               
                 analogWrite(Pin, 255); //turn on the led 
                 display.invertDisplay(true);
                 delay(LEDdelay);
                 analogWrite(Pin, 0);//turn off the led
                display.invertDisplay(false);
                 delay(LEDdelay);
                 display.clearDisplay(); 
             }
}

void fade1(int Pin, int Nblink, int LEDdelay ){

         display.stopscroll();
         display.clearDisplay(); 
         display.setTextSize(2);
         display.setTextColor(WHITE);
         display.setCursor(25,20);
         display.println(F("FADING"));
         display.display();
    for (int a= 0; a< Nblink; a++){ //repeat the fading effect as per the Nofblinks[2] value
           
             
           for (int f=0; f<=255; f++){
            if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }    
                            
              //do the fading
                analogWrite(Pin, f);  //fade in the led
                delay((LEDdelay/20));

                }
                display.invertDisplay(true);
       for (int f=255; f>=0; f--){
             if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
                analogWrite(Pin, f);  //fade out the led
                delay((LEDdelay/20));
                } 
                display.invertDisplay(false);
                display.clearDisplay(); 
          }
}

void blink2(int Nblink, int LEDdelay, int R, int G, int B ){
             display.stopscroll();
             display.clearDisplay();
             display.setTextSize(2);
             display.setTextColor(WHITE);
             display.setCursor(20,20);
             display.println(F("BLINKING"));
             display.display();

    for (int a= 0; a< Nblink; a++){ //repeat the fading effect as per the Nofblinks[2] value
         if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
                 
          //do the blinking  
           analogWrite(RedPIN, R);
           analogWrite(GreenPIN, G); //turn on the led using the stored R G B values
           analogWrite(BluePIN, B);
           display.invertDisplay(true);
           delay(LEDdelay);
           analogWrite(RedPIN, 0);
           analogWrite(GreenPIN, 0);//turn of the led
           analogWrite(BluePIN, 0);
           display.invertDisplay(false);
           delay(LEDdelay);
           display.clearDisplay();
      }
}

void fade2(unsigned int PERIOD, double OMEGA, int Nblink, int R, int G, int B ){
             display.stopscroll();
             display.clearDisplay(); 
             display.setTextSize(2);
             display.setTextColor(WHITE);
             display.setCursor(25,20);
             display.println(F("FADING"));
             display.display();
             
       int count=1;// variable to count the number of oscillations
       StartTime= millis();
     
      while (count <= Nblink){ //loop to be used to repeat the pattern as per Nofblinks[1] value
            if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
                 
                CurrentTime= millis();
               /////////////////////////////////////////////////////
               //  led, set to an R G B value will fade using a   //
               // sine wave from 0% brightness to 100% brightness //
               /////////////////////////////////////////////////////

               int FadeRed= ((R/2) + (R/2)*(cos(OMEGA*CurrentTime)));    ///////////////////////////////////////////////
               int FadeGreen= ((G/2) + (G/2)*(cos(OMEGA*CurrentTime)));  // let the value of the brightness oscillate //
               int FadeBlue= ((B/2) + (B/2)*(cos(OMEGA*CurrentTime)));   // between 0% and 100% of the set RGB colour //
                                                                         //     with the variation of a sine wave     //
                                                                         ///////////////////////////////////////////////

               if ((unsigned long)(millis() - StartTime) >= PERIOD){ ///////////////////////////////////////////////////////////////////////////
                count++;                                             // every full oscillation increase the counter and take the current time //
                StartTime= millis();                                 //  as the new starting point to calculate next period of oscilation     //
                                                                     ///////////////////////////////////////////////////////////////////////////
                }
               
                analogWrite(RedPIN, FadeRed);
                analogWrite(GreenPIN, FadeGreen);// activate the led
                analogWrite(BluePIN, FadeBlue);
                display.invertDisplay(true);

               if (count == Nblink ){
                analogWrite(RedPIN, 0);
                analogWrite(GreenPIN, 0);// turn off the led
                analogWrite(BluePIN, 0);
                display.invertDisplay(false);
                display.clearDisplay();
                break;
                }
          }
}

void rainbow(int Nblink, int LEDdelay){
           
         int redrainbow;
         int greenrainbow;
         int bluerainbow;
             display.stopscroll();
             display.clearDisplay(); 
             display.setTextSize(2);
             display.setTextColor(WHITE);
             display.setCursor(25,20);
             display.println(F("RAINBOW"));
             display.display();
             
   for (int a= 0; a< Nblink; a++){  //repeat the rainbow effect as per the Nofblinks[0] value
      for (int y = 0; y < 768; y++){
        
        if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
       if (y <= 255){          //zone1
             redrainbow = 255 - y;    // red goes from on to off
             greenrainbow = y;        // green goes from off to on
             bluerainbow = 0;         // blue is always off
             }
       else if (y <= 511){     // zone 2
             redrainbow = 0;                 // red is always off
             greenrainbow = 255 - (y - 256); // green on to off
             bluerainbow = (y - 256);        // blue off to on
             }
       else{ // color >= 512       // zone 3
             redrainbow = (y - 512);         // red off to on
             greenrainbow = 0;               // green is always off
             bluerainbow = 255 - (y - 512);  // blue on to off
             }
             
       /////////////////////////////////////////////////
       // Now that the Intensity values have been set,//
       // activate the LED with those values          //
       /////////////////////////////////////////////////
       
            analogWrite(RedPIN, redrainbow);
            analogWrite(BluePIN, bluerainbow);
            analogWrite(GreenPIN, greenrainbow);
            display.invertDisplay(true);
            delay((LEDdelay/30));                    
            }
         }
         
         //////////////////////////////////////////////
         // final off after the whole rainbow is done//
         //////////////////////////////////////////////
         
          delay(500);
          redrainbow = 0;    // red off
          greenrainbow = 0;        // green off
          bluerainbow = 0;             // blue off
          analogWrite(RedPIN, redrainbow);
          analogWrite(BluePIN, bluerainbow);
          analogWrite(GreenPIN, greenrainbow);
          display.invertDisplay(false);
          display.clearDisplay();
}

void ice(int Nblink, int LEDdelay){
             display.stopscroll();
             display.clearDisplay(); 
             display.setTextSize(2);
             display.setTextColor(WHITE);
             display.setCursor(30,20);
             display.println(F("ICE"));
             display.display();
  
        //repeat the ice effect as per Nblink value
   for (int a= 0; a< Nblink; a++){
       if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
          
        //loop for fade in effect  
    for (int y=0; y<=510; y++){
      if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
            display.invertDisplay(true);
           // fade in to blue-ice colour
        if(y<=255){
           analogWrite(RedPIN, 0); 
           analogWrite(GreenPIN, y); //
           analogWrite(BluePIN, y); //
          }
           //fade to white colour
        if(y>255){
           analogWrite(RedPIN, (y-255)); 
           analogWrite(GreenPIN, 255); //
           analogWrite(BluePIN, 255); //
          } 
         delay ((LEDdelay/20));
         
         
     }
    for (int x=510; x>=0; x--){
      if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
         display.invertDisplay(false);   
           //fade to blue-ice colour
        if(x>255){
           analogWrite(RedPIN, (x-255)); 
           analogWrite(GreenPIN, 255); //
           analogWrite(BluePIN, 255); //
          }    
           // fade out to off
        if(x<=255){
           analogWrite(RedPIN, 0);
           analogWrite(GreenPIN, x); //
           analogWrite(BluePIN, x); //
          }  
         delay ((LEDdelay/20));
                
     }
 }
     analogWrite(RedPIN, 0); // turn off the led
     analogWrite(GreenPIN, 0); // just to make sure it is off
     analogWrite(BluePIN, 0); //
     display.clearDisplay();
}

void fire(int Nblink, int LEDdelay){
             display.stopscroll(); //stop scrolling previous text (just in case)
             display.clearDisplay(); //clear the display
             display.setTextSize(2); //set text size
             display.setTextColor(WHITE); //set text colour
             display.setCursor(30,20); //set where to display text
             display.println(F("FIRE")); // write text
             display.display(); //display the text
             
 for (int a= 0; a< Nblink; a++){ //repeat the rainbow effect as per the Nofblinks[1] value
     for (int y=0; y<=295; y++){
           if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
      display.invertDisplay(true); //invert colours of display
          // fade in till full red brightness
       if(y<=255){
          analogWrite(RedPIN, y);
         }
          // fade to orange-yellow
       if(y>=255){
          analogWrite(GreenPIN, (y-255));
         }
          delay ((LEDdelay/20));  
    }
    display.invertDisplay(false); //invert again display colours
    for (int x=295; x>=0; x--){
           if (Serial.available() > 0){ // when data is received
      analogWrite(RedPIN, 0);    //turn off the led  
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      display.clearDisplay(); // clear display
      break; //and exit the loop
      }
          // fade back to red
       if(x>=255){
          analogWrite(GreenPIN, (x-255));
         }
          // fade till off
       if(x<=255){
          analogWrite(RedPIN, x);
         }     
      delay ((LEDdelay/20));  
    }
    display.invertDisplay(true); //invert again display colours
 }
     analogWrite(RedPIN, 0); // turn off the led
     analogWrite(GreenPIN, 0); // (just to make sure)
     display.invertDisplay(false); //bring colours to normality
     display.clearDisplay(); //clear display
}



