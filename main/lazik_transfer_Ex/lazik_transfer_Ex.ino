#include <EasyTransfer.h>
#include <Servo.h>
#include <math.h>

EasyTransfer ET;

Servo servo_grip;
Servo servo_twist;
Servo servo_wrist;
Servo servo_elbow;
Servo servo_shoulder;

Servo servo_left_track;
Servo servo_right_track;

/*
   hand_grip.write(90);
   hand_roll.write(90);
   hand_pivot.write(90);
   elbow.write(160);
   shoulder.writeMicroseconds(map(0, -45, 180+45, 500, 2500));
*/


struct RECEIVE_DATA_STRUCTURE {
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


struct SERVO_SIGNALS {
  int16_t wrist;
  int16_t elbow;
  int16_t shoulder;
  int16_t left_track;
  int16_t right_track;
};
SERVO_SIGNALS signals;


void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  ET.begin(details(command), &Serial);

  // remember to servo attach the tracks here
};

bool current_arm_power  = false;
bool current_drive_power = false;

void loop() {
  if (ET.receiveData()) {
    digitalWrite(13, command.arm_power);
    primary_parser();


  }
  //you should make this delay shorter then your transmit delay or else messages could be lost
  delay(20);
};


void primary_parser() {
  driver();
  if (current_drive_power) {
    driver_writer();
  }
  arm_power_ctrl();
  if (current_arm_power) {
    arm_model_calc();
    arm_writer();
  }
};


void driver() {
  command.drive;
  command.turn;

  signals.left_track;
  signals.right_track;

};

void driver_writer() {
  servo_track_left.writeMicroseconds(signals.track_left, 0, 512, 1500, 2500);
  servo_track_right.writeMicroseconds(signals.track_right, 0, 512, 1500, 2500);  // values to callibrate
};

void arm_power_ctrl() {
  if (command.arm_power != current_arm_power) {
    if (command.arm_power) {
      arm_attach();
    }
    else {
      arm_detach();
    }

    current_arm_power = command.arm_power;
  }
};

void arm_model_calc() {
  // welcome
  const int a = 105;
  const int b = 105;
  const int c = 140;


  float bx = command.target_x + cos(command.deg_a) * (command.len_a + c); //bx = -10
  float by = command.target_y + sin(command.deg_a) * (command.len_a + c); //by = 100

  float f  = sqrt(pow(bx, 2) + pow(by, 2)); // 99.49874371

  float beta3 = M_PI / 2 - command.deg_a; // pi/2
  float beta2 = M_PI / 2 - atan2(by, -bx); // 0.09966865249
  float beta1 = acos((f / b + b / f - pow(a, 2) / (f * b)) / 2); // 1.077191431
  float beta = beta1 + beta2 + beta3; // 2.74765641028

  float gamma = acos((a / b + b / a - (pow(bx, 2) + pow(by, 2)) / (a * b)) / 2); // 0.9872097919

  float delta = beta1 + beta2 + gamma - M_PI / 2 // 0.5932735486

  signals.wrist = beta;
  signals.elbow = gamma;
  signals.shoulder = delta;
};

void arm_writer() {
  servo_grip.writeMicroseconds(map(command.grip, 0, 1024, 600, 2260));
  servo_twist.writeMicroseconds(map(command.twist, 0, 1024, 630, 2460));

  servo_wrist.writeMicroseconds(map(signals.wrist, M_PI / 2, 3 * M_PI / 2, 540, 2300));
  servo_elbow.writeMicroseconds(map(signals.elbow, 0, M_PI, 2370, 550));
  servo_shoulder.writeMicroseconds(map(map(signals.shoulder, -M_PI/4, M_PI+M_PI/4, 500, 2500);
};

void arm_attach() {
  servo_grip.attach(14, 600, 2260);  // S05NF STD grip
  servo_twist.attach(15, 630, 2460);  // S3003 hand roll
  servo_wrist.attach(16, 540, 2300);  // S3003 hand pivot
  servo_elbow.attach(17, 550, 2370);  // MG946R elbow
  servo_shoulder.attach(18, 500, 2500); // LF-20MG shoulder
};


void arm_detach() {
  servo_grip.detach();
  servo_twist.detach();  // S3003 hand roll
  servo_wrist.detach();  // S3003 hand pivot
  servo_elbow.detach();  // MG946R elbow
  servo_shoulder.detach(); // LF-20MG shoulder
}
