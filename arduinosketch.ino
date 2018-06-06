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
int BRIGHTU[3]; // This array will store the information regarding the colour of the RGB led for an incoming upvote trigger (3 values for the colour)
int BRIGHTF[3];// This array will store the information regarding the colour of the RGB led for an incoming follower trigger (3 values for the colour)
int BRIGHTP[3]; // This array will store the information regarding the colour of the RGB led for an incoming post trigger (3 values for the colour)
int EFFECT[3]; // This array will store the information regarding the effect chosen for each led/notification (1 for normal Blink, 2 for Fading, 3 for rainbow, 4 for ice, 5 for fire)
unsigned long StartTime; //will be used to calculate timing for fading effect
unsigned long CurrentTime; //will be used to calculate timing for fading effect

void setup(){
    Serial.begin(9600);//initialize serial comunication
    Serial.flush();    
      pinMode(RedPIN, OUTPUT); 
      pinMode(GreenPIN, OUTPUT);//set the selected pins as output 
      pinMode(BluePIN, OUTPUT); 
}
 
void loop(){ //collecting the incoming data and filling the buffer with it
  StartTime = millis(); // updating time
  if (Serial.available() > 0) { //if there is any incoming serial data of any sort (any setting or any notification)
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
   for (int r=1; r<=3; r++){          // if within 3 seconds  
      if (Serial.available() == NULL){  // no serial data is detected
        delay (1000);                    //(thus no settings or no notification) -> activate the function breath
       }
      else{ // if serial data is detected then stop the process
        break;
        }}
        
     breath(); // activate the function called breath (standby fading light pattern)
     }
}

void breath(){
   int redIntensity = 0;   //variable to handle the brightness of the RED colour of the RGB led
   int blueIntensity = 0;  //variable to handle the brightness of the BLUE colour of the RGB led
   int greenIntensity = 0; //variable to handle the brightness of the GREEN colour of the RGB led

while (Serial.available() == NULL){ //run below fading pattern till some data is detected over serial
 
 for (int i = 0; i <=240; i+=1){ //This loop will light up/dim linearly the led till it goes from off state to a value close to
                                 //R=110 G=0 B=255 the starting value for our fading pattern

    if (Serial.available() > 0){ // 
      analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      break;
      }
      
   redIntensity = i/2;
   blueIntensity = i ;
 
      analogWrite(RedPIN, redIntensity);     //
      analogWrite(GreenPIN, greenIntensity);  //activate with proper delay the led using the currently stored values for R G B
      analogWrite(BluePIN, blueIntensity);   //
      delay(10);    
      }

 for (int t = 0; t<1; t++){  //this FOR loop will let use repeat the below (whole) fade-in, fade-out pattern twice
  
   if (Serial.available() > 0){ // 
      analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      break;
      }
      
 for (int i = 110; i <=255; i+=1){    //In this for loop we will handle the first transition from R=110 G=0 B=255 (darker Violet) to 
                                      //R=255 G=10 B=255 (much brighter Violet) and have chosen to use the redIntensity as the variable part of the FOR loop
                                      //based on the value of R = i we will calculate the Green brightness.
   if (Serial.available() > 0){ // 
      analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      break;
      }
      
 redIntensity = i;
 greenIntensity = ((118937/10000)*log(i)-(559050/10000)); //The neperien logarithmic function usually wrote as ln(x) 
                                                          //is not recognized in Arduino coding by using ln but log
                                                          
if(greenIntensity> 10){ //based on the aproximation in calculations we do this just in case not to pass the value
  greenIntensity =10;
}
if(greenIntensity< 1){ //based on the aproximation in the calculations this serves to not let
  greenIntensity =0;   //the Green pin turns negative 
}

analogWrite(RedPIN, redIntensity);     //
analogWrite(GreenPIN, greenIntensity); //activate with proper delay the led using the currently stored values for R G B
analogWrite(BluePIN, blueIntensity);   //
delay(10);                             //
}

for (int i = 10; i <=220; i+=1){ //using this For loop we handle the linear transition between the bright Violet (R=255 G=10 B=255) 
                                 //and the White colour (R=255 G=255 b=255)
                                 
   if (Serial.available() > 0){ // 
      analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      break;
      }
      
   redIntensity=255;
   blueIntensity=255;
   greenIntensity=i;

analogWrite(RedPIN, redIntensity);     //
analogWrite(GreenPIN, greenIntensity); //activate with proper delay the led using the currently stored values for R G B
analogWrite(BluePIN, blueIntensity);   //
delay(10);                             //
}

//below we reverse the loops used before to fade from Violet to White and use them to do the other way (from White to Violet)
for (int i = 220; i >=10; i-=1){ //using this For loop we handle the linear transition between the White colour (R=255 G=255 b=255)  
                                 //and the bright Violet (R=255 G=10 B=255)
                                 
  if (Serial.available() > 0){ // 
      analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      break;
      }
      
   redIntensity=255;
   blueIntensity=255;
   greenIntensity=i;

analogWrite(RedPIN, redIntensity);     //
analogWrite(GreenPIN, greenIntensity); //activate with proper delay the led using the currently stored values for R G B
analogWrite(BluePIN, blueIntensity);   //
delay(10);                             //
}

for (int i = 255; i >=110; i-=1){ //In this for loop we will handle the transition from R=255 G=10 B=255 (much brighter Violet) to 
                                  // R=110 G=0 B=255 (darker Violet) and as before have chosen to use the redIntensity as the variable part of the FOR loop
                                  //based on the value of R = i we will calculate the Green brightness.
   if (Serial.available() > 0){ // 
      analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      break;
      }
      
redIntensity = i;
greenIntensity = ((118937/10000)*log(i)-(559050/10000));
 
if(greenIntensity<1){ //based ont he aproximations this will keep us on the safe side.
  greenIntensity =0;
  }
analogWrite(RedPIN, redIntensity);     //
analogWrite(GreenPIN, greenIntensity); //activate with proper delay the led using the currently stored values for R G B
analogWrite(BluePIN, blueIntensity);   //
delay(10);                             //
}}

for (int i = 255; i >=0; i-=1){ //This loop will dim linearly the led till it goes totally of
                                //since we went back to R=110 G=0 B=255 with a quick calculation and a good aproximation 
                                //we set the step to take for each brightness decrease so that they get dimmed simultaneously and of the same ammount each time

   if (Serial.available() > 0){ // 
      analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
      analogWrite(GreenPIN, 0); //
      analogWrite(BluePIN, 0); //
      break;
      }
      
redIntensity = i/3;
blueIntensity = i ;
 
analogWrite(RedPIN, redIntensity);     //
analogWrite(GreenPIN, greenIntensity); //activate with proper delay the led using the currently stored values for R G B
analogWrite(BluePIN, blueIntensity);   //
delay(10);    
}
}
} 

