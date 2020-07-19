#include <EasyTransfer.h>
#include <Servo.h>

EasyTransfer ET; 

Servo servo_grip;
Servo servo_twist;
Servo servo_wrist;
Servo servo_elbow; 
Servo servo_shoulder;

Servo servo_left_track;
Servo servo_right_track;

/*
 * hand_grip.write(90);
 * hand_roll.write(90);
 * hand_pivot.write(90);
 * elbow.write(160);
 * shoulder.writeMicroseconds(map(0, -45, 180+45, 500, 2500));
 */


struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  float target_x;
  float target_y;
  float len_a;
  float deg_a;

  int16_t grip;      // 0 -- 1024
  int16_t twist;     // 0 -- 1024

  int16_t drive;   // -512 -- 0 -- 512
  int16_t turn;    // -512 -- 0 -- 512

  bool arm_power;
};
RECEIVE_DATA_STRUCTURE command;


struct SERVO_SIGNALS{
  int16_t wrist;
  int16_t elbow;
  int16_t shoulder;
  int16_t left_track;
  int16_t right_track;
};
SERVO_SIGNALS signals;


void setup(){
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  ET.begin(details(command), &Serial);

  // remember to servo attach the tracks here 
};

bool current_arm_power  = false;
bool current_drive_power = false;

void loop(){
  if(ET.receiveData()){ 
    digitalWrite(13, command.arm_power);
    primary_parser();
  
  
  }
  //you should make this delay shorter then your transmit delay or else messages could be lost
  delay(20);
};


void primary_parser(){
  driver();
  if(current_drive_power){
    driver_writer();  
  }
  arm_power_ctrl();
  if(current_arm_power){
    arm_model_calc();
    arm_writer();
  }
};


void driver(){
  command.drive;
  command.turn;

  signals.left_track;
  signals.right_track;
   
};

void driver_writer(){
  
};

void arm_power_ctrl(){
  if(command.arm_power != current_arm_power){
    if(command.arm_power){
      arm_attach();
    }
    else{
      arm_detach();
    }

    current_arm_power = command.arm_power;
  }
};

void arm_model_calc(){
  
};

void arm_writer(){
  servo_grip.writeMicroseconds(map(command.grip, 0, 1024, 600, 2260));
  servo_twist.writeMicroseconds(map(command.twist, 0, 1024, 630, 2460));
};

void arm_attach() {
  servo_grip.attach(14, 600, 2260);  // S05NF STD grip
  servo_twist.attach(15, 630, 2460);  // S3003 hand roll
  servo_wrist.attach(16, 540, 2300);  // S3003 hand pivot
  servo_elbow.attach(17, 550, 2370);  // MG946R elbow
  servo_shoulder.attach(18, 500, 2500); // LF-20MG shoulder
};


void arm_detach(){
  servo_grip.detach();
  servo_twist.detach();  // S3003 hand roll
  servo_wrist.detach();  // S3003 hand pivot
  servo_elbow.detach();  // MG946R elbow
  servo_shoulder.detach(); // LF-20MG shoulder
}
