#include <EasyTransfer.h>

//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
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

  bool custom_command_number;
};

//give a name to the group of data
SEND_DATA_STRUCTURE command;

void setup(){
  Serial1.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(command), &Serial1);
  
  pinMode(13, OUTPUT);
  
}

void loop(){
  //this is how you access the variables. [name of the group].[variable name]


  DATA PROCESSING HERE
  
  //send the data
  ET.sendData();
  
  //Just for fun, we will blink it out too
  
  
  delay(5000);
}
