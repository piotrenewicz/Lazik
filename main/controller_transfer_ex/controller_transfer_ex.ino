#include <EasyTransfer.h>
#include <Esplora.h>



EasyTransfer ET;
EasyTransfer ETdebug;

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

struct RECEIVE_DATA_STRUCTURE{
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
RECEIVE_DATA_STRUCTURE debug;


void setup() {
  initialize_command(); //do not use the usb serial, ... Best way to avoid corrupting the bootloader
  Serial1.begin(9600);
  Serial.begin(9600);
  ET.begin(details(command), &Serial1);
//  ETdebug.begin(details(debug), &Serial1);/
  pinMode(13, OUTPUT);
}


void loop() {
  primary_control_junction();
  ET.sendData();
  delay(40);
  for(int i=0; i<4; i++){
//    pass_debug_through();/
  }
}

void debug_tmp(){
  Serial.print("trg_x=");
  Serial.println(command.target_x);
  Serial.print("trg_y=");
  Serial.println(command.target_y);
  Serial.print("len_a=");
  Serial.println(command.len_a);
  Serial.print("deg_a=");
  Serial.println(command.deg_a);
}

void pass_debug_through(){
  if(ETdebug.receiveData()){
    Serial.println("=====================");
    
    Serial.print("alpha=");
    Serial.println(debug.alpha);
    Serial.print("bx   =");
    Serial.println(debug.bx);
    Serial.print("by   =");
    Serial.println(debug.by);
    Serial.print("f    =");
    Serial.println(debug.f);
    Serial.print("beta =");
    Serial.println(debug.beta);
    Serial.print("beta1=");
    Serial.println(debug.beta1);
    Serial.print("beta2=");
    Serial.println(debug.beta2);
    Serial.print("beta3=");
    Serial.println(debug.beta3);
    Serial.print("gamma=");
    Serial.println(debug.gamma);
    Serial.print("delta=");
    Serial.println(debug.delta);

    Serial.println("=====================");
    debug_tmp();
    Serial.println("=====================");
  }
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

int apply_deadzone(int in){
  int deadzone = 10;
  if(abs(in) < deadzone){
    return 0;
  }
  else{
    return in;
  }
  
}

void joytodrive(){
  command.turn = apply_deadzone(Esplora.readJoystickX());
  command.drive = apply_deadzone(Esplora.readJoystickY());
}

void slidetogrip(){
  command.grip = Esplora.readSlider();
}

void joytotarget(){
  command.target_x -= apply_deadzone(Esplora.readJoystickX())/100.0;
  command.target_y -= apply_deadzone(Esplora.readJoystickY())/100.0;
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
  command.len_a -= apply_deadzone(Esplora.readJoystickX())/100.0;
  command.deg_a -= apply_deadzone(Esplora.readJoystickY())/100.0;
}
