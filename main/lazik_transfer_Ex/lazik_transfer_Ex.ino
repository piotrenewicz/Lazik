#include <EasyTransfer.h>
#include <Servo.h>
#include <math.h>

EasyTransfer ET;
EasyTransfer ETdebug;

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
  float wrist;
  float elbow;
  float shoulder;
  int16_t left_track;
  int16_t right_track;
};
SERVO_SIGNALS signals;


struct SEND_DATA_STRUCTURE {
  float alpha;
  float bx;
  float by;
  float f;
  float beta;
  float beta1;
  float beta2;
  float beta3;
  float gamma;
  float delta;
};
SEND_DATA_STRUCTURE debug;


void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  ET.begin(details(command), &Serial);
  ETdebug.begin(details(debug), &Serial);

  // remember to servo attach the tracks here
};

bool current_arm_power  = false;
bool current_drive_power = false;

void loop() {
  if (ET.receiveData()) {
    digitalWrite(13, command.arm_power);
    primary_parser();
    ETdebug.sendData();


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
  servo_left_track.writeMicroseconds(map(signals.left_track, 0, 512, 1500, 2500));
  servo_right_track.writeMicroseconds(map(signals.right_track, 0, 512, 1500, 2500));  // values to callibrate
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

  float alpha = command.deg_a * M_PI/180;

  float bx = command.target_x - cos(alpha) * (command.len_a + c); //bx = 70
  float by = command.target_y + sin(alpha) * (command.len_a + c); //by = 70

  float f  = sqrt(pow(bx, 2) + pow(by, 2)); // 98.994949

  float beta3 = M_PI / 2 - alpha; // pi/2
  float beta2 = M_PI / 2 - atan2(by, bx); // 45
  float beta1 = acos((f / b + b / f - pow(a, 2) / (f * b)) / 2); //
  float beta = beta1 + beta2 + beta3; // 180

  float gamma = acos((2*pow(a, 2) - pow(f, 2)) / (2 * pow(a,2))); // 30

  float delta = beta1 + beta2 + gamma - M_PI / 2; // 30

  if(!(isnan(beta) || isnan(gamma) || isnan(delta))){  
    signals.wrist = beta;
    signals.elbow = gamma;
    signals.shoulder = delta;
  }
  
  debug.alpha = alpha;
  debug.bx = bx;
  debug.by = by;
  debug.f = f;
  debug.beta = beta*180/M_PI;
  debug.beta1 = beta1*180/M_PI;
  debug.beta2 = beta2*180/M_PI;
  debug.beta3 = beta3*180/M_PI;
//  debug.gamma = gamma*180/M_PI;
//  debug.delta = delta*180/M_PI;
  debug.gamma = fmap(signals.elbow, M_PI, 0, 550, 2370);
  debug.delta = fmap(signals.shoulder, -M_PI/4, M_PI+M_PI/4, 500, 2500);

};

float fmap(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void arm_writer() {
  servo_grip.writeMicroseconds(map(command.grip, 0, 1024, 600, 2260));
  servo_twist.writeMicroseconds(map(command.twist, 0, 1024, 630, 2460));

  servo_wrist.writeMicroseconds(int(fmap(signals.wrist, M_PI / 2, M_PI + M_PI / 2, 540, 2300)));
  servo_elbow.writeMicroseconds(int(fmap(signals.elbow, M_PI, 0, 550, 2370)));
  servo_shoulder.writeMicroseconds(int(fmap(signals.shoulder, -M_PI/4, M_PI+M_PI/4, 500, 2500)));
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