void splitString(char* data) { //splitting the data received based on a fixed separator (in this case the blank space " ")
       Serial.print("Data entered: "); 
       Serial.println(data); // display data entered/received
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
      Serial.println("Current Settings: ");
      Serial.println("---------------------------------------------------- ");
      Serial.println(" ");  
if(ledmode[0]== 1){     
        Serial.println("ledmode is set to: 3x single colour LED ");
        Serial.println(" ");  
        
    for (int i=0;i<=2;i++){
        Serial.print("effect for led ");
        Serial.print(i+1);
        Serial.print(" is set to: ");
      if(EFFECT[i]==1){
        Serial.println("NORMAL BLINK");
        }
      if(EFFECT[i]==2){
        Serial.println("FADING");
        }
      }
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
             
              if (EFFECT[0]==1){
                Serial.println("Effect for upvote is: NORMAL BLINKING");
                Serial.println(" ");
                 }
              if (EFFECT[0]==2){
                Serial.println("Effect for upvote is: FADING");
                Serial.println(" ");
                 }
              if (EFFECT[0]==3){
                Serial.println("Effect for upvote is: RAINBOW");
                Serial.println(" ");
              }
              if (EFFECT[0]==4){
                Serial.println("Effect for upvote is: ICE");
                Serial.println(" ");
              }
              if (EFFECT[0]==4){
                Serial.println("Effect for upvote is: FIRE");
                Serial.println(" ");
              }
                          
              Serial.println("RGB value for follower trigger is set to");
              Serial.println(" ");
              Serial.print("R= ");
              Serial.println(BRIGHTF[0]);
              Serial.print("G= ");
              Serial.println(BRIGHTF[1]);
              Serial.print("B= ");
              Serial.println(BRIGHTF[2]);
              Serial.println("");
              if (EFFECT[1]==1){
                Serial.println("Effect for follower is: NORMAL BLINKING");
                Serial.println(" ");
                 }
              if (EFFECT[1]==2){
                Serial.println("Effect for follower is: FADING");
                Serial.println(" ");
                 }
              if (EFFECT[1]==3){
                Serial.println("Effect for follower is: RAINBOW");
                Serial.println(" ");
              }
              if (EFFECT[1]==4){
                Serial.println("Effect for follower is: ICE");
                Serial.println(" ");
              }
              if (EFFECT[1]==5){
                Serial.println("Effect for follower is: FIRE");
                Serial.println(" ");
              }
                            
              Serial.println("RGB value for post trigger is set to");
              Serial.println(" ");
              Serial.print("R= ");
              Serial.println(BRIGHTP[0]);
              Serial.print("G= ");
              Serial.println(BRIGHTP[1]);
              Serial.print("B= ");
              Serial.println(BRIGHTP[2]);
              Serial.println("");
              if (EFFECT[2]==1){
                Serial.println("Effect for post is: NORMAL BLINKING");
                Serial.println(" ");
                }
              if (EFFECT[2]==2){
                Serial.println("Effect for post is: FADING");
                Serial.println(" ");
               }
              if (EFFECT[2]==3){
                Serial.println("Effect for post is: RAINBOW");
                Serial.println(" ");
              }  
              if (EFFECT[2]==4){
                Serial.println("Effect for post is: ICE");
                Serial.println(" ");
              }
              if (EFFECT[2]==5){
                Serial.println("Effect for post is: FIRE");
                Serial.println(" ");
              }
         }
    }
}

