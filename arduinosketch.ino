char buffer[30];

int LED_ONE= 2; //pin for led 1
int LED_TWO= 3; //pin for led 2
int LED_THREE= 4; //pin for led 3
int y=0;
int ledmode[2]; //this array will store the information regarding the ledmode (for now '1' for standard led mode but RGB mode will be coming soon and other modes also in the roadmap)
int blinkdelay[3]; //this array will store the information to be used for the blink speed for each led
int transferdata[3]; //array to be used to store the incoming settings and transfer them
int Nofblinks[3]; //this array will store the information to be used for the number of triggers to do (number of repeated triggers)

void setup()
{
Serial.begin(14400);
Serial.flush();
    pinMode(LED_ONE, OUTPUT); //set the selected pins as output 
    pinMode(LED_TWO, OUTPUT);
    pinMode(LED_THREE, OUTPUT);
}
 
void loop() //collecting the incoming data and filling the buffer with it
{
if (Serial.available() > 0) {
int index=0;
delay(100); // let the buffer fill up
int numChar = Serial.available();
if (numChar>30) {
numChar=30;
}
while (numChar--) {
buffer[index++] = Serial.read();
}
splitdata(buffer); 
} 
}

void splitdata(char* data) { //splitting the data received based on a fixed separator (in this case the blank space " ")
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

//here we will store the splitted data received over the serial comunication to save our settings
void setallsettings(char* data) { 
  
if (data[0] == 'm') {  // select and store in the array lemode the mode (for now only Single Led Mode - 1)
    int blinkdata = strtol(data+1, NULL, 10);
    blinkdata = constrain(blinkdata,1,2); 
    transferdata[y] = blinkdata;
    y=y+1;
    if (y==1){ // when transferdata is full of data (we expect 2 values one for the mode and one for the number of leds between 1 and 3, for now what matters is the mode that should be 1)
    memcpy(ledmode, transferdata, sizeof(transferdata)); //copying transferdata to ledmode then clearing transferdata and resetting y, 
    for (int i=0; i<=sizeof(transferdata); i++){
    transferdata[i]= '\0';
    }
    y=0;
    }
  }
if (data[0] =='d'){ //store in an array called blinkdelay the sequence of speed on which later leds will trigger
    int blinkdata = strtol(data+1, NULL, 10);
    blinkdata = constrain(blinkdata,1,1000); 
    transferdata[y] = blinkdata;
    y=y+1;
    if (y==3){                                                // when transferdata is full of data (we expect 3 speeds value between 1 and 1000)
    memcpy(blinkdelay, transferdata, sizeof(transferdata));   //copying transferdata to blinkdelay then clearing transferdata and resetting y, 
    for (int i=0; i<=sizeof(transferdata); i++){
    transferdata[i]= '\0';
    }
    y=0;
    }
  }
  if (data[0] == 'n') { //store in an array called blinklength the sequence of the number of repeated trigger we want for each led and on which later leds will trigger
    int blinkdata = strtol(data+1, NULL, 10);
    blinkdata = constrain(blinkdata,1,30); 
    transferdata[y] = blinkdata;
    y=y+1;
    if (y==3){                                                // when transferdata is full of data (we expect 3 speeds value between 1 and 30)
    memcpy(Nofblinks, transferdata, sizeof(transferdata));   //copying transferdata to Nofblinks then clearing transferdata and resetting y, 
    for (int i=0; i<=sizeof(transferdata); i++){
    transferdata[i]= '\0';
    }
    y=0;
    }
  }
     
if (data[0] =='M'){ //send command M to view on serial console the currently stored value stored in ledmode[]
  for (int i=0;i<=1;i++){
     if(ledmode[i]!= NULL){
      Serial.print("ledmode is set to: ");
     Serial.println(ledmode[i]);
      Serial.println(" ");
     }
    }
  }
if (data[0] =='D'){       //send command S to view on serial console the currently stored value stored in blinkdelay[]
     for (int i=0;i<=2;i++){
     if(blinkdelay[i]!= NULL){
      Serial.print("blinkdelay for led ");
      Serial.print(i+1);
      Serial.print(" is:");
     Serial.println(blinkdelay[i]);
     if (i==2){
      Serial.println(" ");}
     }
    }
  }

if (data[0] =='N'){       //send command L to view on serial console the currently stored value stored in Nofblinks[]
     for (int i=0;i<=2;i++){
     if(Nofblinks[i]!= NULL){
      Serial.print("Nofblinks for led");
      Serial.print(i+1);
      Serial.print(" is:");
     Serial.println(Nofblinks[i]);
     if (i==2){
      Serial.println(" ");}
     }
    }
  }
 }

void setLEDon(char* data) {
if(ledmode[0]==1){
  if (data[0] == 'u') {         //if i receive the command 'u' from serial (upvote) and if ledmode[0] is set to 1 (standard led mode) do the blinking   
    for (int a= 0; a< Nofblinks[0]; a++){
      digitalWrite(LED_ONE, HIGH);
      delay(blinkdelay[0]);
      digitalWrite(LED_ONE, LOW);
      delay(blinkdelay[0]);
      }
  Serial.println("you got an upvote ");
   Serial.println(" ");  
  }
  
if (data[0] == 'f') { //if i receive the command f from serial (follower) and if ledmode[0] is set to 1 (standard led mode) do the blinking   
    for (int a= 0; a< Nofblinks[1]; a++){
      digitalWrite(LED_TWO, HIGH);
      delay(blinkdelay[1]);
      digitalWrite(LED_TWO, LOW);
      delay(blinkdelay[1]);  
      }
Serial.println("you got a new follower ");
 Serial.println(" ");
}

if (data[0] == 'p') { //if i receive the command p from serial (new post) and if ledmode[0] is set to 1 (standard led mode) do the blinking   
    for (int a= 0; a< Nofblinks[2]; a++){
      digitalWrite(LED_THREE, HIGH);
      delay(blinkdelay[2]);
      digitalWrite(LED_THREE, LOW);
      delay(blinkdelay[2]);  
      }
Serial.println("there is a new post");
 Serial.println(" ");
  }
}
}

