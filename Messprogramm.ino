/* 
  Initial test to get readings from multiple buttons on two analog pins
  
  This will just print out the readings taken on the analog pins to the serial 
  port. As long as you hold down the button, the values will be printed.
  
  Created 5 February 2011       Modified and Augmented 01 June 2012
  by Riaan Cornelius		by Scratch (http://www.youtube.com/user/WhiteRavingPanther)
  */

 const int buttonPin1 = 0;     // the number of the analog pins
 const int buttonPin2 = 1;
 
 void setup() {
   pinMode(buttonPin1, INPUT);
   pinMode(buttonPin2, INPUT);
   Serial.begin(9600);
 }

 void loop() {
   // Just print the value to the serial port if it is more than 1;
   int reading1 = analogRead(buttonPin1);
 if(reading1 > 1){
     Serial.println(reading1);
   }
      int reading2 = analogRead(buttonPin2);
 if(reading2 > 1){
     Serial.println(reading2);
   }
 }