void setLEDon(char* data) {
  //if Arduino receives the command u from serial (upvote detected)
if (data[0] == 'u') {             
   if(ledmode[0]==1){ //if ledmode is set to 3 single colour led mode
       if (EFFECT[0]== 1){ // if the effect selected is normal blinking
           for (int a= 0; a< Nofblinks[0]; a++){ //repeat the blinking effect as per the Nofblinks[0] value
                if (Serial.available() > 0){ // 
                    analogWrite(RedPIN, 0);    //turn off the led whenever data is received and exit the loop
                    analogWrite(GreenPIN, 0); //
                    analogWrite(BluePIN, 0); //
                    break;
                    }
              //do the blinking  
                 analogWrite(RedPIN, 255); //turn on the led 
                 delay(blinkdelay[0]);
                 analogWrite(RedPIN, 0);//turn off the led
                 delay(blinkdelay[0]);
             }
       }
    if (EFFECT[0]== 2){ // if the effect selected is fading effect
         for (int a= 0; a< Nofblinks[0]; a++){ //repeat the fading effect as per the Nofblinks[0] value
              for (int f=0; f<=255; f++){
             
                 if (Serial.available() > 0){ // 
                    analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
                    analogWrite(GreenPIN, 0); //
                    analogWrite(BluePIN, 0); //
                    break;
                   }   
                   
           /////////////////////////////////////////
           // led will fade in from 0% brightness //
           //       to 100% brightness            //
           /////////////////////////////////////////

                   analogWrite(RedPIN, f); // activate the led
                   delay((blinkdelay[0]/20));
              }
           for (int f=255; f>=0; f--){
            
                if (Serial.available() > 0){ // 
                    analogWrite(RedPIN, 0);    //turn off the led whenever data is received and exit the loop
                    analogWrite(GreenPIN, 0); //
                    analogWrite(BluePIN, 0); //
                    break;
                    }   
                    
            ///////////////////////////////////////////
            // led will fade in from 100% brightness //
            //      0% brightness back and forth     //
            ///////////////////////////////////////////
                                   
               analogWrite(RedPIN, f); // activate the led
               delay((blinkdelay[0]/20));
            }
        }
    }
 }
 
 if(ledmode[0]==2){
    if (EFFECT[0]== 1){ // if the effect selected is normal blinking
        for (int a= 0; a< Nofblinks[0]; a++){ //repeat the blinking effect as per the Nofblinks[0] value
             if (Serial.available() > 0){ // 
                 analogWrite(RedPIN, 0);    //turn off the led whenever data is received and exit the loop
                 analogWrite(GreenPIN, 0); //
                 analogWrite(BluePIN, 0); //
                 break;
                 }
           //do the blinking  
           analogWrite(RedPIN, BRIGHTU[0]);
           analogWrite(GreenPIN, BRIGHTU[1]); //turn on the led using the stored R G B values
           analogWrite(BluePIN, BRIGHTU[2]);
           delay(blinkdelay[0]);
           analogWrite(RedPIN, 0);
           analogWrite(GreenPIN, 0);//turn of the led
           analogWrite(BluePIN, 0);
           delay(blinkdelay[0]);
          }
       }
    if (EFFECT[0]== 2){ // if the effect selected is fading effect
       unsigned int PERIOD = (blinkdelay[0]*10);
       double OMEGA = 2*PI/PERIOD; //use a double variable since OMEGA can contain decimals
       int count=1;// variable to count the number of oscillations
       StartTime= millis();
     
      while (count <= Nofblinks[0]){ //loop to be used to repeat the pattern as per Nofblinks[0] value
               if (Serial.available() > 0){ /////////////////////////////////////// 
                 analogWrite(RedPIN, 0);    //turn off the led when any new data //
                 analogWrite(GreenPIN, 0);  //  is received and exit the loop    //
                 analogWrite(BluePIN, 0);   ///////////////////////////////////////
                 break;
                 }   
                 
                CurrentTime= millis();
               /////////////////////////////////////////////////////
               //  led, set to an R G B value will fade using a   //
               // sine wave from 0% brightness to 100% brightness //
               /////////////////////////////////////////////////////

               int FadeRed= ((BRIGHTU[0]/2) + (BRIGHTU[0]/2)*(cos(OMEGA*CurrentTime)));    ///////////////////////////////////////////////
               int FadeGreen= ((BRIGHTU[1]/2) + (BRIGHTU[1]/2)*(cos(OMEGA*CurrentTime)));  // let the value of the brightness oscillate //
               int FadeBlue= ((BRIGHTU[2]/2) + (BRIGHTU[2]/2)*(cos(OMEGA*CurrentTime)));   // between 0% and 100% of the set RGB colour //
                                                                                           //     with the variation of a sine wave     //
                                                                                           ///////////////////////////////////////////////

               if ((unsigned long)(millis() - StartTime) >= PERIOD){ ///////////////////////////////////////////////////////////////////////////
                count++;                                             // every full oscillation increase the counter and take the current time //
                StartTime= millis();                                 //  as the new starting point to calculate next period of oscilation     //
               }                                                     ///////////////////////////////////////////////////////////////////////////
               
                analogWrite(RedPIN, FadeRed);
                analogWrite(GreenPIN, FadeGreen);// activate the led
                analogWrite(BluePIN, FadeBlue);
                

               if (count == Nofblinks[0] ){
                analogWrite(RedPIN, 0);
                analogWrite(GreenPIN, 0);// turn off the led
                analogWrite(BluePIN, 0);
                break;
                }
          }              
    }

                  
if (EFFECT[0]== 3){ // if the chosen effect for this led is rainbow effect
         int redrainbow;
         int greenrainbow;
         int bluerainbow;
   for (int a= 0; a< Nofblinks[0]; a++){  //repeat the rainbow effect as per the Nofblinks[0] value
      for (int y = 0; y < 768; y++){
        
        if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led whenever data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
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
            delay((blinkdelay[0]/30));                    
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
      }
      
if (EFFECT[0]==4){ // if the chosen effect for this led is ice effect
   for (int a= 0; a< Nofblinks[0]; a++){//repeat the ice effect as per the Nofblinks[0] value
       if (Serial.available() > 0){ // 
          analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
          analogWrite(GreenPIN, 0); //
          analogWrite(BluePIN, 0); //
          break;
          }
          
    for (int y=0; y<=240; y++){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
        // fade in till full blue-ice brightness
        analogWrite(GreenPIN, y); //
        analogWrite(BluePIN, y); //
        delay((blinkdelay[0]/20));  
        }
    for (int x=0; x<=255; x++){
        if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, x); //mix red proportionally to reach white colour
         analogWrite(GreenPIN, 255); //
         analogWrite(BluePIN, 255); //
         delay ((blinkdelay[0]/20));
         }
    for (int z=255; z>=0; z--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
      
         analogWrite(RedPIN, z); // reverse the action to fade out
         analogWrite(GreenPIN, 255); //
         analogWrite(BluePIN, 255); //
         delay ((blinkdelay[0]/20));
         }
    for (int w=255; w>=0; w--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, 0); // fade out blue ice till all off
         analogWrite(GreenPIN, w); //
         analogWrite(BluePIN, w); //
         delay((blinkdelay[0]/20));  
         }
     }
     analogWrite(RedPIN, 0); // turn off the led
     analogWrite(GreenPIN, 0); // just to make sure it is off
     analogWrite(BluePIN, 0); //
  }

      
if (EFFECT[0]==5){ // if the chosen effect for this led is fire effect
  for (int a= 0; a< Nofblinks[0]; a++){ //repeat the rainbow effect as per the Nofblinks[1] value
     for (int y=0; y<=255; y++){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
        analogWrite(RedPIN, y);// fade in till full red brightness
        delay((blinkdelay[0]/20));  
        }
    for (int x=0; x<=40; x++){
        if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, 255); //mix green proportionally to the red in order to 
         analogWrite(GreenPIN, x); // blend the colours
         delay ((blinkdelay[0]/20));
         }
         
    for (int z=40; z>=0; z--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
      
         analogWrite(RedPIN, 255); // reverse the action to fade out
         analogWrite(GreenPIN, z); //
         delay ((blinkdelay[0]/20));
         }
    for (int w=255; w>=0; w--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, w); // fade out red till off
         delay((blinkdelay[0]/20));  
         }
     }
     analogWrite(RedPIN, 0); // turn off the led
     analogWrite(GreenPIN, 0); //
  }  
  }
 Serial.println("you got an upvote ");
 Serial.println(" "); 
}
      
