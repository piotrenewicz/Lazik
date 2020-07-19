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
  command.target_x = -10.0;
  command.target_y = 5.0;
  command.len_a = 0.0;
  command.deg_a = 0.0;
}




int menu_mode = 0;
/*
 * Mode 0 - Drive mode, 
 *  joystick will control driving, (no input will mean a detach) ^
 *  slider will control grip ^
 *  BL will set arm to default pos (grip not included) ^
 *  BR will send custom 1 (detach arm) ^
 *  BR again will send custom 2 (attach arm) ^
 *  BU will change mode to 1 ^
 *  
 * Mode 1 - Basic arm mode,
 *  joystick will move the target, xy, ^
 *  slider will control grip ^
 *  BU will change mode to 0 ^
 *  BD will change mode to 2 ^
 *  
 * Mode 2 - Full arm mode, (led on esplora is lit in this mode)
 *  slider does nothing, this allows it to be prepared for custom
 *  BU will change mode to 0
 *  BD will change mode to 1
 *  joystick will move the target xy,
 *  holding BL changes behaviour:
 *    joystick will control target distance and angle,
 *    slider will control grip
 *  holding BR changes behaviour:
 *    joustick will move the target xy, while keeping the b point static 
 *      (will update target distance and angle to counter the change)
 *    or 
 *    joystick will allow for flick controlling the angle of target.
 *    slider will control twist
 * 
 */

void primary_control_junction(){
  if(menu_mode == 0){
    mode0();
  }
  else if(menu_mode == 1){
    mode1();
  }
  else if(menu_mode == 2){
    secondary_control_junction();
  }

  
  // data send, perhaps check for changes, maybe a changes flag from the deeper?
}

void secondary_control_junction(){
  if(not Esplora.readButton(SWITCH_LEFT)){
    mode3b();
  }
  else if (not Esplora.readButton(SWITCH_RIGHT)){
    mode3c();
  }
  else{
    mode3a();
  }
}

void joytodrive(){
  command.turn = Esplora.readJoystickX();
  command.drive = Esplora.readJoystickY();
}

void slidetogrip(){
  command.grip = Esplora.readSlider();
}

void joytotarget(){
  command.target_x += map(Esplora.readJoystickX(), -512, 512, -1.0, 1.0);
  command.target_y += map(Esplora.readJoystickY(), -512, 512, -1.0, 1.0);
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
    slidetogrip();
  }
}

void mode1(){
  if(!Esplora.readButton(SWITCH_UP)){
    while(!Esplora.readButton(SWITCH_UP)){}
    menu_mode = 0;
  }

  else if(!Esplora.readButton(SWITCH_DOWN)){
    while(!Esplora.readButton(SWITCH_DOWN)){}
    menu_mode = 2;
  }
  
  else {
    digitalWrite(13, HIGH);
    joytotarget();
    slidetogrip();
  }
}

void mode3a(){

}

void mode3b(){

}

void mode3c(){

}
