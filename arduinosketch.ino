char buffer[22];

int RedPIN = 3; //pin for led 1 (single colour led) or pin for the Red led in the RGB led
int GreenPIN = 5; //pin for led 2 (single colour led) or pin for the Green led in the RGB led
int BluePIN = 6 ; //pin for led 3 (single colour led) or pin for the Blue led in the RGB led
//above pins must be DIGITAL PWM PINS (in Arduino Uno, PIN 3, 5, 6, 9, 10, 11 are PWM) in order to obtain the colours shades with the RGB led
int y=0;
int transferdata[3]; //array to be used to store the incoming settings and transfer them to the right setting array
int ledmode[2]; //this array will store the information regarding the ledmode (mode 1 for 3x single colour leds and mode 2 for 1x RGB led )
int blinkdelay[3]; //this array will store the information to be used for the blink speed (in this case it is a delay between blinks)
int Nofblinks[3]; //this array will store the information to be used for the number of triggers/blinks to do (number of repeated triggers)
int BRIGHTU[3]; // This array will store the information regarding the brightness of the RGB led for the upvote trigger (3 values, 1 for for each colour to create the blend with the others)
int BRIGHTF[3];// This array will store the information regarding the brightness of the RGB led for the follower trigger (3 values, 1 for for each colour to create the blend with the others)
int BRIGHTP[3]; // This array will store the information regarding the brightness of the RGB led for the post trigger (3 values, 1 for for each colour to create the blend with the others)

void setup()
{
Serial.begin(9600);
Serial.flush();
    pinMode(RedPIN, OUTPUT); //set the selected pins as output 
    pinMode(GreenPIN, OUTPUT);
    pinMode(BluePIN, OUTPUT);
}
 
