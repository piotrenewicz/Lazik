#include <Esplora.h>

void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.print("X:");
  Serial.print(Esplora.readJoystickX());
  Serial.print(" Y:");
  Serial.println   (Esplora.readJoystickY());
  
}
