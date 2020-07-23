#include <EasyTransfer.h>
#include <Esplora.h>



EasyTransfer ET;

struct SEND_DATA_STRUCTURE {
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
SEND_DATA_STRUCTURE command;


void setup() {
  initialize_command(); //do not use the usb serial, ... Best way to avoid corrupting the bootloader
  Serial1.begin(9600);
  ET.begin(details(command), &Serial1);
  pinMode(13, OUTPUT);
}


void loop() {
  primary_control_junction();
  ET.sendData();
  delay(40);
}


void initialize_command() {
  init_target();
  
  command.grip = 512;
  command.twist = 512;

  command.drive = 0;
  command.turn = 0;

  command.arm_power = false;
}

void init_target(){
  command.target_x = 210.0;  // 10
  command.target_y = 70.0;   // 240
  command.len_a = 0.0;
  command.deg_a = 0.0;        // -90
}




int menu_mode = 0;
/*
 * Mode 0 - Drive mode, 
 *  joystick will control driving, (no input will mean a detach) ^
 *  slider will control grip ^
 *  BL will set arm to default pos (grip not included) ^
 *  BD will set arm to rest pos
 *  BR will toggle arm_power ^
 *  BU will change mode to 1 ^
 *  
 * Mode 1 - Arm mode, (LED 13 is on in this mode)
 *  joystick(A) will move the target, xy, ^
 *  slider will control grip ^
 *  BU will change mode to 0 ^
 *  BD will change behaviour of joystick(B), to control distance and attack angle
 *  BL BR will control twist LR over time held.
 *  
 * 
 */

void primary_control_junction(){
//  slidetogrip();
  if(menu_mode == 0){
    mode0();
  }
  else if(menu_mode == 1){
    if(Esplora.readButton(SWITCH_DOWN)){
      mode1a();
    }
    else{
      mode1b();
    }
  }

  // data send, perhaps check for changes, maybe a changes flag from the deeper?
}

void joytodrive(){
  command.turn = Esplora.readJoystickX();
  command.drive = Esplora.readJoystickY();
}

void slidetogrip(){
  command.grip = Esplora.readSlider();
}

void joytotarget(){
  command.twist += map(Esplora.readJoystickX(), -400, 400, -5, 5);
  command.grip += map(Esplora.readJoystickY(), -400, 400, -5, 5);
//
//  command.target_x += map(Esplora.readJoystickX(), -512, 512, -1.0, 1.0);
//  command.target_y += map(Esplora.readJoystickY(), -512, 512, -1.0, 1.0);
}

void mode0(){
  if(!Esplora.readButton(SWITCH_LEFT)){
    while(!Esplora.readButton(SWITCH_LEFT)){}
    init_target();
  }

  else if(!Esplora.readButton(SWITCH_RIGHT)){
    while(!Esplora.readButton(SWITCH_RIGHT)){}
    command.arm_power = !command.arm_power;
  }

  else if(!Esplora.readButton(SWITCH_UP)){
    while(!Esplora.readButton(SWITCH_UP)){}
    menu_mode = 1;
  }

  else {
    digitalWrite(13, LOW);
    joytodrive();
  }
}

void mode1a(){
  if(!Esplora.readButton(SWITCH_UP)){
    while(!Esplora.readButton(SWITCH_UP)){}
    menu_mode = 0;
  }

  else if(!Esplora.readButton(SWITCH_LEFT)){
    command.twist += 1;  // this needs callibration to make the speed controllable
  }
  else if(!Esplora.readButton(SWITCH_RIGHT)){
    command.twist -= 1;
  }
  
  else {
    digitalWrite(13, HIGH);
    joytotarget();
  }
}

void mode1b(){
  command.len_a += map(Esplora.readJoystickX(), -512, 512, -1.0, 1.0);
  command.deg_a += map(Esplora.readJoystickY(), -512, 512, -1.0, 1.0);
}