//if Arduino receives the command f from serial (follower)   
if (data[0] == 'f') { //if Arduino detects the trigger command f from serial (follower)    
    if(ledmode[0]==1){    // if ledmode is set to 3 single colour led mode
        if (EFFECT[1]== 1){ // if the effect selected is normal blinking
           for (int a= 0; a< Nofblinks[1]; a++){ //repeat the blinking effect as per the Nofblinks[1] value
                if (Serial.available() > 0){ // 
                    analogWrite(RedPIN, 0);    //turn off the led whenever data is received and exit the loop
                    analogWrite(GreenPIN, 0); //
                    analogWrite(BluePIN, 0); //
                    break;
                    }
              //do the blinking  
                 analogWrite(GreenPIN, 255); //turn on the led 
                 delay(blinkdelay[1]);
                 analogWrite(GreenPIN, 0);//turn off the led
                 delay(blinkdelay[1]);
             }
       }
    if (EFFECT[1]== 2){ // if the effect selected is fading effect
         for (int a= 0; a< Nofblinks[1]; a++){ //repeat the fading effect as per the Nofblinks[0] value
              for (int f=0; f<=255; f++){
             
                 if (Serial.available() > 0){ // 
                    analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
                    analogWrite(GreenPIN, 0); //
                    analogWrite(BluePIN, 0); //
                    break;
                   }   
                   
           /////////////////////////////////////////
           // led will fade in from 0% brightness //
           //       to 100% brightness            //
           /////////////////////////////////////////

                   analogWrite(GreenPIN, f); // activate the led
                   delay(blinkdelay[1]);
              }
           for (int f=255; f>=0; f--){
            
                if (Serial.available() > 0){ // 
                    analogWrite(RedPIN, 0);    //turn off the led whenever data is received and exit the loop
                    analogWrite(GreenPIN, 0); //
                    analogWrite(BluePIN, 0); //
                    break;
                    }   
                    
            ///////////////////////////////////////////
            // led will fade in from 100% brightness //
            //      0% brightness back and forth     //
            ///////////////////////////////////////////
                                   
               analogWrite(GreenPIN, f); // activate the led
               delay(blinkdelay[1]);
            }
        }
    }
 }
      
    if(ledmode[0]==2){ //and if ledmode is set to 1 RGB led
       if (EFFECT[1]== 1){ // if the effect selected is normal blinking
        for (int a= 0; a< Nofblinks[1]; a++){ //repeat the blinking effect as per the Nofblinks[1] value
             if (Serial.available() > 0){ // 
                 analogWrite(RedPIN, 0);    //turn off the led if data is received and exit the loop immediately
                 analogWrite(GreenPIN, 0); //
                 analogWrite(BluePIN, 0); //
                 break;
                 }
           //activate the led and let it blink        
           analogWrite(RedPIN, BRIGHTF[0]);
           analogWrite(GreenPIN, BRIGHTF[1]); //light up the led using the previously stored settings
           analogWrite(BluePIN, BRIGHTF[2]);
           delay(blinkdelay[1]);
           analogWrite(RedPIN, 0);
           analogWrite(GreenPIN, 0); //turn the led off
           analogWrite(BluePIN, 0);
           delay(blinkdelay[1]);
          }
       }
   if (EFFECT[1]== 2){ // if the effect selected is fading effect
       unsigned int PERIOD = (blinkdelay[1]*10);
       double OMEGA = 2*PI/PERIOD; //use a double variable since OMEGA can contain decimals
       int count=1;// variable to count the number of oscillations
       StartTime= millis();
     
      while (count <= Nofblinks[1]){ //loop to be used to repeat the pattern as per Nofblinks[1] value
               if (Serial.available() > 0){ /////////////////////////////////////// 
                 analogWrite(RedPIN, 0);    //turn off the led when any new data //
                 analogWrite(GreenPIN, 0);  //  is received and exit the loop    //
                 analogWrite(BluePIN, 0);   ///////////////////////////////////////
                 break;
                 }   
                 
                CurrentTime= millis();
               /////////////////////////////////////////////////////
               //  led, set to an R G B value will fade using a   //
               // sine wave from 0% brightness to 100% brightness //
               /////////////////////////////////////////////////////

               int FadeRed= ((BRIGHTF[0]/2) + (BRIGHTF[0]/2)*(cos(OMEGA*CurrentTime)));    ///////////////////////////////////////////////
               int FadeGreen= ((BRIGHTF[1]/2) + (BRIGHTF[1]/2)*(cos(OMEGA*CurrentTime)));  // let the value of the brightness oscillate //
               int FadeBlue= ((BRIGHTF[2]/2) + (BRIGHTF[2]/2)*(cos(OMEGA*CurrentTime)));   // between 0% and 100% of the set RGB colour //
                                                                                           //     with the variation of a sine wave     //
                                                                                           ///////////////////////////////////////////////

               if ((unsigned long)(millis() - StartTime) >= PERIOD){ ///////////////////////////////////////////////////////////////////////////
                count++;                                             // every full oscillation increase the counter and take the current time //
                StartTime= millis();                                 //  as the new starting point to calculate next period of oscilation     //
               }                                                     ///////////////////////////////////////////////////////////////////////////
               
                analogWrite(RedPIN, FadeRed);
                analogWrite(GreenPIN, FadeGreen);// activate the led
                analogWrite(BluePIN, FadeBlue);
                

               if (count == Nofblinks[1] ){
                analogWrite(RedPIN, 0);
                analogWrite(GreenPIN, 0);// turn off the led
                analogWrite(BluePIN, 0);
                break;
                }
          }              
    }

      
if (EFFECT[1]== 3){ // if the chosen effect for this led is rainbow effect
         int redrainbow;
         int greenrainbow;
         int bluerainbow;
   for (int a= 0; a< Nofblinks[1]; a++){  //repeat the rainbow effect as per the Nofblinks[1] value
      for (int y = 0; y < 768; y++){
        
        if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led whenever data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
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
            delay((blinkdelay[1]/30));                    
            }
         }
         // final off after the whole rainbow is done
          delay(500);
          redrainbow = 0;    // red off
          greenrainbow = 0;        // green off
          bluerainbow = 0;             // blue off
          analogWrite(RedPIN, redrainbow);
          analogWrite(BluePIN, bluerainbow);
          analogWrite(GreenPIN, greenrainbow);
        }
        
if (EFFECT[1]==4){ // if the chosen effect for this led is ice effect
   for (int a= 0; a< Nofblinks[1]; a++){//repeat the ice effect as per the Nofblinks[1] value
       if (Serial.available() > 0){ // 
          analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
          analogWrite(GreenPIN, 0); //
          analogWrite(BluePIN, 0); //
          break;
          }
          
    for (int y=0; y<=240; y++){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
        // fade in till full blue-ice brightness
        analogWrite(GreenPIN, y); //
        analogWrite(BluePIN, y); //
        delay((blinkdelay[1]/20));  
        }
    for (int x=0; x<=255; x++){
        if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, x); //mix red proportionally to reach white colour
         analogWrite(GreenPIN, 255); //
         analogWrite(BluePIN, 255); //
         delay ((blinkdelay[1]/20));
         }
    for (int z=255; z>=0; z--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
      
         analogWrite(RedPIN, z); // reverse the action to fade out
         analogWrite(GreenPIN, 255); //
         analogWrite(BluePIN, 255); //
         delay ((blinkdelay[1]/20));
         }
    for (int w=255; w>=0; w--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, 0); // fade out blue ice till all off
         analogWrite(GreenPIN, w); //
         analogWrite(BluePIN, w); //
         delay((blinkdelay[1]/20));  
         }
     }
     analogWrite(RedPIN, 0); // turn off the led
     analogWrite(GreenPIN, 0); // just to make sure it is off
     analogWrite(BluePIN, 0); //
  }
        
