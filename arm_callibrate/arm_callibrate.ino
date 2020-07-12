/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo servodrive;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  servodrive.attach(17, 550, 2370); 
}

void loop() {
//  servodrive.writeMicroseconds(700);
  
//  servodrive.write(0);
//  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servodrive.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//  delay(2000);
//  delay(2000);   
  servodrive.write(90);
  delay(2000);   
// servodrive.writeMicroseconds(map(0, -45, 180+45, 500, 2500));
  delay(2000);
  
//  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    servodrive.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(60);                       // waits 15ms for the servo to reach the position
//  }
//  delay(1000);
}
