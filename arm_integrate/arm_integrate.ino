#include <Servo.h>

Servo hand_grip;
Servo hand_roll;  // create servo object to control a servo
Servo hand_pivot;
Servo elbow; 
Servo shoulder;
// twelve servo objects can be created on most boards
bool test = false;

int pos = 90;    // variable to store the servo position
int velocity = 1;
Servo all_joints[] = {hand_grip, hand_roll, hand_pivot, elbow};

void setup() {
  hand_grip.attach(14, 600, 2260);  // S05NF STD grip
  hand_roll.attach(15, 630, 2460);  // S3003 hand roll
  hand_pivot.attach(16, 540, 2300);  // S3003 hand pivot
  elbow.attach(17, 550, 2370);  // MG946R elbow
  shoulder.attach(18, 500, 2500); // LF-20MG shoulder

  hand_grip.write(90);
  hand_roll.write(90);
  hand_pivot.write(90);
  elbow.write(160);
  shoulder.writeMicroseconds(map(0, -45, 180+45, 500, 2500));

  test = false;
}

int idx = 0;

void loop() {
//  
//  hand_grip.write(180);
//  shoulder.writeMicroseconds(map(0, -45, 180+45, 500, 2500));
//  elbow.write(90);
//  delay(2000);   
//  hand_grip.write(0);
//  shoulder.writeMicroseconds(map(180, -45, 180+45, 500, 2500));
//  elbow.write(90);
//  delay(2000);


}


