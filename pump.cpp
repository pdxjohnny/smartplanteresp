#include "pump.h"

Pump::Pump(int pinNum, int typeIn) {
	// initialize pin
  pinNumber = pinNum;
  type = typeIn;

  pinMode(pinNumber, OUTPUT);
}

void Pump::pumpWater() {
  digitalWrite(pinNumber, HIGH);

  // delay some amount of time based on what the pump is for
  if(type == WATER_TYPE) {
    delay(DISPENCE_ONE_WATER_TIME);
  }
  else if(type == FERTILIZER_TYPE) {
    delay(DISPENCE_ONE_FERTILIZER_TIME);
  }
  else {
    // should not reach here
    Serial.println("Pump error: Invalid TYPE");
  }
  
  digitalWrite(pinNumber, LOW);
	// return nothing
}
