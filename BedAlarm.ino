
 /*
 * This is Arduino Sketch for Tutorial video 
 * explaining the 15A MOSFET AOD4184A used as switch 
 * this sketch used to test the moseft with load at 5A, 10A, 15A, 25A
 * 
 * Written by Ahmad Shamshiri on July 21, 2018 in Ajax, Ontario, Canada
 * For Robojax.com
 * Watch instruciton video for this code:https://youtu.be/tCJ2Q-CT6Q8 
 * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.
 */
int loadPin =1;// load pin

  
void setup() {
  // Robojax.com demo
  pinMode(loadPin,OUTPUT);
  Serial.begin(9600);
  Serial.println("Robojax Demo"); 

}

void loop() {
  // Robojax.com  tutorial

  digitalWrite(loadPin, HIGH);
 while(1);// wait forever


} 
