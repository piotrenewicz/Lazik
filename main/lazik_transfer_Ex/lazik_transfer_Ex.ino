#include <EasyTransfer.h>

//create object
EasyTransfer ET; 

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
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
RECEIVE_DATA_STRUCTURE command;

void setup(){
  Serial.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc. 
  ET.begin(details(command), &Serial);
  
  pinMode(13, OUTPUT);
  
}

void loop(){
  //check and see if a data packet has come in. 
  if(ET.receiveData()){
    //this is how you access the variables. [name of the group].[variable name]
    //since we have data, we will blink it out. 
     DATA PROCESSING HERE
    }
  }
  
  //you should make this delay shorter then your transmit delay or else messages could be lost
  delay(250);
}
