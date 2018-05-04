#include "waterLevel.h"

WaterLevel::WaterLevel(int pinNum) {
  // set pin number
  pinNumber = pinNum;
  // initialize pin mode
  pinMode(pinNumber, INPUT);
}

bool WaterLevel::waterPresent() {
	// read from sensor
	// read 1 if water presents, 0 if not
  return digitalRead(pinNumber);
}
