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

  float grip;
  float twist;

  float drive;
  float turn;

  bool drive_power;
  bool arm_power;

  int16_t custom;
};
SEND_DATA_STRUCTURE command;


void setup() {
  initialize_command();

  //  do not use the usb serial, ... Best way to avoid corrupting the bootloader
  Serial1.begin(9600);
  ET.begin(details(command), &Serial1);

  pinMode(13, OUTPUT);

}


void initialize_command() {
  command.target_x = -10.0;
  command.target_y = 5.0;
  command.len_a = 0.0;
  command.deg_a = 0.0;

  command.grip = 0.0;
  command.twist = 0.0;

  command.drive = 0.0;
  command.turn = 0.0;

  command.drive_power = true;
  command.arm_power = false;

  command.custom = 0;
}


void loop() {

  ET.sendData();
  delay(120);
}

int menu_mode = 0;
/*
 * Mode 0 - Drive mode, 
 *  joystick will control driving, (no input will mean a detach)
 *  slider will control grip
 *  BL will set arm to default pos (grip not included)
 *  BR will send custom 1 (detach arm)
 *  BR again will send custom 2 (attach arm)
 *  BU will change mode to 1
 *  
 * Mode 1 - Basic arm mode,
 *  joystick will move the target, xy,
 *  slider will control grip
 *  BU will change mode to 0
 *  BD will change mode to 2
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
    delay(10);
  }
  else if(menu_mode == 1){
    delay(10);
  }
  else if(menu_mode == 2){
    secondary_control_junction();
  }

  // data send, perhaps check for changes, maybe a changes flag from the deeper?
}

void secondary_control_junction(){
  if(not Esplora.readButton(SWITCH_LEFT)){
    delay(10);
  }
  else if (not Esplora.readButton(SWITCH_RIGHT)){
    delay(10);
  }
  else{
    delay(10);
  }
}
