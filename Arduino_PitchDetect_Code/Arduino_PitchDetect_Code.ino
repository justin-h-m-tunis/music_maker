#include <math.h>
#include <SD.h>                           //include SD module library
#include <TMRpcm.h>                       //include speaker control library
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define SD_ChipSelectPin 4                //define CS pin

TMRpcm tmrpcm;   

int led = 7; 

int count = 0;
int state = 0;

int valNewR = 0; 
int valOldR = 0; 

int valNewP = 0; 
int valOldP = 0; 

int valNewY = 0;
int valOldY = 0; 

const int x_out = A1; /* connect x_out of module to A1 of UNO board */
const int y_out = A2; /* connect y_out of module to A2 of UNO board */
const int z_out = A3; /* connect z_out of module to A3 of UNO board */

SoftwareSerial mySoftwareSerial(9, 11); // RX, TX // WAS CHANGED TO 9 FROM 10 
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
  Serial.begin(9600); 
  mySoftwareSerial.begin(9600);
  
  pinMode(led, OUTPUT);
  
 // if (!SD.begin(SD_ChipSelectPin)) {      //see if the card is present and can be initialized
    
    //return;                               //don't do anything more if not
  //}
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
  
  
}

void loop() {
  int x_adc_value, y_adc_value, z_adc_value; 
  double x_g_value, y_g_value, z_g_value;
  double roll, pitch, yaw;
  x_adc_value = analogRead(x_out); /* Digital value of voltage on x_out pin */ 
  y_adc_value = analogRead(y_out); /* Digital value of voltage on y_out pin */ 
  z_adc_value = analogRead(z_out); /* Digital value of voltage on z_out pin */ 
  Serial.print("x = ");
  Serial.print(x_adc_value);
  Serial.print("\t\t");
  Serial.print("y = ");
  Serial.print(y_adc_value);
  Serial.print("\t\t");
  Serial.print("z = ");
  Serial.print(z_adc_value);
  Serial.print("\t\t");
  
  
  x_g_value = ( ( ( (double)(x_adc_value * 5)/1024) - 1.65 ) / 0.330 ); /* Acceleration in x-direction in g units */ 
  y_g_value = ( ( ( (double)(y_adc_value * 5)/1024) - 1.65 ) / 0.330 ); /* Acceleration in y-direction in g units */ 
  z_g_value = ( ( ( (double)(z_adc_value * 5)/1024) - 1.80 ) / 0.330 ); /* Acceleration in z-direction in g units */ 

  roll = ( ( (atan2(y_g_value,z_g_value) * 180) / 3.14 ) + 180 ); /* Formula for roll */
  pitch = ( ( (atan2(z_g_value,x_g_value) * 180) / 3.14 ) + 180 ); /* Formula for pitch */
  yaw = ( ( (atan2(x_g_value,y_g_value) * 180) / 3.14 ) + 180 ); /* Formula for yaw */
  /* Not possible to measure yaw using accelerometer. Gyroscope must be used if yaw is also required */


  
  Serial.print("Roll = ");
  Serial.print(roll);
  Serial.print("\t");
  Serial.print("Pitch = ");
  Serial.print(pitch);
  Serial.print("\t");
  Serial.print("Yaw = ");
  Serial.print(yaw);
  Serial.print("\t");
  Serial.print("Count = ");
  Serial.print(count);
  Serial.print("\t");
  Serial.print("State = ");
  Serial.print(state);
  Serial.print("\n\n");
  
  
  static unsigned long timer = millis();

  valNewR = roll; 
  valNewP = pitch; 
  valNewY = yaw; 
  

  int diffRoll = valNewR - valOldR;
  int diffPitch = valNewP - valOldP; 
  int diffYaw = valNewY - valOldY; 

    if (diffRoll > 30  || diffPitch > 30 || diffYaw > 30) {
      state = 1;
    }

    if (state == 1) {
      count = count + 1;
      myDFPlayer.next();
      digitalWrite(led, HIGH);
      if (count > 5) {
        state = 0; 
        count = 0;
      }
    }
    else {
        digitalWrite(led, LOW);
        myDFPlayer.pause(); 
    }
    
  delay(1000);
  valOldR = valNewR;
  valOldP = valNewP; 
  valOldY = valNewY;
  
    //if (pitch > 150) {
      //myDFPlayer.next();  //Play next mp3 every 3 second.
     // digitalWrite(led, HIGH);
   // }
    
    //if (roll < 10) {
      //myDFPlayer.next();  //Play next mp3 every 3 second.
      //digitalWrite(led, LOW);
    //}

    //if (pitch < 150) {
      //myDFPlayer.next();  //Play next mp3 every 3 second.
      //digitalWrite(led, LOW);
    //}
    
}