if (EFFECT[1]==5){ // if the chosen effect for this led is fire effect
  for (int a= 0; a< Nofblinks[1]; a++){ //repeat the rainbow effect as per the Nofblinks[1] value
     for (int y=0; y<=255; y++){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
        analogWrite(RedPIN, y);// fade in till full red brightness
        delay((blinkdelay[1]/20));  
        }
    for (int x=0; x<=40; x++){
        if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, 255); //mix green proportionally to the red in order to 
         analogWrite(GreenPIN, x); // blend the colours
         delay ((blinkdelay[1]/20));
         }
    for (int z=40; z>=0; z--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
      
         analogWrite(RedPIN, 255); // reverse the action to fade out
         analogWrite(GreenPIN, z); //
         delay ((blinkdelay[1]/20));
         }
    for (int w=255; w>=0; w--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, w); // fade out red till off
         delay((blinkdelay[1]/20));  
         }
     }
     analogWrite(RedPIN, 0); // turn off the led
     analogWrite(GreenPIN, 0); //
  }
      
  }
 Serial.println("you got a new follower ");
 Serial.println(" "); 
}
     
  

if (data[0] == 'p') { //if Arduino detects the trigger command p from serial (new post) 
    if(ledmode[0]==1){    //and if ledmode[0] is set to 1 (3x single colour led mode)
      if (EFFECT[2]== 1){ // if the effect selected is normal blinking
         for (int a= 0; a< Nofblinks[2]; a++){ //repeat the blinking effect as per the Nofblinks[2] value
            if (Serial.available() > 0){ // 
                 analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
                 analogWrite(GreenPIN, 0); //
                 analogWrite(BluePIN, 0); //
                 break;
                 }  
              // do the blinking
            analogWrite(BluePIN, 255); //turn on the led to full birghtness
            delay(blinkdelay[2]);
            analogWrite(BluePIN, 0);//turn off the led
            delay(blinkdelay[2]);
            }
         }
         
     if (EFFECT[2]== 2){ // if the effect selected is fading effect
        for (int a= 0; a< Nofblinks[2]; a++){ //repeat the fading effect as per the Nofblinks[2] value
           for (int f=0; f<=255; f++){
              if (Serial.available() > 0){ // 
                 analogWrite(RedPIN, 0);    //turn off the led whenever data is received and exit the loop
                 analogWrite(GreenPIN, 0); //
                 analogWrite(BluePIN, 0); //
                 break;
                 }        
                analogWrite(BluePIN, f); //fade in the led
                delay((blinkdelay[2]/20));
                }
           for (int f=255; f>=0; f--){
                analogWrite(BluePIN, f);//fade out the led
                delay((blinkdelay[2]/20));
                } 
          }
       }
    }
if(ledmode[0]==2){ //if ledmode[0] is set to 2 (1x RGB led mode)
    if (EFFECT[2]== 1){ // if the effect selected is normal blinking
        for (int a= 0; a< Nofblinks[2]; a++){ //repeat the blinking effect as per the Nofblinks[2] value
             if (Serial.available() > 0){ // 
                 analogWrite(RedPIN, 0);    //turn off the led whenever data is received and exit the loop
                 analogWrite(GreenPIN, 0); //
                 analogWrite(BluePIN, 0); //
                 break;
                 }
           //do the blinking  
           analogWrite(RedPIN, BRIGHTP[0]);
           analogWrite(GreenPIN, BRIGHTP[1]); //turn on the led using the stored R G B values
           analogWrite(BluePIN, BRIGHTP[2]);
           delay(blinkdelay[2]);
           analogWrite(RedPIN, 0);
           analogWrite(GreenPIN, 0);//turn of the led
           analogWrite(BluePIN, 0);
           delay(blinkdelay[2]);
          }
       }
if (EFFECT[2]== 2){ // if the effect selected is fading effect
       unsigned int PERIOD = (blinkdelay[2]*10);
       double OMEGA = 2*PI/PERIOD; //use a double variable since OMEGA can contain decimals
       int count=1;// variable to count the number of oscillations
       StartTime= millis();
     
      while (count <= Nofblinks[2]){ //loop to be used to repeat the pattern as per Nofblinks[2] value
               if (Serial.available() > 0){ /////////////////////////////////////// 
                 analogWrite(RedPIN, 0);    //turn off the led when any new data //
                 analogWrite(GreenPIN, 0);  //  is received and exit the loop    //
                 analogWrite(BluePIN, 0);   ///////////////////////////////////////
                 break;
                 }   
                 
                CurrentTime= millis();
               /////////////////////////////////////////////////////
               //  led, set to an R G B value will fade using a   //
               // sine wave from 0% brightness to 100% brightness //
               /////////////////////////////////////////////////////

               int FadeRed= ((BRIGHTP[0]/2) + (BRIGHTP[0]/2)*(cos(OMEGA*CurrentTime)));    ///////////////////////////////////////////////
               int FadeGreen= ((BRIGHTP[1]/2) + (BRIGHTP[1]/2)*(cos(OMEGA*CurrentTime)));  // let the value of the brightness oscillate //
               int FadeBlue= ((BRIGHTP[2]/2) + (BRIGHTP[2]/2)*(cos(OMEGA*CurrentTime)));   // between 0% and 100% of the set RGB colour //
                                                                                           //     with the variation of a sine wave     //
                                                                                           ///////////////////////////////////////////////

               if ((unsigned long)(millis() - StartTime) >= PERIOD){ ///////////////////////////////////////////////////////////////////////////
                count++;                                             // every full oscillation increase the counter and take the current time //
                StartTime= millis();                                 //  as the new starting point to calculate next period of oscilation     //
               }                                                     ///////////////////////////////////////////////////////////////////////////
               
                analogWrite(RedPIN, FadeRed);
                analogWrite(GreenPIN, FadeGreen);// activate the led
                analogWrite(BluePIN, FadeBlue);
                

               if (count == Nofblinks[2] ){
                analogWrite(RedPIN, 0);
                analogWrite(GreenPIN, 0);// turn off the led
                analogWrite(BluePIN, 0);
                break;
                }
          }              
    }
      
if (EFFECT[2]== 3){ // if the chosen effect for this led is rainbow effect
         int redrainbow;
         int greenrainbow;
         int bluerainbow;
   for (int a= 0; a< Nofblinks[2]; a++){  //repeat the rainbow effect as per the Nofblinks[2] value
      for (int y = 0; y < 768; y++){
        
        if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
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
            delay((blinkdelay[2]/30));                    
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
      }
      
if (EFFECT[2]==4){ // if the chosen effect for this led is ice effect
   for (int a= 0; a< Nofblinks[2]; a++){//repeat the ice effect as per the Nofblinks[2] value
       if (Serial.available() > 0){ // 
          analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
          analogWrite(GreenPIN, 0); //
          analogWrite(BluePIN, 0); //
          break;
          }
          
    for (int y=0; y<=240; y++){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
        // fade in till full blue-ice brightness
        analogWrite(GreenPIN, y); //
        analogWrite(BluePIN, y); //
        delay((blinkdelay[2]/20));  
        }
    for (int x=0; x<=255; x++){
        if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, x); //mix red proportionally to reach white colour
         analogWrite(GreenPIN, 255); //
         analogWrite(BluePIN, 255); //
         delay ((blinkdelay[2]/20));
         }
    for (int z=255; z>=0; z--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
      
         analogWrite(RedPIN, z); // reverse the action to fade out
         analogWrite(GreenPIN, 255); //
         analogWrite(BluePIN, 255); //
         delay ((blinkdelay[2]/20));
         }
    for (int w=255; w>=0; w--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, 0); // fade out blue ice till all off
         analogWrite(GreenPIN, w); //
         analogWrite(BluePIN, w); //
         delay((blinkdelay[2]/20));  
         }
     }
     analogWrite(RedPIN, 0); // turn off the led
     analogWrite(GreenPIN, 0); // just to make sure it is off
     analogWrite(BluePIN, 0); //
  }
      
if (EFFECT[2]==5){ // if the chosen effect for this led is fire effect
  for (int a= 0; a< Nofblinks[2]; a++){ //repeat the rainbow effect as per the Nofblinks[2] value
     if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }//repeat the fire effect as per the Nofblinks[2] value
    for (int y=0; y<=255; y++){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
        analogWrite(RedPIN, y);// fade in till full red brightness
        delay((blinkdelay[2]/20));  
        }
    for (int x=0; x<=40; x++){
        if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, 255); //mix green proportionally to the red in order to 
         analogWrite(GreenPIN, x); // blend the colours
         delay ((blinkdelay[2]/20));
         }
    for (int z=40; z>=0; z--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
      
         analogWrite(RedPIN, 255); // reverse the action to fade out
         analogWrite(GreenPIN, z); //
         delay ((blinkdelay[2]/20));
         }
    for (int w=255; w>=0; w--){
       if (Serial.available() > 0){ // 
           analogWrite(RedPIN, 0);    //turn off the led when data is received and exit the loop
           analogWrite(GreenPIN, 0); //
           analogWrite(BluePIN, 0); //
            break;
            }
         analogWrite(RedPIN, w); // fade out red till off
         delay((blinkdelay[2]/20));  
         }
      
     }
     analogWrite(RedPIN, 0); // turn off the led
     analogWrite(GreenPIN, 0); //
  }
     
      Serial.println("there is a new post ");
      Serial.println(" "); 
     }     
   }
}


