/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo servodrive;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
//  servodrive.attach(3, 600, 2260);  // S05NF STD grip
//  servodrive.attach(3, 630, 2460);  // hand roll
  servodrive.attach(3, 540, 2300);  // hand pivot
    
}

void loop() {
//  servodrive.writeMicroseconds(700);
  
//  servodrive.write(180);
//  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servodrive.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//  delay(2000);
  servodrive.write(90);
//  delay(2000);   
//  servodrive.write(0);
//  delay(2000);   
//  servodrive.write(90);
//  delay(4000);
  
//  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    servodrive.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(60);                       // waits 15ms for the servo to reach the position
//  }
//  delay(1000);
}