void loop() //collecting the incoming data and filling the buffer with it
{
if (Serial.available() > 0) {
int index=0;
delay(100); // let the buffer fill up
int numChar = Serial.available();
if (numChar>22) {
numChar=22;
}
while (numChar--) {
buffer[index++] = Serial.read();
}
splitString(buffer);
}
 
}
void splitString(char* data) { //splitting the data received based on a fixed separator (in this case the blank space " ")
//Serial.print("Data entered: "); 
//Serial.println(data); // display data entered/received
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
  
if (data[0] == 'm') {  // select and store in the array lemode the mode ( m1 for 3x single colour led, m2 for 1x RGB led)
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
  if (data[0] == 'n') { //store in an array called Nofblinks the sequence of the number of repeated trigger we want for each led and on which later leds will trigger
    int blinkdata = strtol(data+1, NULL, 10);
    blinkdata = constrain(blinkdata,1,30); 
    transferdata[y] = blinkdata;
    y=y+1;
    if (y==3){                                                // when transferdata is full of data (we expect 3 values)
    memcpy(Nofblinks, transferdata, sizeof(transferdata));   //copying transferdata to Nofblinks then clearing transferdata and resetting y, 
    for (int i=0; i<=sizeof(transferdata); i++){
    transferdata[i]= '\0';
    }
    y=0;
    }
  }
  if (data[0] == 'd') { //store in an array called blinkdelay the sequence of delays for each trigger
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
if (data[0] == 'U') {
    int blinkdata = strtol(data+1, NULL, 10);
    blinkdata = constrain(blinkdata,0,255); 
    transferdata[y] = blinkdata;
    y=y+1;
    if (y==3){                                                // when transferdata is full of data (we expect 3 values)
    memcpy(BRIGHTU, transferdata, sizeof(transferdata));   //copying transferdata to BRIGHTU then clearing transferdata and resetting y, 
    transferdata[0]= '\0';
    y=0;
    }
  }
if (data[0] == 'F') {
    int blinkdata = strtol(data+1, NULL, 10);
    blinkdata = constrain(blinkdata,0,255); 
    transferdata[y] = blinkdata;
    y=y+1;
    if (y==3){                                                // when transferdata is full of data (we expect 3 values)
    memcpy(BRIGHTF, transferdata, sizeof(transferdata));   //copying transferdata to BRIGHTF then clearing transferdata and resetting y, 
    transferdata[0]= '\0';
    y=0;
    }
  }
if (data[0] == 'P') {
    int blinkdata = strtol(data+1, NULL, 10);
    blinkdata = constrain(blinkdata,0,255); 
    transferdata[y] = blinkdata;
    y=y+1;
    if (y==3){                                                // when transferdata is full of data (we expect 3 values)
    memcpy(BRIGHTP, transferdata, sizeof(transferdata));   //copying transferdata to BRIGHTP then clearing transferdata and resetting y, 
    transferdata[0]= '\0';
    y=0;
    }
  }
if (data[0] == '?'){   //send command set to view on serial all the current setups
  Serial.println("Current Settings: ");
  Serial.println("---------------------------------------------------- ");
  Serial.println(" ");  
if(ledmode[0]== 1){     
        Serial.println("ledmode is set to: 3x single colour LED ");
        Serial.println(" ");   
  for (int i=0;i<=2;i++){
      Serial.print("Nofblinks for led");
      Serial.print(i+1);
      Serial.print(" is:");
      Serial.print(Nofblinks[i]);
      Serial.println(" Blinks");
    if (i==2){
      Serial.println(" ");
      }
   }     
  for (int i=0;i<=2;i++){
        Serial.print("blinkdelay for led ");
        Serial.print(i+1);
        Serial.print(" is:");
        Serial.print(blinkdelay[i]);
        Serial.println(" Millis");
     if (i==2){
        Serial.println(" ");
        }
   }
}
   if(ledmode[0]==2){
              Serial.println("ledmode is set to: 1x RGB LED ");
              Serial.println(" ");
              
              Serial.print("number of blinks for upvote trigger is: ");
              Serial.print(Nofblinks[0]);
              Serial.println(" Blinks");
              Serial.print("number of blinks for follower trigger is: ");
              Serial.print(Nofblinks[1]);
              Serial.println(" Blinks");
              Serial.print("number of blinks for post trigger is: ");
              Serial.print(Nofblinks[2]);
              Serial.println(" Blinks");
              Serial.println(" ");
        
              Serial.print("blinkdelay for upvote trigger is: ");
              Serial.print(blinkdelay[0]);
              Serial.println(" Millis");
              Serial.print("blinkdelay for follower trigger is: ");
              Serial.print(blinkdelay[1]);
              Serial.println(" Millis");
              Serial.print("blinkdelay for post trigger is: ");
              Serial.print(blinkdelay[2]);
              Serial.println(" Millis");
              Serial.println(" ");
              
              Serial.println("RGB value for upvote trigger is set to");
              Serial.println(" ");
              Serial.print("R= ");
              Serial.println(BRIGHTU[0]);
              Serial.print("G= ");
              Serial.println(BRIGHTU[1]);
              Serial.print("B= ");
              Serial.println(BRIGHTU[2]);
              Serial.println("");
              
              Serial.println("RGB value for follower trigger is set to");
              Serial.println(" ");
              Serial.print("R= ");
              Serial.println(BRIGHTF[0]);
              Serial.print("G= ");
              Serial.println(BRIGHTF[1]);
              Serial.print("B= ");
              Serial.println(BRIGHTF[2]);
              Serial.println("");
              
              Serial.println("RGB value for post trigger is set to");
              Serial.println(" ");
              Serial.print("R= ");
              Serial.println(BRIGHTP[0]);
              Serial.print("G= ");
              Serial.println(BRIGHTP[1]);
              Serial.print("B= ");
              Serial.println(BRIGHTP[2]);
              Serial.println("");
         }
    }
}

void setLEDon(char* data) {
  //if i receive the command u from serial (upvote)
if (data[0] == 'u') {             
  if(ledmode[0]==1){ //and if ledmode[0] is set to 1 (3x single colour led mode)
    for (int a= 0; a< Nofblinks[0]; a++){
      digitalWrite(RedPIN, HIGH);
      delay(blinkdelay[0]);
      digitalWrite(RedPIN, 0);
      delay(blinkdelay[0]);
       }
}
  if(ledmode[0]==2){
    for (int a= 0; a< Nofblinks[0]; a++){
      analogWrite(RedPIN, BRIGHTU[0]);
      analogWrite(GreenPIN, BRIGHTU[1]);
      analogWrite(BluePIN, BRIGHTU[2]);
      delay(blinkdelay[0]);
      analogWrite(RedPIN, 0);
      analogWrite(GreenPIN, 0);
      analogWrite(BluePIN, 0);
      delay(blinkdelay[0]);
    }
  }
 Serial.println("you got an upvote ");
 Serial.println(" "); 
}      
//if i receive the command f from serial (follower)   
if (data[0] == 'f') {    
    if(ledmode[0]==1){    //and if ledmode[0] is set to 1 (standard led mode)
       for (int a= 0; a< Nofblinks[1]; a++){
         digitalWrite(GreenPIN, HIGH);
         delay(blinkdelay[1]);
         digitalWrite(GreenPIN, 0);
         delay(blinkdelay[1]);
          }
}
    if(ledmode[0]==2){ //and if ledmode[0] is set to 2 (1x RGB led mode)
       for (int a= 0; a< Nofblinks[1]; a++){
         analogWrite(RedPIN, BRIGHTF[0]);
         analogWrite(GreenPIN, BRIGHTF[1]);
         analogWrite(BluePIN, BRIGHTF[2]);
         delay(blinkdelay[1]);
         analogWrite(RedPIN, 0);
         analogWrite(GreenPIN, 0);
         analogWrite(BluePIN, 0);
         delay(blinkdelay[1]);
         }
     }
 Serial.println("you got a new follower ");
 Serial.println(" "); 
}     
  

if (data[0] == 'p') { //if i receive the command p from serial (new post) and if ledmode[0] is set to 1 (standard led mode)   
    if(ledmode[0]==1){    //and if ledmode[0] is set to 1 (standard led mode)
       for (int a= 0; a< Nofblinks[2]; a++){
         digitalWrite(BluePIN, HIGH);
         delay(blinkdelay[2]);
         digitalWrite(BluePIN, 0);
         delay(blinkdelay[2]);
          }
}
    if(ledmode[0]==2){ //and if ledmode[0] is set to 2 (1x RGB led mode)
       for (int a= 0; a< Nofblinks[2]; a++){
         analogWrite(RedPIN, BRIGHTP[0]);
         analogWrite(GreenPIN, BRIGHTP[1]);
         analogWrite(BluePIN, BRIGHTP[2]);
         delay(blinkdelay[2]);
         analogWrite(RedPIN, 0);
         analogWrite(GreenPIN, 0);
         analogWrite(BluePIN, 0);
         delay(blinkdelay[2]);
         }
     }
 Serial.println("ther is a new post ");
 Serial.println(" "); 
}     
}

